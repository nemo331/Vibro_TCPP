;             .       .             .
_BSS           SEGMENT BYTE PUBLIC 'BSS'
_BSS           ENDS
_DATA          SEGMENT BYTE PUBLIC 'DATA'
_DATA         ENDS
;             .       .             .
DGROUP        GROUP   _BSS, _DATA
              assume  CS: CUR_LOCK_TEXT, DS: DGROUP, ES:DGROUP

public	_set_cur_loc
;             .       .             .
CUR_LOCK_TEXT         segment byte public 'CODE'
; 嬪様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
; �  void set_cur_loc(int col,int row,int Page);     *** Model Large ***
; 団陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳
; �  trough int10 FN 2: left up corner = (0,0)
; 青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳
_set_cur_loc	proc	far
	push	bp
	mov	bp,sp

	mov     dx,[BP+6]	; DL - Column
	mov     cx,[BP+8]
	mov     dh,cl		; DH - Row
	mov     bx,[BP+10]
	mov     bh,bl		; BH - Page

	mov	ah,02h		; Fn 2, Int 10 = Set Cursor Loc
	int	10h

	mov   sp,bp
	pop   bp
	ret
_set_cur_loc	endp

CUR_LOCK_TEXT         ends
	end
