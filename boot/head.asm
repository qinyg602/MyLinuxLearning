extern main
extern user_stack_top
extern disp_str
extern clear
SECTION .text
	global _start,disp_pos,idt,gdt,pg_dir
	BITS 32
pg_dir:
_start:
	mov	eax,010h
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	fs,ax
	mov	gs,ax

	mov	esp,[user_stack_top]

	call	setup_idt
	lgdt	[gdt_descr]
	jmp	dword 8:go		;reload cs descriptor because descriptor redefined
go:
	mov	eax,010h
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	fs,ax
	mov	eax,018h
	mov	gs,ax

	mov	esp,[user_stack_top]
	xor	eax,eax
.1:	inc	eax
	mov	[000000h],eax
	cmp	eax,[100000h]
	je	.1
	jmp after_page_tables

setup_idt:
	mov	edx,_IdtHandler
	mov	eax,00080000h
	mov	ax,dx
	mov	dx,8E00h
	mov	edi,idt
	mov	ecx,256
rp_sidt:
	mov	[edi],eax
	mov	[edi+4],edx
	add	edi,8
	dec	ecx
	jnz rp_sidt
	cli
	lidt [idt_descr]
	ret
;page tables
	times 0x1000-($-$$) db 0
pg0:
	times 0x1000 db 0
pg1:
	times 0x1000 db 0
pg2:
	times 0x1000 db 0
pg3:
	times 0x1000 db 0
tmp_floppy_area:
	times 1024 db 0

after_page_tables:
	xor	eax,eax
	xor	edi,edi
	mov	ecx,1024*5
	cld
	rep	stosd
	mov	dword [pg_dir],pg0+7
	mov	dword [pg_dir+4],pg1+7
	mov	dword [pg_dir+8],pg2+7
	mov	dword [pg_dir+12],pg3+7

	mov	edi,pg3+4092
	mov	eax,0x0fff007
	std
.2:
	stosd
	sub	eax,0x1000
	jae	.2
	xor	eax,eax
	mov	cr3,eax
	mov	eax,cr0
	or	eax,0x80000000
	mov	cr0,eax
	cli
	caLL main
	jmp $

_IdtHandler:

	iretd

disp_pos:
		dd	0
ALIGN	8
	dw 0
idt_descr:
	dw	256*8-1
	dd	idt
idt:
	times 256 dq 0
ALIGN	8
	dw  0
gdt_descr:
	dw	256*8-1
	dd	gdt
gdt:
	dw	0,0,0,0		; dummy

	dw	 0FFFh		; 16Mb - limit=2047 (2048*4096=8Mb)
	dw	 0000h		; base address=0
	dw	 9A00h		; code read/exec
	dw	 00C0h		; granularity=4096, 386

	dw	00FFFh		; 16Mb - limit=2047 (2048*4096=8Mb)
	dw	00000h		; base address=0
	dw	09200h		; data read/write
	dw	000C0h		; granularity=4096, 386

;	Video descriptor
	dw	 0FFFh		; 16Mb - limit=2047 (2048*4096=8Mb)
	dw	08000h		; base address=0
	dw	0F20Bh		; data read/write
	dw	 00C0h		; granularity=4096, 386

	times 252 dq 0
