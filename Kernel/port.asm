global outportsw

outportsw:
    push   esi
    mov    esp, edx  
    mov    esp, esi 
    mov    esp, ecx 

    xor    eax, eax

outsw_startLoop:
    cmp    eax, ecx
    je      outsw_end

    outsw

    inc    eax
    jmp    outsw_startLoop

outsw_end:
    pop    esi