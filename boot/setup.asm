BOOTSEG		equ	0x07c0
INITSEG		equ	0x9000
SETUPSEG	equ	0x9020

	mov	ax,INITSEG
	mov	ds,ax
;show loading message
	mov	ax,SETUPSEG
	mov	es,ax
	mov	ax,1301h
	mov	bx,000ch
	mov	cx,19
	mov	bp,msg2
	int	10h
;get cursor position.

	xor	bh,bh
	mov	ah,03h
	int	10h	
	mov	[0],dx
;get extended memory(kb)

	mov	ah,88h
	int	15h
	mov	[2],ax
;Get video-card data:

	mov	ah,0fh
	int	10h
	mov	[4],bx		; bh = display page
	mov	[6],ax		; al = video mode, ah = window width

;check for EGA/VGA and some config parameters

	mov	ah,0x12
	mov	bl,0x10
	int	0x10
	mov	[8],ax
	mov	[10],bx
	mov	[12],cx
;get hd0 data
	mov	ax,0000h
	mov	ds,ax
	mov	ax,INITSEG
	mov	si,[4*41h]
	mov	es,ax
	mov	di,0080h
	mov	cx,10h
	rep 
	movsb
;get hd1 data
	mov	ax,0000h
	mov	ds,ax
	mov	si,[4*46h]
	mov 	ax,INITSEG
	mov	es,ax
	mov	di,0090h
	mov	cx,10h
	rep
	movsb
;check if hd1 exists:
	mov	ax,01500h
	mov	dl,81h
	int	13h
	jc	no_disk1
	cmp	ah,03h
	je	is_disk1
no_disk1:
	mov	ax,INITSEG
	mov	es,ax
	mov	di,0090h
	mov	al,00h
	mov	cx,10h
	rep 
	movsb
is_disk1:
	
	cli
	mov	ax,0
	cld
rp_move:
	mov	es,ax
	add	ax,1000h
	cmp	ax,09000h
	jz	end_move
	mov	ds,ax
	xor	si,si
	xor	di,di
	mov	cx,08000h
	rep
	movsw
	jmp	rp_move
end_move:	
	mov	ax,SETUPSEG
	mov	ds,ax

;load idt and gdt
	lidt	[idt_48]
	lgdt	[gdt_48]
;enable	A20
	in	al,92h
	or	al,00000010b
	out	92h,al
;Initialize 8259A
	call	init_8259A
;	move into protected mode
	mov	eax,cr0
	or	eax,1
	mov	cr0,eax
	jmp dword	8:0
	
init_8259A:
	mov	al,011h				;ICW1 to main 8259A
	out	020h,al
	call	io_delay
	
	out	0A0h,al				;ICW1 to slave 9259A
	call	io_delay	
	
	mov	al,020h				; start of hardware(IR0)
	out	021h,al
	call	io_delay
	
	mov	al,028h				;start of hardware(IR8)
	out	0A1h,al
	call	io_delay
	
	mov	al,04h				;ICW3 to main 8259A
	out	021h,al
	call	io_delay
	
	mov	al,02h				;ICW3 to slave 8259A
	out	0A1h,al
	call	io_delay
	
	mov	al,01h				;ICW4
	out	021h,al
	call	io_delay
	
	out	0A1h,al				;ICW4
	call	io_delay
	
	mov	al,0ffh				;mask off all	IRs
	out	021h,al
	call	io_delay
	
	mov	al,0ffh
	out	0A1h,al
	call	io_delay
	
	ret
	
io_delay:
	nop
	nop
	nop
	nop
	ret
	
msg2:
	db 13,10,"Loading setup ..."
gdt:
	dw	0,0,0,0		; dummy

	dw	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	dw	0x0000		; base address=0
	dw	0x9A00		; code read/exec
	dw	0x00C0		; granularity=4096, 386

	dw	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	dw	0x0000		; base address=0
	dw	0x9200		; data read/write
	dw	0x00C0		; granularity=4096, 386

idt_48:
	dw	0			; idt limit=0
	dw	0,0			; idt base=0L

gdt_48:
	dw	0x800-1		; gdt limit=2048, 256 GDT entries
	dw	512+gdt,0x9	; gdt base = 0X9xxxx
	


