#ifndef	_SCHED_H
#define _SCHED_H
#define NR_TASKS 64
#define	FIRST_TASK	task[0]
#define	LAST_TASK	task[NR_TASKS-1]
#define FIRST_TSS_ENTRY   4
#define FIRST_LDT_ENTRY   (FIRST_TSS_ENTRY+1)

#define _TSS(n) ((((unsigned long)n)<<4)+(FIRST_TSS_ENTRY<<3))
#define _LDT(n) ((((unsigned long)n)<<4)+(FIRST_LDT_ENTRY<<3))

#define ltr(n)  __asm__("ltr %%ax"::"a"(_TSS(n)))
#define lldt(n)  __asm__("lldt %%ax"::"a"(_LDT(n)))

#include "linux/head.h"
#include "linux/mm.h"
#include "linux/fs.h"
extern void sched_init(void);
struct i387_struct{
    long cwd;
    long swd;
    long twd;
    long fip;
    long fcs;
    long foo;
    long fos;
    long st_space[20];
};

struct tss_struct {
	long	back_link;	/* 16 high bits zero */
	long	esp0;
	long	ss0;		/* 16 high bits zero */
	long	esp1;
	long	ss1;		/* 16 high bits zero */
	long	esp2;
	long	ss2;		/* 16 high bits zero */
	long	cr3;
	long	eip;
	long	eflags;
	long	eax,ecx,edx,ebx;
	long	esp;
	long	ebp;
	long	esi;
	long	edi;
	long	es;		/* 16 high bits zero */
	long	cs;		/* 16 high bits zero */
	long	ss;		/* 16 high bits zero */
	long	ds;		/* 16 high bits zero */
	long	fs;		/* 16 high bits zero */
	long	gs;		/* 16 high bits zero */
	long	ldt;		/* 16 high bits zero */
	long	trace_bitmap;	/* bits: trace 0, bitmap 16-31 */
	struct i387_struct i387;
};

struct task_struct {
	long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;
	long priority;

	struct desc_struct ldt[3];
/* tss for this task */
	struct tss_struct tss;
};

#define INIT_TASK \
/* state etc */	{ 0,15,15, \
	{		{0,0}, \
/* ldt */	{0x9f,0xc0fa00}, \
		{0x9f,0xc0f200} \
	}, \
/*tss*/	{0,PAGE_SIZE+(long)&init_task,0x10,0,0,0,0,(long)&pg_dir,\
	 0,0,0,0,0,0,0,0, \
	 0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 _LDT(0),0x80000000, \
	 {}\
	}, \
}

#endif
