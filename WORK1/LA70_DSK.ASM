	.286p
	ifndef	??version
?debug	macro
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	S "LA70_DSK.C"
	?debug	C E92D519D260A4C4137305F44534B2E43
	?debug	C E92008021714433A5C54435C494E434C5544455C4D4154482E48
	?debug	C E92008021713433A5C54435C494E434C5544455C444F532E48
	?debug	C E92008021712433A5C54435C494E434C5544455C494F2E48
	?debug	C E92008021716433A5C54435C494E434C5544455C5354444152472E+
	?debug	C 48
	?debug	C E92008021715433A5C54435C494E434C5544455C535444494F2E48
	?debug	C E92008021715433A5C54435C494E434C5544455C434F4E494F2E48
	?debug	C E92008021716433A5C54435C494E434C5544455C535452494E472E+
	?debug	C 48
LA70_DSK_TEXT	segment byte public 'CODE'
LA70_DSK_TEXT	ends
	assume	cs:LA70_DSK_TEXT,ds:LA70_DSK_DATA
LA70_DSK_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
b@	label	byte
b@w	label	word
_ErrMsg	label	dword
	dd	s@
	dd	s@+32
	dd	s@+90
	dd	s@+135
	dd	s@+157
	dd	s@+194
	dd	s@+229
_ErrNum	label	byte
	db	0
_res_name	label	byte
	db	112
	db	115
	db	46
	db	100
	db	97
	db	116
	db	122 dup (0)
_FREQ	label	dword
	db	0
	db	64
	db	28
	db	70
_Base_Address	label	word
	db	0
	db	0
_VOLUME	label	word
	db	1
	db	0
_REM_SIZE	label	word
	db	0
	db	0
_wait_flag	label	byte
	db	0
_counter	label	word
	db	255
	db	255
_TmissIni	label	word
	db	1
	db	0
_Nchans	label	word
	db	1
	db	0
_old_index	label	word
	db	0
	db	0
_new_index	label	word
	db	0
	db	0
_DeviceBisyLoop	label	byte
	db	0
_inn	label	word
	db	0
	db	0
_outt	label	word
	db	0
	db	0
LA70_DSK_DATA	ends
LA70_DSK_TEXT	segment byte public 'CODE'
	?debug	C E8000A4C4137305F44534B2E432D519D26
	?debug	L 177
	assume	cs:LA70_DSK_TEXT
_New_Int15_Hndl	proc	far
	?debug	B
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,LA70_DSK_DATA
	mov	ds,bp
	mov	bp,sp
	?debug	B
	?debug	L 180
  	cmp	 ah,90h
	je	short @1@146
	?debug	L 181
	cmp	 ah,91h
	je	short @1@386
	?debug	L 182
	jmp	short @1@410
@1@146:
	?debug	L 195
	inc	byte ptr _DeviceBisyLoop
	?debug	L 196
   	cmp	 _DeviceBisyLoop,1
	?debug	L 197
	jne	short @1@410
@1@218:
	?debug	L 199
	les	bx,dword ptr _scr
	inc	word ptr es:[bx]
	?debug	L 200
   	sti	
	?debug	L 201
   	hlt	
	?debug	L 202
   	cli	
	?debug	L 203
   	cmp	 _DeviceBisyLoop,1
	?debug	L 204
	je	short @1@218
	?debug	L 205
	jmp	short @1@410
@1@386:
	?debug	L 207
	les	bx,dword ptr _scr
	inc	word ptr es:[bx+162]
	?debug	L 219
	dec	byte ptr _DeviceBisyLoop
	?debug	L 220
	jmp	short @1@410
@1@410:
	?debug	L 224
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
	?debug	E
	?debug	E
_New_Int15_Hndl	endp
LA70_DSK_TEXT	ends
LA70_DSK_DATA	segment word public 'DATA'
_ncr	label	byte
	db	0
LA70_DSK_DATA	ends
LA70_DSK_TEXT	segment byte public 'CODE'
	?debug	L 227
	assume	cs:LA70_DSK_TEXT
_New_Timer_Intr	proc	far
	?debug	B
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,LA70_DSK_DATA
	mov	ds,bp
	mov	bp,sp
	?debug	B
	?debug	L 233
	cmp	word ptr _Tmissing,0
	jne	short @2@74
	mov	dx,word ptr _hig_port
	mov	al,byte ptr _Channel_new_number
	out	dx,al
	in	al,dx
	and	al,15
	mov	ah,al
	dec	dx
	in	al,dx
	les	bx,dword ptr _BUF_POINTER
	mov	word ptr es:[bx],ax
	add	word ptr _BUF_POINTER,2
	out	dx,al
	dec	word ptr _counter
	mov	ax,word ptr _new_index
	mov	word ptr _old_index,ax
	mov	ax,word ptr _new_index
	inc	ax
	xor	dx,dx
	div	word ptr _Nchans
	mov	word ptr _new_index,dx
	mov	bx,word ptr _new_index
	mov	al,byte ptr _Channel[bx]
	mov	byte ptr _Channel_new_number,al
@2@74:
	mov	ax,word ptr _Tmissing
	inc	ax
	xor	dx,dx
	div	word ptr _TmissIni
	mov	word ptr _Tmissing,dx
	?debug	L 234
	mov	dx,32
	mov	al,32
	out	dx,al
	?debug	L 235
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
	?debug	E
	?debug	E
_New_Timer_Intr	endp
	?debug	L 241
	assume	cs:LA70_DSK_TEXT
_Disable_Acquisition	proc	near
	?debug	B
	push	bp
	mov	bp,sp
	push	ds
	mov	ax,LA70_DSK_DATA
	mov	ds,ax
	?debug	B
	?debug	L 244
	db	250
	?debug	L 245
	mov	dx,33
	mov	al,255
	out	dx,al
	?debug	L 246
	mov	dx,32
	mov	al,199
	out	dx,al
	?debug	L 250
	push	word ptr _Old_Timer_Intr+2
	push	word ptr _Old_Timer_Intr
	push	8
	call	far ptr _setvect
	add	sp,6
	?debug	L 251
	push	word ptr _Old_Int15_Hndl+2
	push	word ptr _Old_Int15_Hndl
	push	21
	call	far ptr _setvect
	add	sp,6
	mov	byte ptr _DeviceBisyLoop,0
	?debug	L 253
	mov	dx,67
	mov	al,54
	out	dx,al
	?debug	L 254
	mov	dx,64
	mov	al,255
	out	dx,al
	mov	dx,64
	mov	al,255
	out	dx,al
	?debug	L 256
	mov	dx,33
	mov	al,byte ptr _OldMask1
	out	dx,al
	?debug	L 257
	mov	dx,67
	mov	al,116
	out	dx,al
	?debug	L 258
	mov	dx,65
	mov	al,18
	out	dx,al
	?debug	L 259
	mov	dx,65
	mov	al,0
	out	dx,al
	?debug	L 261
	db	251
	?debug	L 262
	pop	ds
	pop	bp
	ret	
	?debug	E
	?debug	E
_Disable_Acquisition	endp
	?debug	L 264
	assume	cs:LA70_DSK_TEXT
_Enable_Acquisition	proc	near
	?debug	B
	enter	4,0
	push	ds
	mov	ax,LA70_DSK_DATA
	mov	ds,ax
	?debug	B
	?debug	L 267
	mov	ax,word ptr _Base_Address
	add	ax,4
	mov	word ptr _low_port,ax
	?debug	L 268
	mov	ax,word ptr _Base_Address
	add	ax,5
	mov	word ptr _hig_port,ax
	?debug	B
	?debug	L 273
	mov	ax,word ptr _rate+2
	mov	dx,word ptr _rate
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-2],ax
	jmp	short @4@122
@4@74:
	push	0
	push	word ptr _TmissIni
	push	word ptr _rate+2
	push	word ptr _rate
	call	far ptr F_LUDIV@
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-2],dx
	inc	word ptr _TmissIni
@4@122:
	cmp	word ptr [bp-2],0
	ja	short @4@74
	jb	short @4@194
	cmp	word ptr [bp-4],16384
	jae	short @4@74
@4@194:
	?debug	L 274
	mov	ax,word ptr [bp-4]
	mov	word ptr _Tcounter,ax
	?debug	C E601540C02FCFF
	?debug	E
	?debug	L 277
	cmp	word ptr _Tcounter,40
	jae	short @4@266
	mov	byte ptr _ErrNum,5
	mov	ax,1
	jmp	@4@770
@4@266:
	?debug	L 279
	db	250
	?debug	L 283
	mov	dx,33
	in	al,dx
	mov	byte ptr _OldMask1,al
	?debug	L 284
	mov	dx,33
	mov	al,255
	out	dx,al
	?debug	L 285
	mov	dx,32
	mov	al,192
	out	dx,al
	?debug	L 289
	cmp	word ptr _Tcounter,4096
	jae	short @4@314
	?debug	L 291
	mov	dx,67
	mov	al,116
	out	dx,al
	?debug	L 292
	mov	al,byte ptr _Tcounter
	and	al,255
	mov	dx,65
	out	dx,al
	?debug	L 293
	mov	ax,word ptr _Tcounter
	and	ax,65280
	shr	ax,8
	mov	dx,65
	out	dx,al
@4@314:
	?debug	L 296
	mov	dx,67
	mov	al,54
	out	dx,al
	?debug	L 297
	mov	dx,64
	mov	al,byte ptr _Tcounter
	out	dx,al
	?debug	L 298
	mov	ax,word ptr _Tcounter
	shr	ax,8
	mov	dx,64
	out	dx,al
	?debug	L 303
	mov	word ptr _new_index,0
	?debug	L 304
	mov	ax,word ptr _new_index
	mov	word ptr _old_index,ax
	?debug	L 305
	mov	bx,word ptr _new_index
	mov	al,byte ptr _Channel[bx]
	mov	byte ptr _Channel_new_number,al
	?debug	L 306
	mov	word ptr _Tmissing,0
	?debug	L 307
	cmp	word ptr _Tmissing,0
	jne	short @4@362
	mov	dx,word ptr _hig_port
	mov	al,byte ptr _Channel_new_number
	out	dx,al
	in	al,dx
	and	al,15
	mov	ah,al
	dec	dx
	in	al,dx
	les	bx,dword ptr _BUF_POINTER
	mov	word ptr es:[bx],ax
	add	word ptr _BUF_POINTER,2
	out	dx,al
	dec	word ptr _counter
	mov	ax,word ptr _new_index
	mov	word ptr _old_index,ax
	mov	ax,word ptr _new_index
	inc	ax
	xor	dx,dx
	div	word ptr _Nchans
	mov	word ptr _new_index,dx
	mov	bx,word ptr _new_index
	mov	al,byte ptr _Channel[bx]
	mov	byte ptr _Channel_new_number,al
@4@362:
	mov	ax,word ptr _Tmissing
	inc	ax
	xor	dx,dx
	div	word ptr _TmissIni
	mov	word ptr _Tmissing,dx
	sub	word ptr _BUF_POINTER,2
	inc	word ptr _counter
	?debug	L 309
	mov	dx,67
	mov	al,182
	out	dx,al
	?debug	L 310
	mov	dx,66
	mov	al,255
	out	dx,al
	mov	dx,66
	mov	al,255
	out	dx,al
	?debug	L 311
	mov	dx,97
	in	al,dx
	mov	dl,1
	or	dl,al
	mov	al,253
	and	al,dl
	mov	dx,97
	out	dx,al
	?debug	L 313
 	push	 ax
 	mov	 al,80h
 	out	 43h,al
 	nop	
	nop	
	nop	
 	in	 al,42h
 	mov	 ah,al
 	nop	
	nop	
	nop	
 	in	 al,42h
 	xchg	 ah,al
 	mov	 _T2Count,ax
 	pop	 ax
	?debug	L 314
	mov	ax,word ptr _T2Count
	mov	word ptr _T2pCount,ax
	?debug	L 318
	push	8
	call	far ptr _getvect
	pop	cx
	mov	word ptr _Old_Timer_Intr,ax
	mov	word ptr _Old_Timer_Intr+2,dx
	?debug	L 319
	push	cs
	push	offset _New_Timer_Intr
	push	8
	call	far ptr _setvect
	add	sp,6
	?debug	L 321
	push	21
	call	far ptr _getvect
	pop	cx
	mov	word ptr _Old_Int15_Hndl,ax
	mov	word ptr _Old_Int15_Hndl+2,dx
	?debug	L 322
	push	cs
	push	offset _New_Int15_Hndl
	push	21
	call	far ptr _setvect
	add	sp,6
	?debug	L 325
	db	251
	?debug	L 326
	xor	ax,ax
	jmp	short @4@770
@4@770:
	?debug	L 327
	pop	ds
	leave	
	ret	
	?debug	E
	?debug	E
_Enable_Acquisition	endp
LA70_DSK_TEXT	ends
LA70_DSK_DATA	segment word public 'DATA'
	db	0
	db	2
	db	16
	db	2
	db	32
	db	2
	db	48
	db	2
	db	0
	db	3
	db	16
	db	3
	db	32
	db	3
	db	48
	db	3
LA70_DSK_DATA	ends
LA70_DSK_TEXT	segment byte public 'CODE'
	?debug	L 329
	assume	cs:LA70_DSK_TEXT
_PS_available	proc	near
	?debug	B
	enter	16,0
	push	si
	push	ds
	mov	ax,LA70_DSK_DATA
	mov	ds,ax
	?debug	B
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	ds
	push	offset d@w+184
	mov	cx,16
	call	far ptr F_SCOPY@
	?debug	L 333
	xor	si,si
	jmp	short @5@146
@5@50:
	?debug	L 335
	mov	bx,si
	shl	bx,1
	lea	ax,word ptr [bp-16]
	add	bx,ax
	mov	dx,word ptr ss:[bx]
	add	dx,5
	mov	al,13
	out	dx,al
	?debug	L 336
	?debug	L 337
	mov	bx,si
	shl	bx,1
	lea	ax,word ptr [bp-16]
	add	bx,ax
	mov	dx,word ptr ss:[bx]
	add	dx,5
	in	al,dx
	mov	ah,0
	sar	ax,4
	cmp	ax,13
	je	short @5@98
	?debug	L 338
	jmp	short @5@122
@5@98:
	?debug	L 339
	mov	bx,si
	shl	bx,1
	lea	ax,word ptr [bp-16]
	add	bx,ax
	mov	ax,word ptr ss:[bx]
	jmp	short @5@194
@5@122:
	?debug	L 333
	inc	si
@5@146:
	cmp	si,8
	jl	short @5@50
	?debug	L 341
	xor	ax,ax
	jmp	short @5@194
@5@194:
	?debug	L 342
	pop	ds
	pop	si
	leave	
	ret	
	?debug	C E3180010001A04
	?debug	C E60562617365731802F0FF0169040406
	?debug	E
	?debug	E
_PS_available	endp
LA70_DSK_TEXT	ends
LA70_DSK_DATA	segment word public 'DATA'
_buf0	label	dword
	db	0
	db	0
	db	0
	db	0
_not_written	label	dword
	db	0
	db	0
	db	0
	db	0
_handler	label	word
	db	255
	db	255
	?debug	C E918519D260A43485F505344534B2E43
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	dd	s@+263
	dd	s@+282
	dd	s@+301
	dd	s@+326
	dd	s@+365
	dd	s@+393
	dd	s@+416
	dd	s@+465
	dd	s@+486
LA70_DSK_DATA	ends
LA70_DSK_TEXT	segment byte public 'CODE'
	?debug	L 346
	assume	cs:LA70_DSK_TEXT
_main	proc	far
	?debug	B
	enter	50,0
	push	si
	push	di
	push	ds
	mov	ax,LA70_DSK_DATA
	mov	ds,ax
	?debug	C E31A000400160200
	?debug	C E319000400161A00
	?debug	C E603617267190A0800054E61726773040A0600
	mov	di,word ptr [bp+6]
	?debug	B
	?debug	L 350
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	?debug	L 351
	mov	word ptr [bp-8],0
	mov	word ptr [bp-6],0
	?debug	L 352
	mov	word ptr [bp-12],0
	mov	word ptr [bp-10],0
	?debug	L 353
	mov	word ptr [bp-16],0
	mov	word ptr [bp-14],0
	?debug	L 356
	mov	word ptr _scr,0
	mov	word ptr _scr+2,47104
	?debug	L 357
	mov	byte ptr _Channel,0
	?debug	B
	push	ss
	lea	ax,word ptr [bp-42]
	push	ax
	push	ds
	push	offset d@+210
	mov	cx,10
	call	far ptr F_SCOPY@
	?debug	C E8000A43485F505344534B2E4318519D26
	?debug	L 16
	?debug	L 22
	push	ds
	push	offset s@+511
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	L 24
	dec	di
	?debug	L 25
	cmp	di,5
	jle	short @6@98
	push	word ptr d@w+220+2
	push	word ptr d@w+220
	push	ds
	push	offset s@+824
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@98:
	?debug	L 27
	mov	byte ptr [bp-29],1
	jmp	@6@290
@6@122:
	mov	byte ptr [bp-30],0
	jmp	short @6@242
@6@146:
	?debug	L 29
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr [bp-30]
	cbw	
	add	bx,ax
	mov	al,byte ptr es:[bx]
	mov	byte ptr [bp-31],al
	?debug	L 30
	cmp	byte ptr [bp-31],97
	jl	short @6@218
	cmp	byte ptr [bp-31],122
	jg	short @6@218
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr [bp-30]
	cbw	
	add	bx,ax
	mov	al,byte ptr es:[bx]
	add	al,224
	push	ax
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr [bp-30]
	cbw	
	add	bx,ax
	pop	ax
	mov	byte ptr es:[bx],al
@6@218:
	?debug	L 27
	inc	byte ptr [bp-30]
@6@242:
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _strlen
	pop	cx
	pop	cx
	push	ax
	mov	al,byte ptr [bp-30]
	cbw	
	pop	dx
	cmp	dx,ax
	jbe	@@85
	jmp	@6@146
@@85:
	inc	byte ptr [bp-29]
@6@290:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	@@86
	jmp	@6@122
@@86:
	?debug	L 35
	mov	byte ptr [bp-29],1
	jmp	short @6@482
@6@338:
	?debug	L 36
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],47
	je	short @6@410
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],45
	je	short @6@410
	jmp	short @6@458
@6@410:
	?debug	L 37
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+1],63
	jne	short @6@458
@6@434:
	?debug	L 40
	?debug	L 64
	push	ds
	push	offset s@+830
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	L 65
	mov	ax,1
	jmp	@6@6794
@6@458:
	?debug	L 35
	inc	byte ptr [bp-29]
@6@482:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jle	short @6@338
	?debug	L 69
	or	di,di
	je	short @6@698
@6@530:
	?debug	L 71
	les	bx,dword ptr [bp+8]
	les	bx,dword ptr es:[bx+4]
	cmp	byte ptr es:[bx],47
	je	short @6@578
	les	bx,dword ptr [bp+8]
	les	bx,dword ptr es:[bx+4]
	cmp	byte ptr es:[bx],45
	jne	short @6@602
@6@578:
	jmp	short @6@698
@6@602:
	?debug	L 72
	les	bx,dword ptr [bp+8]
	push	word ptr es:[bx+6]
	push	word ptr es:[bx+4]
	call	far ptr _strlen
	pop	cx
	pop	cx
	mov	byte ptr [bp-29],al
	?debug	L 73
	cmp	byte ptr [bp-29],127
	jl	short @6@650
	push	word ptr d@w+220+6
	push	word ptr d@w+220+4
	push	ds
	push	offset s@+1567
	call	far ptr _printf
	add	sp,8
	jmp	short @6@434
@6@650:
	?debug	L 74
	mov	byte ptr [bp-41],1
	?debug	L 75
	les	bx,dword ptr [bp+8]
	push	word ptr es:[bx+6]
	push	word ptr es:[bx+4]
	push	ds
	push	offset _res_name
	call	far ptr _strcpy
	add	sp,8
	?debug	L 76
	jmp	short @6@698
	?debug	L 77
	jmp	short @6@530
@6@698:
	?debug	B
	?debug	L 81
	mov	byte ptr [bp-31],0
	?debug	L 83
	mov	byte ptr [bp-29],1
	jmp	short @6@866
@6@722:
	?debug	L 84
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],47
	je	short @6@794
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],45
	je	short @6@794
	jmp	short @6@842
@6@794:
	?debug	L 85
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+1],65
	jne	short @6@842
	push	ss
	lea	ax,word ptr [bp-44]
	push	ax
	push	ds
	push	offset s@+1573
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx]
	inc	ax
	inc	ax
	push	word ptr es:[bx+2]
	push	ax
	call	far ptr _sscanf
	add	sp,12
	mov	byte ptr [bp-31],al
	jmp	short @6@890
@6@842:
	?debug	L 83
	inc	byte ptr [bp-29]
@6@866:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jle	short @6@722
@6@890:
	?debug	L 87
	cmp	byte ptr [bp-31],0
	je	short @6@986
	?debug	L 89
	mov	dx,word ptr [bp-44]
	add	dx,5
	mov	al,13
	out	dx,al
	?debug	L 90
	?debug	L 91
	mov	dx,word ptr [bp-44]
	add	dx,5
	in	al,dx
	mov	ah,0
	sar	ax,4
	cmp	ax,13
	je	short @6@962
	?debug	L 92
	mov	word ptr _Base_Address,0
	?debug	L 93
	push	ds
	push	offset s@+1577
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	L 94
	jmp	short @6@986
@6@962:
	?debug	L 95
	mov	ax,word ptr [bp-44]
	mov	word ptr _Base_Address,ax
@6@986:
	?debug	L 97
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	short @6@1034
	mov	al,byte ptr [bp-29]
	cbw	
	lea	dx,word ptr [bp-42]
	add	ax,dx
	mov	bx,ax
	mov	byte ptr ss:[bx],1
@6@1034:
	?debug	C E601620402D4FF
	?debug	E
	?debug	L 101
	mov	byte ptr [bp-29],1
	jmp	@6@1514
@6@1058:
	?debug	L 102
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],47
	je	short @6@1130
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],45
	je	short @6@1130
	jmp	@6@1490
@6@1130:
	?debug	L 103
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+1],67
	je	@@87
	jmp	@6@1490
@@87:
	?debug	L 104
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _strlen
	pop	cx
	pop	cx
	dec	ax
	dec	ax
	mov	word ptr _Nchans,ax
	?debug	L 105
	mov	word ptr [bp-18],0
	jmp	short @6@1370
@6@1178:
	?debug	B
	?debug	L 107
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr [bp-18]
	add	dx,word ptr es:[bx]
	mov	bx,dx
	mov	es,ax
	mov	al,byte ptr es:[bx+2]
	mov	cl,al
	?debug	L 108
	cmp	cl,48
	jl	short @6@1250
	cmp	cl,57
	jg	short @6@1250
	mov	al,cl
	add	al,208
	mov	bx,word ptr [bp-18]
	mov	byte ptr _Channel[bx],al
	jmp	short @6@1346
@6@1250:
	?debug	L 110
	cmp	cl,65
	jl	short @6@1322
	cmp	cl,70
	jg	short @6@1322
	mov	al,cl
	add	al,201
	mov	bx,word ptr [bp-18]
	mov	byte ptr _Channel[bx],al
	jmp	short @6@1346
@6@1322:
	?debug	L 111
	mov	word ptr _Nchans,0
	jmp	short @6@1394
@6@1346:
	?debug	C E60178020401
	?debug	E
	?debug	L 105
	inc	word ptr [bp-18]
@6@1370:
	mov	ax,word ptr [bp-18]
	cmp	ax,word ptr _Nchans
	jb	short @6@1178
@6@1394:
	?debug	L 113
	cmp	word ptr _Nchans,16
	ja	short @6@1442
	cmp	word ptr _Nchans,0
	jne	short @6@1466
@6@1442:
	push	word ptr d@w+220+14
	push	word ptr d@w+220+12
	push	ds
	push	offset s@+1579
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@1466:
	?debug	L 114
	jmp	short @6@1538
@6@1490:
	?debug	L 101
	inc	byte ptr [bp-29]
@6@1514:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	@@88
	jmp	@6@1058
@@88:
@6@1538:
	?debug	L 117
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	short @6@1586
	mov	al,byte ptr [bp-29]
	cbw	
	lea	dx,word ptr [bp-42]
	add	ax,dx
	mov	bx,ax
	mov	byte ptr ss:[bx],1
@6@1586:
	?debug	L 120
	FLD	dword ptr s@+1585
	FDIV	dword ptr _FREQ
	call	far ptr F_FTOL@
	mov	word ptr _rate,ax
	mov	word ptr _rate+2,dx
	?debug	L 122
	mov	byte ptr [bp-29],1
	jmp	@6@2282
@6@1610:
	?debug	L 123
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],47
	je	short @6@1682
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],45
	je	short @6@1682
	jmp	@6@2258
@6@1682:
	?debug	L 124
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+1],82
	je	short @6@1730
	jmp	@6@2258
@6@1730:
	?debug	L 125
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr es:[bx+2]
	cbw	
	cmp	ax,67
	jne	@@89
	jmp	@6@1994
@@89:
	cmp	ax,70
	je	short @6@1850
	cmp	ax,73
	jne	@@90
	jmp	@6@2138
@@90:
	jmp	@6@2210
@6@1850:
	?debug	L 126
	push	ds
	push	offset _FREQ
	push	ds
	push	offset s@+1589
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx]
	add	ax,3
	push	word ptr es:[bx+2]
	push	ax
	call	far ptr _sscanf
	add	sp,12
	mov	byte ptr [bp-31],al
	?debug	L 127
	mov	al,byte ptr [bp-31]
	cbw	
	or	ax,ax
	je	short @6@1898
	FLD	dword ptr _FREQ
	FLDZ	
	FCOMPP
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	jne	short @6@1922
@6@1898:
	push	word ptr d@w+220+10
	push	word ptr d@w+220+8
	push	ds
	push	offset s@+1592
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@1922:
	?debug	L 128
	FLD	dword ptr s@+1585
	FDIV	dword ptr _FREQ
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FWAIT	
	call	far ptr _floor
	add	sp,8
	call	far ptr F_FTOL@
	mov	word ptr _rate,ax
	mov	word ptr _rate+2,dx
	?debug	L 129
	FLD	dword ptr s@+1585
	FDIV	dword ptr _FREQ
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FWAIT	
	call	far ptr _floor
	add	sp,8
	mov	ax,word ptr _rate+2
	mov	dx,word ptr _rate
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FCOMPP
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	je	short @6@1970
	push	word ptr d@w+220+10
	push	word ptr d@w+220+8
	push	ds
	push	offset s@+1598
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@1970:
	jmp	@6@2234
@6@1994:
	?debug	L 130
	push	ds
	push	offset _FREQ
	push	ds
	push	offset s@+1604
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx]
	add	ax,3
	push	word ptr es:[bx+2]
	push	ax
	call	far ptr _sscanf
	add	sp,12
	mov	byte ptr [bp-31],al
	?debug	L 131
	mov	ax,word ptr _Nchans
	mov	word ptr [bp-26],ax
	mov	word ptr [bp-24],0
	FILD	dword ptr [bp-26]
	FMUL	dword ptr _FREQ
	FSTP	dword ptr _FREQ
	?debug	L 132
	mov	al,byte ptr [bp-31]
	cbw	
	or	ax,ax
	je	short @6@2042
	FLD	dword ptr _FREQ
	FLDZ	
	FCOMPP
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	jne	short @6@2066
@6@2042:
	push	word ptr d@w+220+10
	push	word ptr d@w+220+8
	push	ds
	push	offset s@+1607
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2066:
	?debug	L 133
	FLD	dword ptr s@+1585
	FDIV	dword ptr _FREQ
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FWAIT	
	call	far ptr _floor
	add	sp,8
	call	far ptr F_FTOL@
	mov	word ptr _rate,ax
	mov	word ptr _rate+2,dx
	?debug	L 134
	FLD	dword ptr s@+1585
	FDIV	dword ptr _FREQ
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FWAIT	
	call	far ptr _floor
	add	sp,8
	mov	ax,word ptr _rate+2
	mov	dx,word ptr _rate
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FCOMPP
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	je	short @6@2114
	push	word ptr d@w+220+10
	push	word ptr d@w+220+8
	push	ds
	push	offset s@+1613
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2114:
	jmp	@6@2234
@6@2138:
	?debug	L 135
	push	ds
	push	offset _rate
	push	ds
	push	offset s@+1619
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx]
	add	ax,3
	push	word ptr es:[bx+2]
	push	ax
	call	far ptr _sscanf
	add	sp,12
	mov	byte ptr [bp-31],al
	?debug	L 136
	push	word ptr _rate+2
	push	word ptr _rate
	push	18
	push	14172
	call	far ptr F_LUDIV@
	mov	word ptr [bp-26],ax
	mov	word ptr [bp-24],dx
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FSTP	dword ptr _FREQ
	?debug	L 137
	FWAIT	
	mov	al,byte ptr [bp-31]
	cbw	
	or	ax,ax
	jne	short @6@2186
	push	word ptr d@w+220+10
	push	word ptr d@w+220+8
	push	ds
	push	offset s@+1623
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2186:
	jmp	short @6@2234
@6@2210:
	?debug	L 138
	push	word ptr d@w+220+30
	push	word ptr d@w+220+28
	push	ds
	push	offset s@+1629
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2234:
	?debug	L 140
	jmp	short @6@2306
@6@2258:
	?debug	L 122
	inc	byte ptr [bp-29]
@6@2282:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	@@91
	jmp	@6@1610
@@91:
@6@2306:
	?debug	L 143
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	short @6@2354
	mov	al,byte ptr [bp-29]
	cbw	
	lea	dx,word ptr [bp-42]
	add	ax,dx
	mov	bx,ax
	mov	byte ptr ss:[bx],1
@6@2354:
	?debug	L 146
	mov	word ptr _DIM,32768
	mov	word ptr _DIM+2,0
	?debug	L 147
	?debug	L 148
	mov	word ptr _SINGLE_BUF_SIZE,16384
	?debug	L 150
	mov	byte ptr [bp-29],1
	jmp	@6@2978
@6@2378:
	?debug	L 151
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],47
	je	short @6@2450
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],45
	je	short @6@2450
	jmp	@6@2954
@6@2450:
	?debug	L 152
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+1],86
	je	@@92
	jmp	@6@2954
@@92:
	?debug	B
	?debug	L 154
	push	ss
	lea	ax,word ptr [bp-46]
	push	ax
	push	ds
	push	offset s@+1635
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	mov	ax,word ptr es:[bx]
	add	ax,3
	push	word ptr es:[bx+2]
	push	ax
	call	far ptr _sscanf
	add	sp,12
	mov	byte ptr [bp-31],al
	?debug	L 155
	mov	al,byte ptr [bp-31]
	cbw	
	or	ax,ax
	jne	short @6@2522
	push	word ptr d@w+220+14
	push	word ptr d@w+220+12
	push	ds
	push	offset s@+1639
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2522:
	?debug	L 156
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr es:[bx+2]
	cbw	
	mov	word ptr [bp-28],ax
	mov	cx,5
	mov	bx,offset @6@C22450
@6@2570:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-28]
	je	short @6@2642
	inc	bx
	inc	bx
	loop	short @6@2570
	jmp	@6@2786
@6@2642:
	jmp	word ptr cs:[bx+10]
@6@2666:
	?debug	L 158
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FLD1	
	FMUL	
	FSTP	dword ptr [bp-50]
	FWAIT	
	jmp	@6@2810
@6@2690:
	?debug	L 159
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FLD1	
	FMUL	
	mov	ax,word ptr _Nchans
	mov	word ptr [bp-26],ax
	mov	word ptr [bp-24],0
	FILD	dword ptr [bp-26]
	FMUL	
	mov	ax,word ptr _DIM+2
	mov	dx,word ptr _DIM
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FDIV	
	FSTP	dword ptr [bp-50]
	FWAIT	
	jmp	@6@2810
@6@2714:
	?debug	L 160
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FLD1	
	FMUL	
	mov	ax,word ptr _DIM+2
	mov	dx,word ptr _DIM
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FDIV	
	FMUL	dword ptr _FREQ
	FSTP	dword ptr [bp-50]
	FWAIT	
	jmp	@6@2810
@6@2738:
	?debug	L 161
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FMUL	dword ptr s@+1645
	mov	ax,word ptr _DIM+2
	mov	dx,word ptr _DIM
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FDIV	
	FMUL	dword ptr _FREQ
	FSTP	dword ptr [bp-50]
	FWAIT	
	jmp	short @6@2810
@6@2762:
	?debug	L 162
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FMUL	dword ptr s@+1649
	mov	ax,word ptr _DIM+2
	mov	dx,word ptr _DIM
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FDIV	
	FMUL	dword ptr _FREQ
	FSTP	dword ptr [bp-50]
	FWAIT	
	jmp	short @6@2810
@6@2786:
	?debug	L 167
	push	word ptr d@w+220+18
	push	word ptr d@w+220+16
	push	ds
	push	offset s@+1653
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2810:
	?debug	L 170
	FLD	dword ptr [bp-50]
	call	far ptr F_FTOL@
	mov	word ptr [bp-46],ax
	mov	word ptr [bp-44],dx
	?debug	L 171
	mov	ax,word ptr [bp-44]
	mov	dx,word ptr [bp-46]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FSUBR	dword ptr [bp-50]
	mov	ax,word ptr _DIM+2
	mov	dx,word ptr _DIM
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FMUL	
	call	far ptr F_FTOL@
	mov	word ptr _REM_SIZE,ax
	?debug	L 172
	mov	ax,word ptr [bp-46]
	mov	word ptr _VOLUME,ax
	?debug	L 173
	?debug	L 174
	cmp	word ptr _VOLUME,0
	jne	short @6@2858
	cmp	word ptr _REM_SIZE,0
	je	short @6@2906
@6@2858:
	mov	ax,word ptr _VOLUME
	xor	dx,dx
	cmp	dx,word ptr [bp-44]
	jne	short @6@2906
	cmp	ax,word ptr [bp-46]
	je	short @6@2930
@6@2906:
	push	word ptr d@w+220+26
	push	word ptr d@w+220+24
	push	ds
	push	offset s@+1659
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@2930:
	?debug	L 176
	jmp	short @6@3002
@6@2954:
	?debug	C E603564F4C0E02CEFF066E756D6E756D0C02D2FF
	?debug	E
	?debug	L 150
	inc	byte ptr [bp-29]
@6@2978:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	@@93
	jmp	@6@2378
@@93:
@6@3002:
	?debug	L 179
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jg	short @6@3050
	mov	al,byte ptr [bp-29]
	cbw	
	lea	dx,word ptr [bp-42]
	add	ax,dx
	mov	bx,ax
	mov	byte ptr ss:[bx],1
@6@3050:
	?debug	L 181
	mov	byte ptr [bp-29],1
	jmp	short @6@3146
@6@3074:
	?debug	L 182
	mov	al,byte ptr [bp-29]
	cbw	
	lea	dx,word ptr [bp-42]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	cbw	
	or	ax,ax
	jne	short @6@3122
	?debug	L 183
	mov	al,byte ptr [bp-29]
	cbw	
	shl	ax,2
	les	bx,dword ptr [bp+8]
	add	bx,ax
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	push	ds
	push	offset s@+1665
	call	far ptr _printf
	add	sp,8
	push	word ptr d@w+220+22
	push	word ptr d@w+220+20
	push	ds
	push	offset s@+1677
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@3122:
	?debug	L 181
	inc	byte ptr [bp-29]
@6@3146:
	mov	al,byte ptr [bp-29]
	cbw	
	cmp	ax,di
	jle	short @6@3074
	?debug	L 185
	cmp	word ptr _Base_Address,0
	jne	short @6@3218
	call	near ptr _PS_available
	mov	word ptr _Base_Address,ax
@6@3218:
	?debug	L 186
	cmp	word ptr _Base_Address,0
	jne	short @6@3266
	push	word ptr d@w+220+34
	push	word ptr d@w+220+32
	push	ds
	push	offset s@+1683
	call	far ptr _printf
	add	sp,8
	jmp	@6@434
@6@3266:
	?debug	C E31C000400160200
	?debug	C E31B0024001A1C
	?debug	C E31D000A001A02
	?debug	C E6094172674572724D73671B000002DC000A7265+
	?debug	C 636F676E697A65641D02D6FF016B0202E1FF016D+
	?debug	C 0202E2FF01630202E3FF
	?debug	E
	?debug	C E801
	?debug	L 361
	push	4
	push	word ptr _SINGLE_BUF_SIZE
	call	far ptr _tcalloc
	pop	cx
	pop	cx
	mov	word ptr _buf0,ax
	mov	word ptr _buf0+2,dx
	?debug	L 362
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	jne	short @6@3410
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	push	word ptr _ErrMsg+10
	push	word ptr _ErrMsg+8
	push	ds
	push	offset s@+1689
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@3338
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@3338:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@3386
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@3386:
	mov	ax,2
	jmp	@6@6794
@6@3410:
	?debug	L 363
	mov	ax,word ptr _buf0+2
	mov	dx,word ptr _buf0
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-2],ax
	?debug	L 365
	mov	ax,word ptr _SINGLE_BUF_SIZE
	shl	ax,1
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	add	bx,ax
	mov	word ptr [bp-8],bx
	mov	word ptr [bp-6],dx
	?debug	L 366
	mov	ax,word ptr _SINGLE_BUF_SIZE
	shr	ax,1
	shl	ax,1
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	add	bx,ax
	mov	word ptr [bp-12],bx
	mov	word ptr [bp-10],dx
	?debug	L 367
	mov	ax,word ptr _SINGLE_BUF_SIZE
	shr	ax,1
	shl	ax,1
	mov	dx,word ptr [bp-6]
	mov	bx,word ptr [bp-8]
	add	bx,ax
	mov	word ptr [bp-16],bx
	mov	word ptr [bp-14],dx
	?debug	L 368
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr _BUF_POINTER,dx
	mov	word ptr _BUF_POINTER+2,ax
	?debug	L 370
	?debug	L 371
	push	32
	push	ds
	push	offset _res_name
	call	far ptr __creat
	add	sp,6
	mov	word ptr _handler,ax
	?debug	L 374
	push	ds
	push	offset s@+1701
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	L 375
	?debug	L 377
	mov	ax,word ptr _DIM+2
	mov	bx,word ptr _DIM
	shl	bx,1
	rcl	ax,1
	push	bx
	push	ax
	mov	ax,word ptr _VOLUME
	xor	dx,dx
	pop	cx
	pop	bx
	call	far ptr F_LXMUL@
	mov	bx,word ptr _REM_SIZE
	xor	si,si
	shl	bx,1
	rcl	si,1
	add	ax,bx
	adc	dx,si
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	or	ax,ax
	je	short @6@3554
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	push	word ptr _ErrMsg+6
	push	word ptr _ErrMsg+4
	push	ds
	push	offset s@+1710
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@3482
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@3482:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@3530
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@3530:
	mov	ax,1
	jmp	@6@6794
@6@3554:
	?debug	L 379
	push	0
	push	0
	push	0
	push	word ptr _handler
	call	far ptr _lseek
	add	sp,8
	?debug	B
	?debug	B
	?debug	L 382
	mov	word ptr [bp-40],1
	mov	word ptr [bp-38],0
	?debug	L 383
	mov	ax,word ptr _rate+2
	mov	dx,word ptr _rate
	mov	word ptr [bp-36],dx
	mov	word ptr [bp-34],ax
	jmp	short @6@3650
@6@3602:
	push	word ptr [bp-38]
	push	word ptr [bp-40]
	push	word ptr _rate+2
	push	word ptr _rate
	call	far ptr F_LUDIV@
	mov	word ptr [bp-36],ax
	mov	word ptr [bp-34],dx
	add	word ptr [bp-40],1
	adc	word ptr [bp-38],0
@6@3650:
	cmp	word ptr [bp-34],0
	ja	short @6@3602
	jb	short @6@3722
	cmp	word ptr [bp-36],16384
	jae	short @6@3602
