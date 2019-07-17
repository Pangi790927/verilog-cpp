; this is only a comment
 ; this is only a comment
    ; this is only a comment
    			; this is only a comment
		; this is only a comment


		; abc
; abc2    
mov eax, ebx ; comment
	jmp label
; mov eax, ebx ; comment
label:
	mov eax,			 ebx ; comment
	mov [eax +   	 123],			 ebx ; comment
	jmp eax
	mov eax, 100
	.local_label:
	jmp [eax + 322]
	jmp label
	jmp .local_label

label2:
	jmp .local_label
	.local_label:
