//
// C++ Implementation: error_macros
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifdef ANSIC_LIBS_ENABLED
#include <stdio.h>

void err_fail_index_msg(const char *p_file,int p_line,int p_index,int p_size) {


	printf("***ERROR*** -  %s:%i INVALID INDEX %i/%i\n",p_file,p_line,p_index,p_size);
}
void err_fail_cond_msg(const char *p_file,int p_line,const char *p_cond) {
	
	printf("***ERROR*** %s:%i COND FAILED: %s\n",p_file,p_line,p_cond);
	
}
void err_continue_msg(const char *p_file,int p_line,const char *p_cond) {
	
	printf("***ERROR*** %s:%i CONTINUED - COND FAILED: %s\n",p_file,p_line,p_cond);
	
}
void err_print_msg(const char *p_file,int p_line,const char *p_msg) {
	
	printf("***ERROR*** %s:%i MESSAGE: %s\n",p_file,p_line,p_msg);
	
}
void wrn_print_msg(const char *p_file,int p_line,const char *p_msg) {
	
	printf("***WARN*** %s:%i MESSAGE: %s\n",p_file,p_line,p_msg);
	
}
void err_return_msg(const char *p_file,int p_line,const char *p_msg) {
	
	printf("***ERROR*** %s:%i FUNCTION - MESSAGE: %s\n",p_file,p_line,p_msg);
	
	
}

#endif
