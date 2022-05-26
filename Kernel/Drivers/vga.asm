global enableVideoMode
global disableVideoMode

enableVideoMode:
    mov ah, 0x00
    mov al, 0x13
    int 0x10


disableVideoMode:
    mov ah, 0x00
    mov al, 0x03
    int 0x10