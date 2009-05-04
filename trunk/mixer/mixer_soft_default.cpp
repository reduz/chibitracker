//
// C++ Implementation: mixer_soft_default
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "mixer_soft.h"
#include "sample_defs.h"
#include <stdio.h>
#include <math.h>


#define SPLINE_QUANTBITS	14
#define SPLINE_QUANTSCALE	(1L<<SPLINE_QUANTBITS)
#define SPLINE_8SHIFT		(SPLINE_QUANTBITS-8)
#define SPLINE_16SHIFT		(SPLINE_QUANTBITS)
// forces coefsset to unity gain
#define SPLINE_CLAMPFORUNITY
// log2(number) of precalculated splines (range is [4..14])

#define SPLINE_FRACSHIFT ((MIXING_FRAC_BITS-SPLINE_FRACBITS)-2)
#define SPLINE_FRACMASK (((1L<<(MIXING_FRAC_BITS-SPLINE_FRACSHIFT))-1)&~3)

#define NO_ASSEMBLER 1

#if (__i386__ && __GNUC__ >= 3 && !NO_ASSEMBLER)
#	define X86_ASM_MIXER
#endif


#ifdef X86_ASM_MIXER

/** GCC HAX0RING for x86 gcc.. assembler mixing with forced noinline (otherwise it will inline the template and crash)*/


template<class Depth,bool use_filter,bool use_vramp,InterpolationType type>
__attribute__((noinline)) void do_resample (
		
	
	volatile const Depth* p_src, //8
	volatile Sint32 *p_dst, // 12
	volatile Uint32 p_amount, //16
	volatile Sint32 p_increment, //20
	volatile Sint32 p_pos, //24
	volatile Sint32 p_lvol, //28
	volatile Sint32 p_rvol, //32
	volatile Sint32 p_lvol_inc, //36
	volatile Sint32 p_rvol_inc //40
					   )  {


	asm("pushl %esi");
	asm("pushl %edi");
	asm("pushl %eax");
	asm("pushl %ebx");
	asm("pushl %ecx");
	asm("pushl %edx");
	asm("movl 8(%ebp),%esi");
	asm("movl 12(%ebp),%edi");
	asm("movl 16(%ebp),%ecx"); //loop
	
	asm("1:");
	asm("pushl %ecx");	
	
	asm("movl 24(%ebp),%edx"); // p_pos -> edx
	
	asm("sarl $"MIXING_FRAC_BITS_TEXT",%edx"); // convert to raw

	if (sizeof(Depth)==2) //condition removed at compile time
		asm("sall $1,%edx");
	
	asm("pushl %esi"); //save ESI
	asm("addl %edx,%esi"); //move esi by offset
	
	if (type==INTERPOLATION_LINEAR) {
		
		asm("movl 24(%ebp),%edx");
		asm("andl $"MIXING_FRAC_BITS_MASK_TEXT",%edx"); //edx -> fractional
		
	}
	 
	if (sizeof(Depth)==1) { //condition removed at compile time
		//8 bits sample
		asm("movsxb (%esi),%ebx");
		asm("sall $8,%ebx");
		if (type==INTERPOLATION_LINEAR) { //condition removed at compile time
			
			asm("movsxb 1(%esi),%eax");
			asm("sall $8,%eax");
		}
	} else { //condition removed at compile time
		//16 bits sample
		asm("movsxw (%esi),%ebx");		
		if (type==INTERPOLATION_LINEAR) {
			
			asm("movsxw 2(%esi),%eax");
		}
	}
	
	
	asm("popl %esi");
		
	if (type==INTERPOLATION_LINEAR) { //condition removed at compile time
	
		asm("subl %ebx,%eax");
		asm("imul %edx");
		asm("sarl $"MIXING_FRAC_BITS_TEXT",%eax");
		asm("addl %eax,%ebx");
	}
	
	/// Filter here 
	
	/// Apply left volume
	asm("movl 28(%ebp),%eax"); //save it
	asm("imul %ebx");
	asm("addl %eax,(%edi)");
	asm("addl $4,%edi");
	
	/// Apply right volume 
	asm("movl 32(%ebp),%eax"); //save it
	asm("imul %ebx");
	asm("addl %eax,(%edi)");
	asm("addl $4,%edi");
	
	if (use_vramp) {
		
		asm("movl 28(%ebp),%eax");
		asm("addl %eax,36(%ebp)");
		asm("movl 32(%ebp),%eax");
		asm("addl %eax,40(%ebp)");
	}
	
	/* Increment.. */
	asm("movl 20(%ebp),%eax");
	asm("addl %eax,24(%ebp)");
	
	asm("popl %ecx");
	asm("loop 1b");
	
	asm("popl %edx");
	asm("popl %ecx");
	asm("popl %ebx");
	asm("popl %eax");
	asm("popl %edi");
	asm("popl %esi");

}