@6@3722:
	?debug	L 384
	mov	ax,word ptr [bp-34]
	mov	dx,word ptr [bp-36]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	mov	ax,word ptr [bp-38]
	mov	dx,word ptr [bp-40]
	mov	word ptr [bp-26],dx
	mov	word ptr [bp-24],ax
	mov	word ptr [bp-22],0
	mov	word ptr [bp-20],0
	FILD	qword ptr [bp-26]
	FMUL	
	FSTP	dword ptr [bp-44]
	?debug	L 385
	FLD	dword ptr s@+1585
	FDIV	dword ptr [bp-44]
	FSTP	dword ptr _FREQ
	FWAIT	
	?debug	C E60572726174650E02D4FF0254490C02D8FF0154+
	?debug	C 0C02DCFF
	?debug	E
	?debug	L 387
	FLD	dword ptr _FREQ
	FDIV	dword ptr s@+1722
	FSTP	dword ptr [bp-32]
	?debug	L 388
	?debug	L 396
	FWAIT	
	mov	ax,word ptr _VOLUME
	shl	ax,6
	mov	word ptr [bp-26],ax
	mov	word ptr [bp-24],0
	FILD	dword ptr [bp-26]
	FMUL	qword ptr s@+1975
	mov	ax,word ptr _REM_SIZE
	mov	word ptr [bp-26],ax
	mov	word ptr [bp-24],0
	FILD	dword ptr [bp-26]
	FDIV	dword ptr s@+1722
	FADD	
	FDIV	dword ptr [bp-32]
	add	sp,65528
	FSTP	qword ptr [bp-64]
	push	64
	FWAIT	
	push	word ptr _VOLUME
	push	word ptr _Base_Address
	push	ds
	push	offset s@+1726
	call	far ptr _printf
	add	sp,18
	?debug	L 397
	?debug	L 400
	FLD	dword ptr _FREQ
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FLD	dword ptr _FREQ
	FCOMP	qword ptr s@+2034
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	jae	short @6@3794
	mov	ax,3
	jmp	short @6@3914
@6@3794:
	FLD	dword ptr _FREQ
	FCOMP	qword ptr s@+2042
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	jae	short @6@3842
	mov	ax,2
	jmp	short @6@3914
@6@3842:
	FLD	dword ptr _FREQ
	FLD1	
	FCOMPP
	FSTSW	word ptr [bp-26]
	FWAIT	
	mov	ax,word ptr [bp-26]
	sahf	
	jbe	short @6@3890
	mov	ax,1
	jmp	short @6@3914
@6@3890:
	xor	ax,ax
@6@3914:
	push	ax
	push	ds
	push	offset s@+1983
	call	far ptr _printf
	add	sp,14
	?debug	L 401
	?debug	L 408
	FLD	dword ptr [bp-32]
	add	sp,65528
	FSTP	qword ptr [bp-64]
	FWAIT	
	push	word ptr _Nchans
	push	ds
	push	offset s@+2050
	call	far ptr _printf
	add	sp,14
	?debug	L 409
	call	far ptr _getch
	?debug	L 410
	push	ds
	push	offset s@+2330
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	C E6067065727365630E02E0FF
	?debug	E
	?debug	B
	?debug	L 414
	mov	word ptr [bp-32],0
	xor	si,si
	mov	word ptr [bp-34],32
	?debug	L 418
	mov	ax,word ptr _VOLUME
	shl	ax,2
	shr	ax,5
	mov	word ptr [bp-30],ax
	cmp	word ptr [bp-30],0
	jne	short @6@3986
	mov	ax,word ptr _VOLUME
	shl	ax,2
	mov	word ptr [bp-34],ax
@6@3986:
	?debug	L 419
	inc	word ptr [bp-30]
	?debug	L 421
	mov	ax,64
	mov	es,ax
	mov	ax,word ptr es:[80]
	shr	ax,8
	mov	word ptr _cRow,ax
	mov	ax,64
	mov	es,ax
	mov	ax,word ptr es:[80]
	and	ax,255
	mov	word ptr _cCol,ax
	?debug	L 422
	push	0
	mov	ax,word ptr _cRow
	sub	ax,3
	push	ax
	mov	ax,word ptr _cCol
	sub	ax,6
	push	ax
	call	far ptr _set_cur_loc
	add	sp,6
	?debug	L 423
	mov	word ptr [bp-18],1
	jmp	short @6@4106
@6@4010:
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@4058
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,177
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@4082
@6@4058:
	push	seg __streams
	push	offset __streams+20
	push	65457
	call	far ptr __fputc
	add	sp,6
@6@4082:
	inc	word ptr [bp-18]
@6@4106:
	mov	ax,word ptr [bp-18]
	cmp	ax,word ptr [bp-34]
	jbe	short @6@4010
	?debug	L 424
	push	0
	mov	ax,word ptr _cRow
	sub	ax,3
	push	ax
	mov	ax,word ptr _cCol
	sub	ax,6
	push	ax
	call	far ptr _set_cur_loc
	add	sp,6
	?debug	L 426
	call	near ptr _Enable_Acquisition
	or	ax,ax
	je	short @6@4274
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2354
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@4202
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@4202:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@4250
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@4250:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@4274:
	?debug	L 427
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr _not_written,dx
	mov	word ptr _not_written+2,ax
	?debug	L 428
	push	seg _cbrk_hnd
	push	offset _cbrk_hnd
	call	far ptr _ctrlbrk
	pop	cx
	pop	cx
	?debug	L 429
	cmp	word ptr _VOLUME,0
	jne	@@94
	jmp	@6@6098
@@94:
	?debug	L 430
	mov	dx,33
	mov	al,216
	out	dx,al
@6@4322:
	?debug	L 434
	inc	word ptr [bp-32]
	mov	ax,word ptr [bp-32]
	xor	dx,dx
	div	word ptr [bp-30]
	or	dx,dx
	jne	short @6@4442
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@4394
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@4418
@6@4394:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@4418:
	inc	si
@6@4442:
	jmp	short @6@4538
@6@4466:
	?debug	L 436
 	HLT	
	les	bx,dword ptr _scr
	inc	word ptr es:[bx+6]
	?debug	L 437
	call	far ptr _kbhit
	or	ax,ax
	je	short @6@4538
	call	far ptr _getch
@6@4538:
	?debug	L 435
	mov	ax,word ptr _counter
	test	word ptr _SINGLE_BUF_SIZE,ax
	jne	short @6@4466
	?debug	L 440
	mov	dx,33
	mov	al,218
	out	dx,al
	?debug	L 441
	push	word ptr _SINGLE_BUF_SIZE
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
	?debug	L 443
	cmp	byte ptr _ErrNum,0
	jne	@@95
	jmp	@6@4706
@@95:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2366
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@4634
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@4634:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@4682
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@4682:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@4706:
	?debug	L 444
	inc	word ptr [bp-32]
	mov	ax,word ptr [bp-32]
	xor	dx,dx
	div	word ptr [bp-30]
	or	dx,dx
	jne	short @6@4826
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@4778
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@4802
@6@4778:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@4802:
	inc	si
@6@4826:
	?debug	L 445
	mov	ax,word ptr _counter
	test	word ptr _SINGLE_BUF_SIZE,ax
	je	short @6@4874
	mov	byte ptr _ErrNum,4
@6@4874:
	?debug	L 446
	cmp	byte ptr _ErrNum,0
	jne	@@96
	jmp	@6@5018
@@96:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2378
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@4946
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@4946:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@4994
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@4994:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@5018:
	?debug	L 448
	push	word ptr _SINGLE_BUF_SIZE
	push	word ptr [bp-10]
	push	word ptr [bp-12]
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
	?debug	L 449
	mov	ax,word ptr [bp-6]
	mov	dx,word ptr [bp-8]
	mov	word ptr _not_written,dx
	mov	word ptr _not_written+2,ax
	?debug	L 450
	mov	dx,33
	mov	al,216
	out	dx,al
	?debug	L 451
	cmp	byte ptr _ErrNum,0
	jne	@@97
	jmp	@6@5162
@@97:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2390
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@5090
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@5090:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@5138
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@5138:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@5162:
	?debug	L 453
	inc	word ptr [bp-32]
	mov	ax,word ptr [bp-32]
	xor	dx,dx
	div	word ptr [bp-30]
	or	dx,dx
	jne	short @6@5282
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@5234
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@5258
@6@5234:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@5258:
	inc	si
@6@5282:
	jmp	short @6@5378
@6@5306:
	?debug	L 454
 	HLT	
	les	bx,dword ptr _scr
	inc	word ptr es:[bx+8]
	?debug	L 455
	call	far ptr _kbhit
	or	ax,ax
	je	short @6@5378
	call	far ptr _getch
@6@5378:
	?debug	L 454
	mov	ax,word ptr _counter
	test	word ptr _SINGLE_BUF_SIZE,ax
	je	short @6@5306
	?debug	L 458
	mov	dx,33
	mov	al,218
	out	dx,al
	?debug	L 459
	push	word ptr _SINGLE_BUF_SIZE
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
	?debug	L 460
	cmp	byte ptr _ErrNum,0
	jne	@@98
	jmp	@6@5546
@@98:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2402
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@5474
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@5474:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@5522
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@5522:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@5546:
	?debug	L 461
	inc	word ptr [bp-32]
	mov	ax,word ptr [bp-32]
	xor	dx,dx
	div	word ptr [bp-30]
	or	dx,dx
	jne	short @6@5666
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@5618
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@5642
@6@5618:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@5642:
	inc	si
@6@5666:
	?debug	L 462
	mov	ax,word ptr _counter
	test	word ptr _SINGLE_BUF_SIZE,ax
	jne	short @6@5714
	mov	byte ptr _ErrNum,4
@6@5714:
	?debug	L 463
	cmp	byte ptr _ErrNum,0
	jne	@@99
	jmp	@6@5858
@@99:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2414
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@5786
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@5786:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@5834
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@5834:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@5858:
	?debug	L 465
	push	word ptr _SINGLE_BUF_SIZE
	push	word ptr [bp-14]
	push	word ptr [bp-16]
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
	?debug	L 466
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr _not_written,dx
	mov	word ptr _not_written+2,ax
	?debug	L 467
	mov	dx,33
	mov	al,216
	out	dx,al
	?debug	L 468
	cmp	byte ptr _ErrNum,0
	jne	@@100
	jmp	@6@6002
