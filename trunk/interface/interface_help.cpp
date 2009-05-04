//
// C++ Interface: interface_help
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "bundles/window_box.h"
#include "interface_help.h"

static const char* help_pattern = "\n\
Pattern Edit:\n\
-------------\n\
(global keys at the end)\n\
\n\
 Summary of Effects.\n\
\n\
  Volume Column effects.\n\
   Ax Fine volume slide up by x\n\
   Bx Fine volume slide down by x\n\
   Cx Volume slide up by x\n\
   Dx Volume slide down by x\n\
   Ex Pitch slide down by x\n\
   Fx Pitch slide up by x\n\
   Gx Slide to note with speed x\n\
   Hx Vibrato with depth x\n\
\n\
  General effects.\n\
   Axx Set song speed (hex)\n\
   Bxx Jump to Order (hex)\n\
   Cxx Break to row xx (hex) of next pattern\n\
   D0x Volume slide down by x\n\
   Dx0 Volume slide up by x\n\
   DFx Fine volume slide down by x\n\
   DxF Fine volume slide up by x\n\
   Exx Pitch slide down by xx\n\
   EFx Fine pitch slide down by x\n\
   EEx Extra fine pitch slide down by x\n\
   Fxx Pitch slide up by xx\n\
   FFx Fine pitch slide up by x\n\
   FEx Extra fine pitch slide up by x\n\
   Gxx Slide to note with speed xx\n\
   Hxy Vibrato with speed x, depth y\n\
   Ixy Tremor with ontime x and offtime y\n\
   Jxy Arpeggio with halftones x and y\n\
   Kxx Dual Command: H00 & Dxx\n\
   Lxx Dual Command: G00 & Dxx\n\
   Mxx Set channel volume to xx (0->40h)\n\
   N0x Channel volume slide down by x\n\
   Nx0 Channel volume slide up by x\n\
   NFx Fine channel volume slide down by x\n\
   NxF Fine channel volume slide up by x\n\
   Oxx Set sample offset to yxx00h, y set with SAy\n\
   P0x Panning slide to right by x\n\
   Px0 Panning slide to left by x\n\
   PFx Fine panning slide to right by x\n\
   PxF Fine panning slide to left by x\n\
   Qxy Retrigger note every y ticks with volume modifier x\n\
     Values for x:\n\
       0: No volume change         8: Not used\n\
       1: -1                       9: +1\n\
       2: -2                       A: +2\n\
       3: -4                       B: +4\n\
       4: -8                       C: +8\n\
       5: -16                      D: +16\n\
       6: *2/3                     E: *3/2\n\
       7: *1/2                     F: *2\n\
   Rxy Tremelo with speed x, depth y\n\
   S0x Unused\n\
   S1x Set chorus send level\n\
   S2x Unused\n\
   S3x Set vibrato waveform to type x\n\
   S4x Set tremelo waveform to type x\n\
   S5x Set panbrello waveform to type x\n\
     Waveforms for commands S3x, S4x and S5x:\n\
       0: Sine wave\n\
       1: Ramp down\n\
       2: Square wave\n\
       3: Random wave\n\
   S6x Pattern delay for x ticks\n\
   S70 Past note cut\n\
   S71 Past note off\n\
   S72 Past note fade\n\
   S73 Set NNA to note cut\n\
   S74 Set NNA to continue\n\
   S75 Set NNA to note off\n\
   S76 Set NNA to note fade\n\
   S77 Turn off volume envelope\n\
   S78 Turn on volume envelope\n\
   S79 Turn off panning envelope\n\
   S7A Turn on panning envelope\n\
   S7B Turn off pitch envelope\n\
   S7C Turn on pitch envelope\n\
   S8x Set panning position\n\
   S91 Set surround sound\n\
   SAy Set high value of sample offset yxx00h\n\
   SB0 Set loopback point\n\
   SBx Loop x times to loopback point\n\
   SCx Note cut after x ticks\n\
   SDx Note delay for x ticks\n\
   SEx Pattern delay for x rows\n\
   SFx Set parameterised MIDI Macro\n\
   T0x Tempo slide down by x\n\
   T1x Tempo slide up by x\n\
   Txx Set Tempo to xx (20h->0FFh)\n\
   Uxy Fine vibrato with speed x, depth y\n\
   Vxx Set global volume to xx (0->80h)\n\
   W0x Global volume slide down by x\n\
   Wx0 Global volume slide up by x\n\
   WFx Fine global volume slide down by x\n\
   WxF Fine global volume slide up by x\n\
   Xxx Set panning position (0->0FFh)\n\
   Yxy Panbrello with speed x, depth y\n\
   Zxx (0->7Fh) Frequency cutoff, (80h->8Fh) Q (Resonance)\n\
       (90h->FFh) Reverb send level.\n\
\n\
-=-=-=-=-=-=-=-=-=-=-=-=-=-<\n\
\n\
 Default Pattern Edit Keys.\n\
   Grey +,-         Next/Previous pattern  (*)\n\
   Ctrl +,-         Next/Previous order's pattern  (*)\n\
   0-9              Change octave/volume/instrument\n\
   0-9, A-F         Change effectvalue\n\
   A-Z              Change effect\n\
   . (Period)       Clear field(s)\n\
   1                Note cut (^^^)\n\
   `                Note off (---) / Panning Toggle\n\
   Spacebar         Use last note/instrument/volume/effect/effectvalue\n\
\n\
   Enter            Get default note/instrument/volume/effect\n\
   <                Decrease instrument\n\
   >                Increase instrument\n\
   Grey /,*         Decrease/Increase octave\n\
   , (Comma)        Toggle edit mask for current field\n\
\n\
   Ins/Del          Insert/Delete a row to/from current channel\n\
   Alt-Ins/Del      Insert/Delete an entire row to/from pattern  (*)\n\
\n\
   Up/Down          Move up/down by the skipvalue (set with Alt 0-9)\n\
   Ctrl-Home/End    Move up/down by 1 row\n\
   Alt-Up/Down      Slide pattern up/down by 1 row\n\
   Left/Right       Move cursor left/right\n\
   Alt-Left/Right   Move forwards/backwards one channel\n\
   Tab/Shift-Tab    Move forwards/backwards to note column\n\
   PgUp/PgDn        Move up/down n lines (n=Row Hilight Major)\n\
   Ctrl-PgUp/PgDn   Move to top/bottom of pattern\n\
   Home             Move to start of column/start of line/start of pattern\n\
   End              Move to end of column/end of line/end ofpattern\n\
\n\
   Alt-Backspace    Revert pattern data  (*)\n\
   Ctrl-Backspace   Undo - any function with  (*) can be undone\n\
\n\
   Ctrl-C           Toggle centralise cursor\n\
   Ctrl-H           Toggle current row hilight\n\
\n\
   Shift+F2               Set pattern length/highlight\n\
\n\
  Block Functions.\n\
   Alt-B            Mark beginning of block\n\
   Alt-E            Mark end of block\n\
   Alt-L            Mark entire column/pattern\n\
   Shift-Arrows     Mark block\n\
\n\
   Alt-U            Unmark block/Release clipboard memory\n\
\n\
   Alt-Q            Raise notes by a semitone  (*)\n\
   Alt-A            Lower notes by a semitone  (*)\n\
   Alt-Shift-Q      Raise notes by an octave  (*)\n\
   Alt-Shift-A      Lower notes by an octave  (*)\n\
   Alt-S            Set Instrument  (*)\n\
   Alt-V            Set volume/panning  (*)\n\
   Alt-W            Wipe vol/pan not associated with a note/instrument  (*)\n\
   Alt-K            Slide volume/panning column  (*)\n\
   Alt-J            Volume amplifier  (*) / Fast volume attenuate  (*)\n\
   Alt-Z            Cut block  (*)\n\
   Alt-X            Slide effect value  (*)\n\
 2*Alt-X            Wipe all effect data  (*)\n\
\n\
   Alt-C            Copy block into clipboard\n\
   Alt-P            Paste data from clipboard  (*)\n\
   Alt-O            Overwrite with data from clipboard   (*)\n\
   Alt-M            Mix each row from clipboard with pattern data  (*)\n\
 2*Alt-M            Mix each field from clipboard with pattern data\n\
\n\
   Alt-F            Double block length  (*)\n\
   Alt-G            Halve block length  (*)\n\
   Alt-T            Time Scale Block (Linear)\n\
   Alt-Shift-T      Time Scale Block\n\
\n\
 Playback Functions.\n\
   4                Play note under cursor\n\
   8                Play row\n\
\n\
   F9           Toggle current channel (Mute/Unmute)\n\
   F10          Solo current channel\n\
\n\
   Ctrl-F      Toggle playback tracing\n\
\n\
-=-=-=-=-=-=-=-=-=-=-=-=-=-<\n\
\n\
\n\
 Global Keys.\n\
\n\
   F2                Pattern Editor / Pattern Editor Options\n\
   F3                Sample List\n\
   F4                Instrument List\n\
   F5                Play Song\n\
   F6                Play current pattern\n\
   Shift-F6          Play song from current Order\n\
   F7                Play from mark / current row\n\
   F8                Stop Playback\n\
   F11               Order List (in Pattern View)\n\
   F12               Song Variables & Message\n\
\n\
\n\
";

