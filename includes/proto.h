
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* klib.asm */
#ifndef __proto__
#define __proto__
 #define TRUE 1
 #define FALSE 0
void	disp_str(char * info);
void	disp_color_str(char * info, int color);
void	clear();
char * itoa(char * str, int num);
#endif