@@100:
	call	near ptr _Disable_Acquisition
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2426
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@5930
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@5930:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@5978
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@5978:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	@6@6794
@6@6002:
	?debug	L 470
	dec	word ptr _VOLUME
	?debug	L 471
	cmp	word ptr _VOLUME,0
	jne	short @6@6050
	jmp	short @6@6074
@6@6050:
	jmp	@6@4322
@6@6074:
	?debug	L 473
	jmp	short @6@6122
@6@6098:
	mov	byte ptr _wait_flag,1
@6@6122:
	?debug	L 475
	cmp	word ptr _REM_SIZE,0
	jne	@@101
	jmp	@6@6458
@@101:
	?debug	B
	?debug	L 477
	mov	ax,65535
	sub	ax,word ptr _REM_SIZE
	mov	word ptr [bp-36],ax
	?debug	L 479
	inc	word ptr [bp-32]
	mov	ax,word ptr [bp-32]
	xor	dx,dx
	div	word ptr [bp-30]
	or	dx,dx
	jne	short @6@6266
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@6218
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@6242
@6@6218:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@6242:
	inc	si
@6@6266:
	?debug	L 480
	cmp	byte ptr _wait_flag,0
	je	short @6@6314
	mov	dx,33
	mov	al,216
	out	dx,al
@6@6314:
	jmp	short @6@6410
@6@6338:
	?debug	L 481
 	HLT	
	les	bx,dword ptr _scr
	inc	word ptr es:[bx+6]
	?debug	L 482
	call	far ptr _kbhit
	or	ax,ax
	je	short @6@6410
	call	far ptr _getch
@6@6410:
	?debug	L 481
	mov	ax,word ptr _counter
	cmp	ax,word ptr [bp-36]
	jae	short @6@6338
	?debug	L 484
	mov	ax,word ptr _REM_SIZE
	shl	ax,1
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
@6@6458:
	?debug	C E603636D700A02DCFF
	?debug	E
	jmp	short @6@6578
@6@6482:
	?debug	L 487
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+20
	inc	ax
	mov	dx,seg __streams
	mov	es,dx
	mov	word ptr es:__streams+20,ax
	or	ax,ax
	jge	short @6@6530
	mov	ax,seg __streams
	mov	es,ax
	mov	ax,word ptr es:__streams+34
	mov	bx,word ptr es:__streams+32
	inc	word ptr es:__streams+32
	mov	dl,219
	mov	es,ax
	mov	byte ptr es:[bx],dl
	mov	al,dl
	mov	ah,0
	jmp	short @6@6554
@6@6530:
	push	seg __streams
	push	offset __streams+20
	push	65499
	call	far ptr __fputc
	add	sp,6
@6@6554:
	inc	si
@6@6578:
	cmp	si,word ptr [bp-34]
	jb	short @6@6482
	?debug	C E6046C696E650A02DEFF064261724E756D0A0406+
	?debug	C 036E756D0A02E0FF0570696563650A02E2FF
	?debug	E
	?debug	L 489
	call	far ptr _kbhit
	or	ax,ax
	je	short @6@6674
	call	far ptr _getch
	or	ax,ax
	jne	short @6@6674
	call	far ptr _getch
@6@6674:
	?debug	L 492
	call	near ptr _Disable_Acquisition
	?debug	L 493
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	mov	al,byte ptr _ErrNum
	cbw	
	shl	ax,2
	mov	bx,ax
	push	word ptr _ErrMsg[bx+2]
	push	word ptr _ErrMsg[bx]
	push	ds
	push	offset s@+2438
	call	far ptr _printf
	add	sp,8
	mov	ax,word ptr _handler
	inc	ax
	je	short @6@6722
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@6@6722:
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @6@6770
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@6@6770:
	mov	al,byte ptr _ErrNum
	cbw	
	jmp	short @6@6794
@6@6794:
	?debug	L 494
	pop	ds
	pop	di
	pop	si
	leave	
	ret	
	?debug	C E31E000400160A00
	?debug	C E31F000400160A00
	?debug	C E320000400160A00
	?debug	C E321000400160A00
	?debug	C E601690402EEFF0562756632311E02F0FF056275+
	?debug	C 6631311F02F4FF04627566322002F8FF04627566+
	?debug	C 312102FCFF03617267190A0800054E6172677304+
	?debug	C 0C07
	?debug	E
	?debug	E
_main	endp
@6@C22450	label	word
	db	66
	db	0
	db	72
	db	0
	db	77
	db	0
	db	78
	db	0
	db	83
	db	0
	dw	@6@2666
	dw	@6@2762
	dw	@6@2738
	dw	@6@2690
	dw	@6@2714
	?debug	L 497
	assume	cs:LA70_DSK_TEXT
_cbrk_hnd	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	push	ds
	mov	ax,LA70_DSK_DATA
	mov	ds,ax
	?debug	B
	?debug	L 500
	call	near ptr _Disable_Acquisition
	?debug	L 501
	push	0
	push	word ptr _cRow
	push	word ptr _cCol
	call	far ptr _set_cur_loc
	add	sp,6
	?debug	L 502
	push	ds
	push	offset s@+2450
	call	far ptr _printf
	pop	cx
	pop	cx
	?debug	L 503
	mov	ax,word ptr _handler
	inc	ax
	je	short @7@122
	?debug	L 504
	mov	ax,word ptr _BUF_POINTER
	cmp	ax,word ptr _not_written
	jbe	short @7@98
	?debug	L 505
	push	0
	push	2
	mov	ax,word ptr _BUF_POINTER
	xor	dx,dx
	sub	ax,word ptr _not_written
	sbb	dx,0
	push	dx
	push	ax
	call	far ptr F_LDIV@
	shl	ax,1
	push	ax
	push	word ptr _not_written+2
	push	word ptr _not_written
	push	word ptr _handler
	call	far ptr __write
	add	sp,8
@7@98:
	?debug	L 506
	push	word ptr _handler
	call	far ptr _tell
	pop	cx
	push	dx
	push	ax
	push	word ptr _handler
	call	far ptr _chsize
	add	sp,6
	?debug	L 507
	push	word ptr _handler
	call	far ptr _close
	pop	cx
@7@122:
	?debug	L 508
	mov	ax,word ptr _buf0
	or	ax,word ptr _buf0+2
	je	short @7@170
	push	word ptr _buf0+2
	push	word ptr _buf0
	call	far ptr _tfree
	pop	cx
	pop	cx
@7@170:
	?debug	L 509
	xor	ax,ax
	jmp	short @7@194
@7@194:
	?debug	L 510
	pop	ds
	pop	bp
	ret	
	?debug	E
	?debug	E
_cbrk_hnd	endp
LA70_DSK_TEXT	ends
LA70_DSK_DATA	segment word public 'DATA'
_DIM	label	word
	db	4 dup (0)
_Tcounter	label	word
	db	2 dup (0)
_low_port	label	word
	db	2 dup (0)
_low_byte	label	byte
	db	1 dup (0)
_Channel	label	byte
	db	16 dup (0)
_OldMask1	label	byte
	db	1 dup (0)
_rate	label	word
	db	4 dup (0)
_BUF_POINTER	label	dword
	db	4 dup (0)
_Tmissing	label	word
	db	2 dup (0)
_cRow	label	word
	db	2 dup (0)
_Old_Int15_Hndl	label	dword
	db	4 dup (0)
_Old_Int76_Hndl	label	dword
	db	4 dup (0)
_hig_port	label	word
	db	2 dup (0)
_hig_byte	label	byte
	db	1 dup (0)
	db	1 dup (0)
_T2Delay	label	word
	db	2 dup (0)
_scr	label	dword
	db	4 dup (0)
_Old_Timer_Intr	label	dword
	db	4 dup (0)
_T2Count	label	word
	db	2 dup (0)
_T2pCount	label	word
	db	2 dup (0)
_cCol	label	word
	db	2 dup (0)
_SINGLE_BUF_SIZE	label	word
	db	2 dup (0)
_Channel_new_number	label	byte
	db	1 dup (0)
	?debug	C E9
