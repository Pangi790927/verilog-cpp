; this is only a comment
 ; this is only a comment
    ; this is only a comment
    			; this is only a comment
		; this is only a comment

	jmp label
	.l32:
		; abc
; abc2    
	mov [eax + ebx + 123], ecx
	mov ecx, [eax + ebx + 123]
mov eax, ebx ; comment
	jmp label
; mov eax, ebx ; comment
label:
	mov eax,			 ebx ; comment
	mov [eax +   	 123],			 ebx ; comment
	jmp eax
	mov eax, 100
	mov ebx, 0xFF
	.local_label:
	jmp [eax + 322]
	jmp label
	not .local_label
	jmp .local_label
	mov [eax + ebx + abcedx], edx
	hlt
label2:
	jmp .local_label
	.local_label:

func:
	mov eax, 10
	.back:
	test eax
	je .back

func2:
	mov eax, 10
	.back:
	test eax
	je .back


; match
; extrct

; "matches" : {
	; "local_label": "{space}{.}{[_a-zA-Z][_a-z,A-Z,0-9]+}{space}+{:}{space}"
; }
; as.s.sad..d
