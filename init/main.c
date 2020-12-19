#include "proto.h"
#include "linux/sched.h"
#include "asm/system.h"
#include "asm/io.h"
void main()
{
	clear();
	disp_str("A new system is running!");
	trap_init();
    	sched_init();
	sti();
   	move_to_user_mode();
    	disp_str("\nIn user mode!");
    	while(1){

	};
}