s@	label	byte
	db	'0 - '
	db	-81
	db	-32
	db	-82
	db	-93
	db	-32
	db	-96
	db	-84
	db	-84
	db	-96
	db	' '
	db	-89
	db	-96
	db	-94
	db	-91
	db	-32
	db	-24
	db	-91
	db	-83
	db	-96
	db	' '
	db	-29
	db	-31
	db	-81
	db	-91
	db	-24
	db	-83
	db	-82
	db	0
	db	'1 - '
	db	-83
	db	-91
	db	' '
	db	-84
	db	-82
	db	-93
	db	-29
	db	' '
	db	-32
	db	-96
	db	-89
	db	-84
	db	-91
	db	-31
	db	-30
	db	-88
	db	-30
	db	-20
	db	' '
	db	-28
	db	-96
	db	-87
	db	-85
	db	' '
	db	-89
	db	-96
	db	-81
	db	-32
	db	-82
	db	-24
	db	-91
	db	-83
	db	-83
	db	-82
	db	-93
	db	-82
	db	' '
	db	-32
	db	-96
	db	-89
	db	-84
	db	-91
	db	-32
	db	-96
	db	' '
	db	-83
	db	-96
	db	' '
	db	-92
	db	-88
	db	-31
	db	-86
	db	-91
	db	0
	db	'2 - '
	db	-83
	db	-91
	db	' '
	db	-84
	db	-82
	db	-93
	db	-29
	db	' '
	db	-32
	db	-96
	db	-89
	db	-84
	db	-91
	db	-31
	db	-30
	db	-88
	db	-30
	db	-20
	db	' '
	db	-95
	db	-29
	db	-28
	db	-91
	db	-32
	db	' '
	db	-92
	db	-96
	db	-83
	db	-83
	db	-21
	db	-27
	db	' '
	db	-94
	db	' '
	db	-81
	db	-96
	db	-84
	db	-17
	db	-30
	db	-88
	db	0
	db	'3 - User'
	db	39
	db	's Ctrl-Break'
	db	0
	db	'4 - '
	db	-83
	db	-91
	db	-29
	db	-31
	db	-81
	db	-91
	db	-94
	db	' '
	db	-89
	db	-96
	db	-81
	db	-88
	db	-31
	db	-88
	db	' '
	db	-31
	db	-94
	db	-82
	db	-95
	db	-82
	db	-92
	db	-83
	db	-82
	db	-93
	db	-82
	db	' '
	db	-95
	db	-29
	db	-28
	db	-91
	db	-32
	db	-96
	db	0
	db	'5 - '
	db	-31
	db	-85
	db	-88
	db	-24
	db	-86
	db	-82
	db	-84
	db	' '
	db	-94
	db	-21
	db	-31
	db	-82
	db	-86
	db	-96
	db	-17
	db	' '
	db	-25
	db	-96
	db	-31
	db	-30
	db	-82
	db	-30
	db	-96
	db	' '
	db	-82
	db	-81
	db	-32
	db	-82
	db	-31
	db	-96
	db	0
	db	'6 - '
	db	-31
	db	-85
	db	-88
	db	-24
	db	-86
	db	-82
	db	-84
	db	' '
	db	-83
	db	-88
	db	-89
	db	-86
	db	-96
	db	-17
	db	' '
	db	-25
	db	-96
	db	-31
	db	-30
	db	-82
	db	-30
	db	-96
	db	' '
	db	-82
	db	-81
	db	-32
	db	-82
	db	-31
	db	-96
	db	0
	db	'Too many arguments'
	db	0
	db	'Too long file name'
	db	0
	db	'Rate: invalid assignment'
	db	0
	db	'Number of Channels: invalid assignment'
	db	0
	db	'Data Volume: invalid switch'
	db	0
	db	'Unrecognized parameter'
	db	0
	db	'Invalid (too long or zero) data volume requested'
	db	0
	db	'Rate: invalid switch'
	db	0
	db	'You PC has no LA70 board'
	db	0
	db	10
	db	'             '
	db	-55
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-72
	db	10
	db	'             '
	db	-70
	db	' PSV Tools 1.0, Chernyshyov V.Yu.   '
	db	-77
	db	10
	db	'             '
	db	-57
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-76
	db	10
	db	'             '
	db	-70
	db	' LA70 Fast Disk Acquisition Utility '
	db	-77
	db	10
	db	'             '
	db	-70
	db	'  Version 1.08, PSV Tools, 1992-93  '
	db	-77
	db	10
	db	'             '
	db	-45
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-39
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	-108
	db	-82
	db	-32
	db	-84
	db	-96
	db	-30
	db	' '
	db	-94
	db	-21
	db	-89
	db	-82
	db	-94
	db	-96
	db	': la70_dsk ['
	db	-88
	db	-84
	db	-17
	db	' '
	db	-28
	db	-96
	db	-87
	db	-85
	db	-96
	db	'] [/'
	db	-86
	db	-85
	db	-18
	db	-25
	db	'1] [/'
	db	-86
	db	-85
	db	-18
	db	-25
	db	'2]...'
	db	10
	db	-118
	db	-85
	db	-18
	db	-25
	db	-88
	db	': Cxx  - '
	db	-83
	db	-82
	db	-84
	db	-91
	db	-32
	db	-96
	db	' '
	db	-86
	db	-96
	db	-83
	db	-96
	db	-85
	db	-82
	db	-94
	db	' (hex<10) '
	db	-83
	db	-96
	db	-81
	db	-32
	db	-88
	db	-84
	db	-91
	db	-32
	db	' C01af, '
	db	-81
	db	-82
	db	' '
	db	-29
	db	-84
	db	-82
	db	-85
	db	-25
	db	-96
	db	-83
	db	-88
	db	-18
	db	' C0'
	db	10
	db	'       Axxx - '
	db	-95
	db	-96
	db	-89
	db	-82
	db	-94
	db	-21
	db	-87
	db	' '
	db	-96
	db	-92
	db	-32
	db	-91
	db	-31
	db	' LA70 (hex)'
	db	10
	db	'       R    - '
	db	-31
	db	-86
	db	-82
	db	-32
	db	-82
	db	-31
	db	-30
	db	-20
	db	' ('
	db	-30
	db	-91
	db	-84
	db	-81
	db	') '
	db	-82
	db	-81
	db	-32
	db	-82
	db	-31
	db	-96
	db	' (1):'
	db	10
	db	'              RFxxx - '
	db	-82
	db	-95
	db	-23
	db	-96
	db	-17
	db	' '
	db	-25
	db	-96
	db	-31
	db	-30
	db	-82
	db	-30
	db	-96
	db	' '
	db	-89
	db	-96
	db	-81
	db	-29
	db	-31
	db	-86
	db	-96
	db	' ['
	db	-125
	db	-26
	db	'],     '
	db	-81
	db	-82
	db	' '
	db	-29
	db	-84
	db	-82
	db	-85
	db	-25
	db	-96
	db	-83
	db	-88
	db	-18
	db	' 10032 <'
	db	-69
	db	10
	db	'              RIxxx - '
	db	-88
	db	-83
	db	-30
	db	-91
	db	-32
	db	-94
	db	-96
	db	-85
	db	' '
	db	-84
	db	-91
	db	-90
	db	-92
	db	-29
	db	' '
	db	-89
	db	-96
	db	-81
	db	-29
	db	-31
	db	-86
	db	-96
	db	-84
	db	-88
	db	' ['
	db	-30
	db	-88
	db	-86
	db	'], '
	db	-81
	db	-82
	db	' '
	db	-29
	db	-84
	db	-82
	db	-85
	db	-25
	db	-96
	db	-83
	db	-88
	db	-18
	db	' 119   <'
	db	-68
	db	10
	db	'              RCxxx - '
	db	-25
	db	-96
	db	-31
	db	-30
	db	-82
	db	-30
	db	-96
	db	' '
	db	-89
	db	-96
	db	-81
	db	-29
	db	-31
	db	-86
	db	-96
	db	' ['
	db	-125
	db	-26
	db	'] '
	db	-94
	db	' '
	db	-32
	db	-96
	db	-31
	db	-25
	db	-91
	db	-30
	db	-91
	db	' '
	db	-83
	db	-96
	db	' 1 '
	db	-86
	db	-96
	db	-83
	db	-96
	db	-85
	db	'.'
	db	10
	db	'       V    - '
	db	-82
	db	-95
	db	-22
	db	-91
	db	-84
	db	' '
	db	-31
	db	-82
	db	-95
	db	-88
	db	-32
	db	-96
	db	-91
	db	-84
	db	-82
	db	-87
	db	' '
	db	-88
	db	-83
	db	-28
	db	-82
	db	-32
	db	-84
	db	-96
	db	-26
	db	-88
	db	-88
	db	' (2):        '
	db	-81
	db	-82
	db	' '
	db	-29
	db	-84
	db	-82
	db	-85
	db	-25
	db	-96
	db	-83
	db	-88
	db	-18
	db	' 1 '
	db	-95
	db	-85
	db	-82
	db	-86
	db	10
	db	'              VSyyy - '
	db	-94
	db	' '
	db	-31
	db	-91
	db	-86
	db	-29
	db	-83
	db	-92
	db	-96
	db	-27
	db	',    VNyyy - '
	db	-94
	db	' '
	db	-82
	db	-30
	db	-31
	db	-25
	db	-91
	db	-30
	db	-96
	db	-27
	db	' ('
	db	-83
	db	-96
	db	' 1 '
	db	-86
	db	-96
	db	-83
	db	-96
	db	-85
	db	'),'
	db	10
	db	'              VMyyy - '
	db	-94
	db	' '
	db	-84
	db	-88
	db	-83
	db	-29
	db	-30
	db	-96
	db	-27
	db	',     VByyy - '
	db	-94
	db	' '
	db	-95
	db	-85
	db	-82
	db	-86
	db	-96
	db	-27
	db	' ('
	db	-81
	db	-82
	db	' 64 KB),'
	db	10
	db	'              VHyyy - '
	db	-94
	db	' '
	db	-25
	db	-96
	db	-31
	db	-96
	db	-27
	db	'.'
	db	10
	db	-111
	db	-95
	db	-82
	db	-32
	db	' '
	db	-92
	db	-96
	db	-83
	db	-83
	db	-21
	db	-27
	db	' '
	db	-84
	db	-82
	db	-90
	db	-91
	db	-30
	db	' '
	db	-95
	db	-21
	db	-30
	db	-20
	db	' '
	db	-82
	db	-31
	db	-30
	db	-96
	db	-83
	db	-82
	db	-94
	db	-85
	db	-91
	db	-83
	db	' '
	db	-94
	db	' '
	db	-85
	db	-18
	db	-95
	db	-82
	db	-87
	db	' '
	db	-84
	db	-82
	db	-84
	db	-91
	db	-83
	db	-30
	db	' '
	db	-83
	db	-96
	db	-90
	db	-96
	db	-30
	db	-88
	db	-91
	db	-84
	db	' Ctrl-Break.'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	'%3x'
	db	0
	db	7
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	-32
	db	-70
	db	-111
	db	'I%f'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	'%f'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	'%lu'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	'%lu'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	0
	db	0
	db	'pB'
	db	0
	db	0
	db	'aE'
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	'*** %s ***'
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	'%s'
	db	10
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	13
	db	'Wait...'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	0
	db	0
	db	0
	db	'D'
	db	13
	db	'             '
	db	-52
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-51
	db	-75
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-127
	db	-96
	db	-89
	db	-82
	db	-94
	db	-21
	db	-87
	db	' '
	db	-96
	db	-92
	db	-32
	db	-91
	db	-31
	db	'    %x         '
	db	9
	db	'  '
	db	-77
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-114
	db	-95
	db	-22
	db	-91
	db	-84
	db	' '
	db	-92
	db	-96
	db	-83
	db	-83
	db	-21
	db	-27
	db	'     %u '
	db	-95
	db	-85
	db	-82
	db	-86
	db	-82
	db	-94
	db	'  '
	db	9
	db	'  '
	db	-77
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-112
	db	-96
	db	-89
	db	-84
	db	-91
	db	-32
	db	' '
	db	-95
	db	-85
	db	-82
	db	-86
	db	-96
	db	'     %u KB      '
	db	9
	db	'  '
	db	-77
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-113
	db	-82
	db	-85
	db	-83
	db	-82
	db	-91
	db	' '
	db	-94
	db	-32
	db	-91
	db	-84
	db	-17
	db	'     %.1f '
	db	-31
	db	-91
	db	-86
	db	-29
	db	-83
	db	-92
	db	9
	db	'  '
	db	-77
	db	0
	db	-4
	db	-87
	db	-15
	db	-46
	db	'Mb'
	db	-16
	db	'?'
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-105
	db	-96
	db	-31
	db	-30
	db	-82
	db	-30
	db	-96
	db	' '
	db	-82
	db	-81
	db	-32
	db	-82
	db	-31
	db	-96
	db	'   %.*f '
	db	-125
	db	-26
	db	'     '
	db	9
	db	'  '
	db	-77
	db	0
	db	'{'
	db	20
	db	-82
	db	'G'
	db	-31
	db	'z'
	db	-124
	db	'?'
	db	-102
	db	-103
	db	-103
	db	-103
	db	-103
	db	-103
	db	-71
	db	'?'
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-105
	db	-88
	db	-31
	db	-85
	db	-82
	db	' '
	db	-86
	db	-96
	db	-83
	db	-96
	db	-85
	db	-82
	db	-94
	db	'    %i         '
	db	9
	db	'  '
	db	-77
	db	10
	db	'             '
	db	-70
	db	'  '
	db	-113
	db	-82
	db	-30
	db	-82
	db	-86
	db	' '
	db	-92
	db	-96
	db	-83
	db	-83
	db	-21
	db	-27
	db	'     %.1f KB/'
	db	-31
	db	-91
	db	-86
	db	9
	db	'  '
	db	-77
	db	10
	db	'             '
	db	-57
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-76
	db	10
	db	'             '
	db	-70
	db	'                                    '
	db	-77
	db	10
	db	'             '
	db	-45
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-60
	db	-39
	db	10
	db	10
	db	'Press any key to start'
	db	7
	db	0
	db	13
	db	'                      '
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	-114
	db	-24
	db	-88
	db	-95
	db	-86
	db	-96
	db	' %s'
	db	0
	db	10
	db	7
	db	'User'
	db	39
	db	't Ctrl-Break'
	db	0
