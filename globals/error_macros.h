#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

void err_fail_index_msg(const char *p_file,int p_line,int p_index,int p_size);
void err_fail_cond_msg(const char *p_file,int p_line,const char *p_cond);
void err_continue_msg(const char *p_file,int p_line,const char *p_cond);
void err_print_msg(const char *p_file,int p_line,const char *p_msg);
void wrn_print_msg(const char *p_file,int p_line,const char *p_msg);
void err_return_msg(const char *p_file,int p_line,const char *p_msg);


#define ERR_FAIL_INDEX(m_index,m_size) \
	 {if ((m_index)<0 || (m_index)>=(m_size)) { \
	 	err_fail_index_msg(__FILE__,__LINE__,m_index,m_size); \
		return;	\
	}}	\

#define ERR_FAIL_INDEX_V(m_index,m_size,m_retval) \
	 {if ((m_index)<0 || (m_index)>=(m_size)) { \
	 	err_fail_index_msg(__FILE__,__LINE__,m_index,m_size); \
		return (m_retval);	\
	}}	\

#define ERR_FAIL_COND(m_cond) \
	{ if ( m_cond ) {	\
		err_fail_cond_msg(__FILE__,__LINE__,#m_cond);\
		return;	 \
	} }	\

#define ERR_FAIL_COND_V(m_cond,m_retval) \
	{ if ( m_cond ) {	\
		err_fail_cond_msg(__FILE__,__LINE__,#m_cond);\
		return m_retval;	 \
	} }	\

#define ERR_CONTINUE(m_cond) \
	{ if ( m_cond ) {	\
		err_continue_msg(__FILE__,__LINE__,#m_cond);\
		continue;	 \
	} }	\

#define ERR_PRINT(m_string) \
	{ \
		err_print_msg(__FILE__,__LINE__,#m_string);\
} \

#define WARN_PRINT(m_string) \
	{ \
		wrn_print_msg(__FILE__,__LINE__,#m_string);\
} \

#define ERR_RETURN(m_string) \
{ \
	err_return_msg(__FILE__,__LINE__,#m_string);\
		return;	\
} \


#endif