static const char* help_skins = "\n\
Installing Skins:\n\
-----------------\n\
\n\
Windows NT/2000/XP:\n\
\n\
Skins must be installed into\n\
the equivalent of:\n\
C:\\Documments And Settings\\Your User\\Application Data\\Chibi\\Skins\\\n\
\n\
\n\
Windows 95/98/ME:\n\
\n\
Skins must be installed into\n\
the same directory chibitracker.exe is installed,\n\
example:\n\
C:\\Chibitracker\\Skins\\\n\
\n\
\n\
Unix:\n\
\n\
Skins must be installed into:\n\
~/.chibi/skins/\n\
";
		


InterfaceHelp::InterfaceHelp(Window *p_parent,HelpType p_type) : Window(p_parent,MODE_POPUP,SIZE_TOPLEVEL_CENTER) {
	
	WindowBox *vbc = new WindowBox("Help!");
	set_root_frame(vbc);
	HBoxContainer *hbc = vbc->add(new HBoxContainer,1);
	TextEdit *te = hbc->add( new TextEdit,1 );
	switch(p_type) {
		case HELP_PATTERN_KEYS: te->set_text( help_pattern ); break;
		case HELP_SKINS: te->set_text( help_skins ); break;
	}
	
	VScrollBar *vsb = hbc->add( new VScrollBar );
	vsb->set_range( te->get_range() );
	vsb->set_auto_hide(true);
	vsb->hide();	
	set_size(Size(50,50));	
	te->set_row(0);	
	vsb->get_range()->set(0);
	
}