#else

int Mixer_Soft::cubic_lut[4*(1L<<SPLINE_FRACBITS)];

template<class Depth,bool is_stereo,bool use_filter,bool use_vramp,bool use_fx,InterpolationType type>
void Mixer_Soft::do_resample(

	
	const Depth* p_src,
	Sint32 *p_dst,int p_amount,
	const Sint32 &p_increment,
	Sint32 p_pos,
	Sint32 *p_lvol,
	Sint32 *p_rvol,
	Sint32 p_lvol_inc,
	Sint32 p_rvol_inc,
	//filter
	Sint32 &p_lp_c1,
	Sint32 &p_lp_c2,
	Sint32 &p_lp_c3,
	Sint32 p_lp_c1_inc,
	Sint32 p_lp_c2_inc,
	Sint32 p_lp_c3_inc,
	
	Sint64 *p_lp_h1,
	Sint64 *p_lp_h2,
	Sint32 *p_chorus_buff,
	Sint32 p_chorus_level,
	Uint32 p_chorus_pos,
	Sint32 *p_reverb_buff,
	Sint32 p_reverb_level,
	Uint32 p_reverb_pos
	
		       ) {

	int spline_fracshift=(MIXING_FRAC_BITS-SPLINE_FRACBITS)-2;
	int spline_fracmask=(((1L<<(MIXING_FRAC_BITS-spline_fracshift))-1)&~3);
	
	Sint32 final,final_r,next,next_r;
	while (p_amount--) {

		Sint32 pos=p_pos >> MIXING_FRAC_BITS;
		if (is_stereo)
			pos<<=1;
		
		final=p_src[pos];
		if (is_stereo)
			final_r=p_src[pos+1];
		
		if (sizeof(Depth)==1) { /* conditions will not exist anymore when compiled! */
			final<<=8;
			if (is_stereo)
				final_r<<=8;
		}
		
		if (type==INTERPOLATION_LINEAR) {

			if (is_stereo) {
				
				next=p_src[pos+2];
				next_r=p_src[pos+3];
			} else {
				next=p_src[pos+1];
			}		
			if (sizeof(Depth)==1) {
				next<<=8;
				if (is_stereo)
					next_r<<=8;
			}
			
			Sint32 frac=Sint32(p_pos&MIXING_FRAC_MASK);
			
			final=final+((next-final)*frac >> MIXING_FRAC_BITS);
			if (is_stereo)
				final_r=final_r+((next_r-final_r)*frac >> MIXING_FRAC_BITS);
		}
		
		if (type==INTERPOLATION_CUBIC) {

			int spfrac = (p_pos >> SPLINE_FRACSHIFT) & SPLINE_FRACMASK;
		
			if (!is_stereo) {
			
				Sint32 a=p_src[pos-1];
				Sint32 b=final;
				Sint32 c=p_src[pos+1];
				Sint32 d=p_src[pos+2];
				
				if (sizeof(Depth)==1) {
					a<<=8;
					c<<=8;
					d<<=8;
				}
				
				
				final = (cubic_lut[spfrac]*a +
					cubic_lut[spfrac+1]*b +
					cubic_lut[spfrac+2]*c +
					cubic_lut[spfrac+3]*d) >> SPLINE_16SHIFT;
			} else {
			
			
				Sint32 al=p_src[pos-2];
				Sint32 bl=final;
				Sint32 cl=p_src[pos+2];
				Sint32 dl=p_src[pos+4];
				
				Sint32 ar=p_src[pos-1];
				Sint32 br=final_r;
				Sint32 cr=p_src[pos+3];
				Sint32 dr=p_src[pos+5];
				
				if (sizeof(Depth)==1) {
					al<<=8;
					cl<<=8;
					dl<<=8;
					ar<<=8;
					cr<<=8;
					dr<<=8;
				}
				
				Sint32 sp0=cubic_lut[spfrac];
				Sint32 sp1=cubic_lut[spfrac+1];
				Sint32 sp2=cubic_lut[spfrac+2];
				Sint32 sp3=cubic_lut[spfrac+3];
				
				final = (sp0*al +
					sp1*bl +
					sp2*cl +
					sp3*dl) >> SPLINE_16SHIFT;
			
				final_r = (sp0*ar +
					sp1*br +
					sp2*cr +
					sp3*dr) >> SPLINE_16SHIFT;
			
			}
		}

		if (use_filter) {
			
			// well, convert to less resolution for filters.. can't do much, sorry!
			//final>>=4;
			final = (Sint32)(((Sint64)final * ((Sint64)p_lp_c1>>10) + *p_lp_h1 * ((Sint64)p_lp_c2>>10) + *p_lp_h2 * ((Sint64)p_lp_c3>>10)) >> FILTER_SHIFT);
			
			*p_lp_h2=*p_lp_h1;
			*p_lp_h1=final;  	//final<<=4;
		
			
			if (is_stereo) {
				
				final_r = (Sint32)(((Sint64)final_r * ((Sint64)p_lp_c1>>10) + p_lp_h1[1] * ((Sint64)p_lp_c2>>10) + p_lp_h2[1] * ((Sint64)p_lp_c3>>10)) >> FILTER_SHIFT);
				p_lp_h2[1]=p_lp_h1[1];
				p_lp_h1[1]=final_r;
				
			}
			
			
			p_lp_c1+=p_lp_c1_inc;
			p_lp_c2+=p_lp_c2_inc;
			p_lp_c3+=p_lp_c3_inc;
		}
		
		if (use_vramp) {
			
			
			if (is_stereo) {
				
				final=final*(*p_lvol>>VOL_RAMP_BITS);
				final_r=final_r*(*p_rvol>>VOL_RAMP_BITS);
				
			} else {
				final_r=final*(*p_rvol>>VOL_RAMP_BITS);
				final=final*(*p_lvol>>VOL_RAMP_BITS);
			}		
		} else {
			
			if (is_stereo) {
				
				final=final*(*p_lvol);
				final_r=final_r*(*p_rvol);
				
			} else {
				final_r=final*(*p_rvol);
				final=final*(*p_lvol);
			}		
			
		}
		
		final>>=(8+VOL_SHIFT);
		final_r>>=(8+VOL_SHIFT);
		
		if (use_fx) {
		
			p_chorus_buff[(p_chorus_pos++)&(CHORUS_RINGBUFF_SIZE*2-1)]+=(final*p_chorus_level)>>8;;
			p_chorus_buff[(p_chorus_pos++)&(CHORUS_RINGBUFF_SIZE*2-1)]+=(final_r*p_chorus_level)>>8;;
			p_reverb_buff[p_reverb_pos++]+=(final*p_reverb_level)>>8;
			p_reverb_buff[p_reverb_pos++]+=(final_r*p_reverb_level)>>8;
		}
		
		
		*p_dst++ +=final;
		*p_dst++ +=final_r;
		
		p_pos+=p_increment;
				
		
		if (use_vramp) {
			(*p_lvol)+=p_lvol_inc;
			(*p_rvol)+=p_rvol_inc;
		}
	}
	
}