LA70_DSK_DATA	ends
LA70_DSK_TEXT	segment byte public 'CODE'
LA70_DSK_TEXT	ends
	extrn	__turboFloat:word
	public	_New_Int15_Hndl
	public	_Channel_new_number
	extrn	__creat:far
	public	_SINGLE_BUF_SIZE
	public	_cCol
	public	_Base_Address
	extrn	_lseek:far
	public	_cbrk_hnd
	public	_T2pCount
	extrn	_set_cur_loc:far
	extrn	_printf:far
	public	_T2Count
	extrn	__fputc:far
	public	_Old_Timer_Intr
	public	_ErrNum
	public	_ErrMsg
	public	_scr
	public	_ncr
	public	_VOLUME
	public	_T2Delay
	public	_hig_byte
	public	_hig_port
	extrn	_kbhit:far
	public	_Old_Int76_Hndl
	public	_Old_Int15_Hndl
	extrn	_tfree:far
	public	_cRow
	public	_res_name
	public	_new_index
	public	_Tmissing
	public	_TmissIni
	public	_wait_flag
	public	_inn
	public	_handler
	extrn	_setvect:far
	extrn	_getvect:far
	public	_Enable_Acquisition
	public	_BUF_POINTER
	public	_REM_SIZE
	public	_rate
	public	_outt
	public	_old_index
	extrn	_tell:far
	extrn	_getch:far
	public	_main
	extrn	_chsize:far
	extrn	_ctrlbrk:far
	public	_PS_available
	public	_OldMask1
	public	_buf0
	public	_Disable_Acquisition
	extrn	_strcpy:far
	public	_FREQ
	public	_not_written
	extrn	_close:far
	public	_Channel
	extrn	_floor:far
	extrn	_strlen:far
	extrn	F_SCOPY@:far
	public	_low_byte
	public	_low_port
	extrn	F_LUDIV@:far
	extrn	__setargv__:far
	extrn	__streams:word
	extrn	F_LDIV@:far
	extrn	_tcalloc:far
	extrn	F_LXMUL@:far
	extrn	__write:far
	public	_Tcounter
	public	_New_Timer_Intr
	extrn	F_FTOL@:far
	extrn	_sscanf:far
	public	_DIM
	public	_counter
	public	_Nchans
	public	_DeviceBisyLoop
	?debug	C EA010D
	?debug	C E32200000023010700
	?debug	C EC0F5F4E65775F496E7431355F486E646C2298
	?debug	C EC135F4368616E6E656C5F6E65775F6E756D6265+
	?debug	C 720200
	?debug	C E32300000023040400
	?debug	C EB075F5F637265617423
	?debug	C EC105F53494E474C455F4255465F53495A450A00
	?debug	C EC055F63436F6C0A00
	?debug	C EC0D5F426173655F416464726573730A00
	?debug	C E32400000023060400
	?debug	C EB065F6C7365656B24
	?debug	C E32500000023040400
	?debug	C EC095F6362726B5F686E642518
	?debug	C EC095F543270436F756E740A00
	?debug	C E32600000023040400
	?debug	C EB0C5F7365745F6375725F6C6F6326
	?debug	C E32700000023040401
	?debug	C EB075F7072696E746627
	?debug	C EC085F5432436F756E740A00
	?debug	C E32800000023040400
	?debug	C EB075F5F667075746328
	?debug	C E32A00000023010700
	?debug	C E329000400162A00
	?debug	C EC0F5F4F6C645F54696D65725F496E74722900
	?debug	C EC075F4572724E756D0200
	?debug	C E32C000400160200
	?debug	C E32B001C001A2C
	?debug	C EC075F4572724D73672B00
	?debug	C E32D000400160A00
	?debug	C EC045F7363722D00
	?debug	C EC045F6E63720200
	?debug	C EC075F564F4C554D450A00
	?debug	C EC085F543244656C61790A00
	?debug	C EC095F6869675F627974650800
	?debug	C EC095F6869675F706F72740400
	?debug	C E32E00000023040400
	?debug	C EB065F6B626869742E
	?debug	C E33000000023010700
	?debug	C E32F000400163000
	?debug	C EC0F5F4F6C645F496E7437365F486E646C2F00
	?debug	C E33200000023010700
	?debug	C E331000400163200
	?debug	C EC0F5F4F6C645F496E7431355F486E646C3100
	?debug	C E33300000023010400
	?debug	C EB065F746672656533
	?debug	C EC055F63526F770A00
	?debug	C E3340080001A08
	?debug	C EC095F7265735F6E616D653400
	?debug	C EC0A5F6E65775F696E6465780A00
	?debug	C EC095F546D697373696E670A00
	?debug	C EC095F546D697373496E690A00
	?debug	C EC0A5F776169745F666C61670800
	?debug	C EC045F696E6E0A00
	?debug	C EC085F68616E646C65720400
	?debug	C E33500000023010400
	?debug	C EB085F7365747665637435
	?debug	C E33800000023010700
	?debug	C E337000400163800
	?debug	C E33600000023370400
	?debug	C EB085F6765747665637436
	?debug	C E33900000023040000
	?debug	C EC135F456E61626C655F4163717569736974696F+
	?debug	C 6E3918
	?debug	C E33A000400160A00
	?debug	C EC0C5F4255465F504F494E5445523A00
	?debug	C EC095F52454D5F53495A450A00
	?debug	C EC055F726174650C00
	?debug	C EC055F6F7574740A00
	?debug	C EC0A5F6F6C645F696E6465780A00
	?debug	C E33B00000023060400
	?debug	C EB055F74656C6C3B
	?debug	C E33C00000023040400
	?debug	C EB065F67657463683C
	?debug	C E33D00000023040400
	?debug	C EC055F6D61696E3D18
	?debug	C E33E00000023040400
	?debug	C EB075F636873697A653E
	?debug	C E33F00000023010400
	?debug	C EB085F6374726C62726B3F
	?debug	C E34000000023040000
	?debug	C EC0D5F50535F617661696C61626C654018
	?debug	C EC095F4F6C644D61736B310200
	?debug	C E341000400160A00
	?debug	C EC055F627566304100
	?debug	C E34200000023010000
	?debug	C EC145F44697361626C655F416371756973697469+
	?debug	C 6F6E4218
	?debug	C E344000400160200
	?debug	C E34300000023440400
	?debug	C EB075F73747263707943
	?debug	C EC055F465245510E00
	?debug	C E345000400160A00
	?debug	C EC0C5F6E6F745F7772697474656E4500
	?debug	C E34600000023040400
	?debug	C EB065F636C6F736546
	?debug	C E3470010001A02
	?debug	C EC085F4368616E6E656C4700
	?debug	C E348000000230F0400
	?debug	C EB065F666C6F6F7248
	?debug	C E349000000230A0400
	?debug	C EB075F7374726C656E49
	?debug	C E34A00000023040500
	?debug	C EB08465F53434F5059404A
	?debug	C EC095F6C6F775F627974650800
	?debug	C EC095F6C6F775F706F72740400
	?debug	C EB08465F4C55444956404A
	?debug	C EB0B5F5F736574617267765F5F4A
	?debug	C E34D000400160800
	?debug	C E34E000400160800
	?debug	C E200056C6576656C040005666C6167730A000266+
	?debug	C 64020004686F6C640800056273697A6504000662+
	?debug	C 75666665724D0004637572704E0006697374656D+
	?debug	C 700A8005746F6B656E04
	?debug	C E34C0014001E
	?debug	C E34B0000001A4C
	?debug	C EB095F5F73747265616D734B
	?debug	C EB07465F4C444956404A
	?debug	C E350000400160100
	?debug	C E34F00000023500400
	?debug	C EB085F7463616C6C6F634F
	?debug	C EB08465F4C584D554C404A
	?debug	C E35100000023040400
	?debug	C EB075F5F777269746551
	?debug	C EC095F54636F756E7465720A00
	?debug	C E35200000023010700
	?debug	C EC0F5F4E65775F54696D65725F496E74725298
	?debug	C EB07465F46544F4C404A
	?debug	C E35300000023040401
	?debug	C EB075F737363616E6653
	?debug	C EC045F44494D0C00
	?debug	C EC085F636F756E7465720A00
	?debug	C EC075F4E6368616E730A00
	?debug	C EC0F5F446576696365426973794C6F6F700800
	?debug	C E60673697A655F740A060666706F735F74060604+
	?debug	C 46494C454C06
	end
