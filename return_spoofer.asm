        .DATA
        	return_address	dq	0
        	real_rbx		dq	0
         
        .CODE
        	spoof PROC
        		mov real_rbx, rbx
        		mov r9, [rsp]
        		mov return_address, r9
        		lea rbx, _ret_asm
        		mov [rsp], r8
        		jmp rdx
        	spoof ENDP
        	
        	_ret_asm PROC
        		mov rbx, real_rbx
        		mov rcx, return_address
        		jmp rcx
        	_ret_asm ENDP
         
        END