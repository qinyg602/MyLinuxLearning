#include "proto.h"
#include "asm/system.h"
#include "linux/head.h"
#include "asm/io.h"

void divide_error();
void debug();
void nmi();
void int3();
void overflow();
void bounds();
void invalid_op();
void device_not_available();
void double_fault();
void coprocessor_segment_overrun();
void invalid_TSS();
void segment_not_present();
void stack_segment();
void general_protection();
void page_fault();
void coprocessor_error();
void reserved();
void clock();
void keyboard();
//void parallel_interrupt()
//void irq13();

void die(char* str)
{
	disp_str(str);
}

void do_divide_error()
{
	die("\ndivided error!");
}

void do_debug()
{
	die("\ndebug!");
}

void do_nmi(){
	die("\nnmi!");
}

void do_int3()
{
	die("\nint3!");
}

void do_overflow(){
	die("\noverflow!");
}

void do_bounds()
{
	die("\nbounce!");
}

void do_invalid_op()
{
	die("\ninvalid_OP!");
}

void do_device_not_available()
{
	die("\ndevice_not_available!");
}

void do_double_fault()
{
	die("\ndouble_fault!");
}

void do_coprocessor_segment_overrun()
{
	die("\ncoprocessor_segment_overrun!");
}

void do_invalid_TSS()
{
	die("\ninvalid_TSS!");
}

void do_segment_not_present()
{
	die("\nsegment_not_present!");
}

void do_stack_segment()
{
	die("\nstack_segment!");
}

void do_general_protection()
{
	die("\ngeneral_protection!");
}

void do_page_fault()
{
	die("\npage_fault!");
}

void do_coprocessor_error()
{
	die("\ncoprocessor_error!");
}

/*void do_parallevel_interrupt(){
	die("\nparallevel_interrupt!");
}

void do_irq13()
{
	die("\nirq13!");
}*/

void do_reserved()
{
	die("\nReserved!");
}
void do_clock()
{
    die("\nclock!");
}
void do_keyboard()
{
    die("\nkeyboard!");
}

void trap_init()
{
    int i;

    // 设置除操作出错的中断向量值。
	set_trap_gate(0,divide_error);
	set_trap_gate(1,debug);
	set_trap_gate(2,nmi);
	set_system_gate(3,int3);	/* int3-5 can be called from all */
	set_system_gate(4,overflow);
	set_system_gate(5,bounds);
	set_trap_gate(6,invalid_op);
	set_trap_gate(7,device_not_available);
	set_trap_gate(8,double_fault);
	set_trap_gate(9,coprocessor_segment_overrun);
	set_trap_gate(10,invalid_TSS);
	set_trap_gate(11,segment_not_present);
	set_trap_gate(12,stack_segment);
	set_trap_gate(13,general_protection);
	set_trap_gate(14,page_fault);
	set_trap_gate(15,reserved);
	set_trap_gate(16,coprocessor_error);
    // 下面把int17-47的陷阱门先均设置为reserved,以后各硬件初始化时会重新设置自己的陷阱门。
	for (i=17;i<48;i++)
		set_trap_gate(i,reserved);
    set_intr_gate(32,clock);
    set_intr_gate(33,keyboard);
	outb_p(inb_p(0x21)&~0x02,0x21);
}
