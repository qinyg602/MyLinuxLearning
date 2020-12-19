#include "linux/sched.h"
#include "linux/mm.h"
#include "asm/system.h"
#include "asm/io.h"

union task_union{
    struct task_struct task;
	char	stack[PAGE_SIZE];
};
static union task_union init_task={INIT_TASK,};
struct task_struct* tasks[NR_TASKS]={&init_task.task,};
long user_stack[PAGE_SIZE>>2];
long* user_stack_top=(long*)(&user_stack[PAGE_SIZE>>2]);
void sched_init()
{
    struct desc_struct* p;
    set_tss_desc((char*)(gdt+FIRST_TSS_ENTRY),&init_task.task.tss);
    set_ldt_desc((char*)(gdt+FIRST_LDT_ENTRY),&init_task.task.ldt);
    p=(struct desc_struct*)(gdt+2+FIRST_TSS_ENTRY);
    for(int i=1;i<NR_TASKS;i++,p++){
        tasks[i]=NULL;
        p->low=p->high=0;
        p++;
        p->low=p->high=0;
        p++;
    }
    __asm__("pushfl;andl $0xffffbfff,(%esp);popfl");
    ltr(0);
    lldt(0);

	outb(inb_p(0x21)&~0x01,0x21);

}