void Mixer_Soft::init_cubic_lut() {


	int length		= (1L<<SPLINE_FRACBITS);
	float length_f	= 1.0f / (float)length;
	float scale	= (float)SPLINE_QUANTSCALE;
	for(int i=0;i<length;i++)
	{	float cm1, c0, c1, c2;
		float x		= ((float)i)*length_f;
		int sum,idx	= i<<2;
		cm1			= (float)floor( 0.5 + scale * (-0.5*x*x*x + 1.0 * x*x - 0.5 * x       ) );
		c0			= (float)floor( 0.5 + scale * ( 1.5*x*x*x - 2.5 * x*x             + 1.0 ) );
		c1			= (float)floor( 0.5 + scale * (-1.5*x*x*x + 2.0 * x*x + 0.5 * x       ) );
		c2			= (float)floor( 0.5 + scale * ( 0.5*x*x*x - 0.5 * x*x                   ) );
		cubic_lut[idx+0]	= (signed short)( (cm1 < -scale) ? -scale : ((cm1 > scale) ? scale : cm1) );
		cubic_lut[idx+1]	= (signed short)( (c0  < -scale) ? -scale : ((c0  > scale) ? scale : c0 ) );
		cubic_lut[idx+2]	= (signed short)( (c1  < -scale) ? -scale : ((c1  > scale) ? scale : c1 ) );
		cubic_lut[idx+3]	= (signed short)( (c2  < -scale) ? -scale : ((c2  > scale) ? scale : c2 ) );
#ifdef SPLINE_CLAMPFORUNITY
		sum			= cubic_lut[idx+0]+cubic_lut[idx+1]+cubic_lut[idx+2]+cubic_lut[idx+3];
		if( sum != SPLINE_QUANTSCALE )
		{	int max = idx;
			if( cubic_lut[idx+1]>cubic_lut[max] ) max = idx+1;
			if( cubic_lut[idx+2]>cubic_lut[max] ) max = idx+2;
			if( cubic_lut[idx+3]>cubic_lut[max] ) max = idx+3;
			cubic_lut[max] += (SPLINE_QUANTSCALE-sum);
		}
#endif
	}

}

#endif

void Mixer_Soft::process_voice(int p_voice_index,Sint32 p_frames,Sint32 *p_auxbuff) {


	Voice &v=voice_array[p_voice_index];


	/* MUST OPTIMIZE THIS!! */
	Sample_Loop_Type loop_type=sample_manager->get_loop_type(v.sample.sample);
	Sint64 sample_size_fp=(Sint64)sample_manager->get_size(v.sample.sample)<<MIXING_FRAC_BITS;
	Sint64 loop_begin_fp=(Sint64)sample_manager->get_loop_begin(v.sample.sample)<<MIXING_FRAC_BITS;
	Sint64 loop_end_fp=(Sint64)sample_manager->get_loop_end(v.sample.sample)<<MIXING_FRAC_BITS;

	bool mix_16=sample_manager->is_16bits(v.sample.sample);
	bool mix_stereo=sample_manager->is_stereo(v.sample.sample);

	Uint32 chorus_pos=chorus.ringbuffer_pos;
	Uint32 reverb_pos=0;

	bool loop_active=(loop_type!=LOOP_NONE) &&
			 (loop_end_fp<=sample_size_fp) &&
			(loop_begin_fp>=0) && (loop_begin_fp<loop_end_fp);


	//Sint64 limit,done;

	int total=p_frames;
	int todo=total;
	if (total<=0) return;

	Sample_ID samp_id=v.sample.sample;
	
	if (sample_manager->lock_data(samp_id))
		return; //very weird, why did it fail to lock the data?
 		/* lock the sample, get the mem */
	
	void *sample_data=sample_manager->get_data(samp_id);
	
	//	ERR_FAIL_COND(sample_data==0);
	        

	Sint64 begin_limit=loop_active?loop_begin_fp:0;
	Sint64 end_limit=loop_active?loop_end_fp:sample_size_fp;

	Sint32 vol_incr_l=(v.realtime.vol_l-v.realtime.old_vol_l)*(1<<VOL_RAMP_BITS)/p_frames;;
	Sint32 vol_incr_r=(v.realtime.vol_r-v.realtime.old_vol_r)*(1<<VOL_RAMP_BITS)/p_frames;;

	bool use_ramp=(vol_incr_l!=0) && (vol_incr_r!=0) && data.ramp_volume;
	if (data.ramp_volume && v.realtime.first_sample_rampup && data.declicker_enabled)
		use_ramp=true;
	bool use_effects_here=data.use_effects && (v.fx.reverb_send || v.fx.chorus_send);
	if (p_auxbuff)
		use_effects_here=false; //used for declicker, dont bother with it
	
	Sint32 vol_l=use_ramp?v.realtime.old_vol_l<<VOL_RAMP_BITS:v.realtime.old_vol_l;
	Sint32 vol_r=use_ramp?v.realtime.old_vol_r<<VOL_RAMP_BITS:v.realtime.old_vol_r;
	
	
	
	bool use_resampler=true;
	
	if (v.realtime.old_vol_l==0 && v.realtime.old_vol_r==0 && v.realtime.vol_l==0 && v.realtime.vol_r==0)
		use_resampler=false;
		
	Sint32 *mixto_buff=p_auxbuff?p_auxbuff:(&data.mixdown_buffer[data.mixdown_pos]);
	
	
	Sint32 filter_c1,filter_c2,filter_c3;
	Sint32 filter_c1_inc,filter_c2_inc,filter_c3_inc;
	
	if (data.filters && v.fx.filter.enabled) {
	
		if (!v.fx.filter.old_enabled) {
		
			v.realtime.filter.h1[0]=0;
			v.realtime.filter.h1[1]=0;
			v.realtime.filter.h2[0]=0;
			v.realtime.filter.h2[1]=0;
			filter_c1=v.fx.filter.c1<<10;
			filter_c2=v.fx.filter.c2<<10;
			filter_c3=v.fx.filter.c3<<10;
			filter_c1_inc=0;
			filter_c2_inc=0;
			filter_c3_inc=0;
				
		} else {
		
		 	if (p_frames<16) { /* safety check for very quick rams */
			
				filter_c1=v.fx.filter.c1<<10;
				filter_c2=v.fx.filter.c2<<10;
				filter_c3=v.fx.filter.c3<<10;
				filter_c1_inc=0;
				filter_c2_inc=0;
				filter_c3_inc=0;
			} else {
		
				filter_c1=v.fx.filter.old_c1;
				filter_c2=v.fx.filter.old_c2;
				filter_c3=v.fx.filter.old_c3;
				filter_c1_inc=((v.fx.filter.c1-v.fx.filter.old_c1)<<10)/p_frames;
				filter_c2_inc=((v.fx.filter.c2-v.fx.filter.old_c2)<<10)/p_frames;
				filter_c3_inc=((v.fx.filter.c3-v.fx.filter.old_c3)<<10)/p_frames; 
				filter_c1<<=10;
				filter_c2<<=10;
				filter_c3<<=10;
			}

		}
		
		v.fx.filter.old_enabled=true;
		
		
		v.fx.filter.old_c1=v.fx.filter.c1;
		v.fx.filter.old_c2=v.fx.filter.c2;
		v.fx.filter.old_c3=v.fx.filter.c3;
		
	} else {
	
		v.fx.filter.old_enabled=false;
	}
	
	
	
	while(todo>0) {


		if ( v.realtime.backwards_play ) {
			/* The sample is playing in reverse */
			if( ( loop_active )&&(v.realtime.current_index_fp<loop_begin_fp) ) {
				/* the sample is looping and has reached the loopstart index */
				if ( loop_type==LOOP_BIDI ) {
					/* sample is doing bidirectional loops, so 'bounce' the
					current index against the loop_begin_fp */
					v.realtime.current_index_fp = loop_begin_fp+(loop_begin_fp-v.realtime.current_index_fp);
					v.realtime.backwards_play=false;
					v.realtime.increment_index_fp = -v.realtime.increment_index_fp;
				} else
					/* normal backwards looping, so set the current position to
					loopend index */
					v.realtime.current_index_fp=loop_end_fp-(loop_begin_fp-v.realtime.current_index_fp);
			} else {
				/* the sample is not looping, so check if it reached index 0 */
				if(v.realtime.current_index_fp < 0) {
					/* playing index reached 0, so stop playing this sample */
					v.realtime.current_index_fp=0;
					v.active=false;
//					printf("end voice %i (bw)\n",p_voice_index);
					//printf("reached zero backward\n");

					break;
				}
			}
		} else {
			/* The sample is playing forward */
			if ( (loop_active) && (v.realtime.current_index_fp >= loop_end_fp)) {
				/* the sample is looping, check the loopend index */
				if( loop_type==LOOP_BIDI ) {
					/* sample is doing bidirectional loops, so 'bounce' the
					current index against the loop_end_fp */
					v.realtime.backwards_play=true;
					v.realtime.increment_index_fp = -v.realtime.increment_index_fp;
					v.realtime.current_index_fp = loop_end_fp-(v.realtime.current_index_fp-loop_end_fp);
				} else
					/* normal backwards looping, so set the current position
					to loopend index */
					v.realtime.current_index_fp=loop_begin_fp+(v.realtime.current_index_fp-loop_end_fp);
			} else {
				/* sample is not looping, so check if it reached the last
				position */
				if(v.realtime.current_index_fp >= sample_size_fp) {
					/* yes, so stop playing this sample */
					v.realtime.current_index_fp=0;
					v.active=false;
//					printf("end voice %i (fw)\n",p_voice_index);
//					printf("reached zero forward idx %i, siz %i\n",v.realtime.current_index_fp,sample_size_fp);

					break;
				}
			}
		}

		
		Sint64 limit=v.realtime.backwards_play?begin_limit:end_limit;

		Sint64 done=MIN((limit-v.realtime.current_index_fp)/v.realtime.increment_index_fp+1,todo);


		
		if ( done<=0 ) {

			v.active = 0;
			break;
		}

		
		
//		Sint64 endpos=v.realtime.current_index_fp+done*v.realtime.increment_index_fp;

		if (use_resampler) { /* MIX TIME! */

			Uint32 beginpos=v.realtime.current_index_fp>>MIXING_FRAC_BITS;
			Sint32 beginbits=v.realtime.current_index_fp&MIXING_FRAC_MASK;
			int mixdownpos=(total-todo)*2;
			
			
			if (!use_ramp) { //NO RAMP
#define NORAMP_MIX(m_depth,m_stereo,m_use_filter,m_use_fx,m_interp) \
do_resample<m_depth,m_stereo,m_use_filter,false,m_use_fx,m_interp>( \
		&((m_depth*)sample_data)[m_stereo?(beginpos<<1):beginpos], \
		&mixto_buff[mixdownpos], \
		done, \
		v.realtime.increment_index_fp, \
		beginbits, \
		&vol_l, \
		&vol_r, \
		0, \
		0,\
		filter_c1,\
		filter_c2,\
		filter_c3,\
		filter_c1_inc,\
		filter_c2_inc,\
		filter_c3_inc,\
		v.realtime.filter.h1,\
		v.realtime.filter.h2, \
		chorus.ringbuffer,\
		v.fx.chorus_send,\
		chorus_pos<<1,\
		data.reverb_buffer,\
		v.fx.reverb_send,\
		reverb_pos<<1); \
	
				if (use_effects_here) {
					if (data.interpolation_type==INTERPOLATION_LINEAR) {
				
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,true,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,true,INTERPOLATION_LINEAR);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,true,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,true,INTERPOLATION_LINEAR);
								}
								
							} 
						} else {
							
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,true,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,true,INTERPOLATION_LINEAR);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,true,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,true,INTERPOLATION_LINEAR);
								}
								
							} 
						}					
							
						
					} else if (data.interpolation_type==INTERPOLATION_CUBIC) {
				
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,true,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,true,INTERPOLATION_CUBIC);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,true,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,true,INTERPOLATION_CUBIC);
								}
								
							} 
						} else {
							
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,true,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,true,INTERPOLATION_CUBIC);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,true,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,true,INTERPOLATION_CUBIC);
								}
								
							} 
						}					
							
						
					} else { //interp
						
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,true,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,true,INTERPOLATION_RAW);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,true,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,true,INTERPOLATION_RAW);
								}
								
							}
						} else {
											
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,true,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,true,INTERPOLATION_RAW);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,true,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,true,INTERPOLATION_RAW);
								}
								
							}
											
						}
						
					}
				}  else { // FX
				
				
					if (data.interpolation_type==INTERPOLATION_LINEAR) {
				
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,false,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,false,INTERPOLATION_LINEAR);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,false,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,false,INTERPOLATION_LINEAR);
								}
								
							} 
						} else {
							
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,false,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,false,INTERPOLATION_LINEAR);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,false,INTERPOLATION_LINEAR);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,false,INTERPOLATION_LINEAR);
								}
								
							} 
						}					
							
					} else if (data.interpolation_type==INTERPOLATION_CUBIC) {
				
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,false,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,false,INTERPOLATION_CUBIC);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,false,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,false,INTERPOLATION_CUBIC);
								}
								
							} 
						} else {
							
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,false,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,false,INTERPOLATION_CUBIC);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,false,INTERPOLATION_CUBIC);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,false,INTERPOLATION_CUBIC);
								}
								
							} 
						}					
							
						
					} else { //interp
						
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,true,false,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,true,true,false,INTERPOLATION_RAW);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,true,false,false,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,true,false,false,INTERPOLATION_RAW);
								}
								
							}
						} else {
											
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,true,false,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,false,true,false,INTERPOLATION_RAW);
								}
							} else { //filter
								
								if (mix_16) {
						
									NORAMP_MIX(Sint16,false,false,false,INTERPOLATION_RAW);
					
								} else {
					
									NORAMP_MIX(Sint8,false,false,false,INTERPOLATION_RAW);
								}
								
							}
											
						}
						
					}				
				} //end effects
			} else { // RAMP
				
				Sint32 save_incr_l;
				Sint32 save_incr_r;
				
				/* Part of the Declicker */
				if (v.realtime.first_sample_rampup && data.declicker_enabled) {
			
								
					save_incr_r=vol_incr_r;
					save_incr_l=vol_incr_l;

					
					if (done>FIRST_SAMPLE_RAMP_LEN)
						done=FIRST_SAMPLE_RAMP_LEN;
					
					Sint32 target_l=vol_l+vol_incr_l*done;
					Sint32 target_r=vol_r+vol_incr_r*done;
					
					vol_l=0;
					vol_r=0;
					
					vol_incr_l=target_l/done;
					vol_incr_r=target_r/done;

					
				} 
				
			
				
#define RAMP_MIX(m_depth,m_stereo,m_use_filter,m_use_fx,m_interp) \
do_resample<m_depth,m_stereo,m_use_filter,true,m_use_fx,m_interp>( \
		&((m_depth*)sample_data)[m_stereo?(beginpos<<1):beginpos], \
		&mixto_buff[mixdownpos], \
		done, \
		v.realtime.increment_index_fp, \
		beginbits, \
		&vol_l, \
		&vol_r, \
		vol_incr_l, \
		vol_incr_r,\
		filter_c1,\
		filter_c2,\
		filter_c3,\
		filter_c1_inc,\
		filter_c2_inc,\
		filter_c3_inc,\
		v.realtime.filter.h1,\
		v.realtime.filter.h2,\
		chorus.ringbuffer,\
		v.fx.chorus_send,\
		chorus_pos<<1,\
		data.reverb_buffer,\
		v.fx.reverb_send,\
		reverb_pos<<1); \
				
				
				if (use_effects_here) {
				
					if (data.interpolation_type==INTERPOLATION_LINEAR) {
			
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,true,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,true,INTERPOLATION_LINEAR);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,true,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,true,INTERPOLATION_LINEAR);
								}
							
							} 
						} else {
						
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,true,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,true,INTERPOLATION_LINEAR);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,true,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,true,INTERPOLATION_LINEAR);
								}
							
							} 
						}					
						
					
					
					} else if (data.interpolation_type==INTERPOLATION_CUBIC) {
			
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,true,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,true,INTERPOLATION_CUBIC);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,true,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,true,INTERPOLATION_CUBIC);
								}
							
							} 
						} else {
						
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,true,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,true,INTERPOLATION_CUBIC);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,true,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,true,INTERPOLATION_CUBIC);
								}
							
							} 
						}					
						
					
										
					} else { //interp
					
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,true,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,true,INTERPOLATION_RAW);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,true,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,true,INTERPOLATION_RAW);
								}
							
							}
						} else {
										
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,true,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,true,INTERPOLATION_RAW);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,true,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,true,INTERPOLATION_RAW);
								}
							
							}
										
						}
					
					}
				} else { // FX
				
				
					if (data.interpolation_type==INTERPOLATION_LINEAR) {
			
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,false,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,false,INTERPOLATION_LINEAR);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,false,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,false,INTERPOLATION_LINEAR);
								}
							
							} 
						} else {
						
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,false,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,false,INTERPOLATION_LINEAR);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,false,INTERPOLATION_LINEAR);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,false,INTERPOLATION_LINEAR);
								}
							
							} 
						}					
						
					
					} else if (data.interpolation_type==INTERPOLATION_CUBIC) {
			
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,false,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,false,INTERPOLATION_CUBIC);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,false,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,false,INTERPOLATION_CUBIC);
								}
							
							} 
						} else {
						
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,false,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,false,INTERPOLATION_CUBIC);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,false,INTERPOLATION_CUBIC);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,false,INTERPOLATION_CUBIC);
								}
							
							} 
						}					
						
					
					} else { //interp
					
						if (mix_stereo) {
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,true,false,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,true,true,false,INTERPOLATION_RAW);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,true,false,false,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,true,false,false,INTERPOLATION_RAW);
								}
							
							}
						} else {
										
							if (v.fx.filter.enabled && data.filters) {
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,true,false,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,false,true,false,INTERPOLATION_RAW);
								}
							} else { //filter
							
								if (mix_16) {
					
									RAMP_MIX(Sint16,false,false,false,INTERPOLATION_RAW);
				
								} else {
				
									RAMP_MIX(Sint8,false,false,false,INTERPOLATION_RAW);
								}
							
							}
										
						}
					
					}					
				}
				
				/* Part of the Declicker */
				if (v.realtime.first_sample_rampup && data.declicker_enabled) {
			
								
					vol_incr_r=save_incr_r;
					vol_incr_l=save_incr_l;
					v.realtime.first_sample_rampup=false;
					
				}
			}
		}	
			
		chorus_pos+=done;
		reverb_pos+=done;
		v.realtime.current_index_fp+=done*v.realtime.increment_index_fp;
		todo-=done;
	}
	//printf("voicepos %i\n",(int)(v.realtime.current_index_fp >> MIXING_FRAC_BITS));
	
	sample_manager->unlock_data(samp_id);

	
}


