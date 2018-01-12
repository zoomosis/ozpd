; This program written by Paul Edwards and Matthew Parker
; released to the public domain
; Switch to protected mode and write an 'M' to the first character
; on the screen and then go into a loop
; assemble with tasm goprot
; link with tlink /3 goprot

% .model large,c
  .386p

_DATA   segment word public 'DATA'
; descriptors are defined as follows:
; first word is the segment limit, bits 15 to 0
; second word is the segment base, bits 15 to 0
; next byte is the segment base, bits 23 to 16
; next byte is the "access" byte
;   high bit is the "present" bit
;   next 2 bits are privilege level
;   next bit is segment descriptor, normally set to 1
;   next bit is the "executable" bit (ie set to 1 for code)
;   next bit is expansion direction for data segments, normally 0 
;      except for stacks, or "conforming" bit for code segments (ie 0)
;   next bit is the "writable" bit for data segments, ie normally
;      set to 1, or "readable" bit for code segments, ie normally 1
;   next bit is "accessed" bit
; next byte is the "granularity" byte
;   high bit is the granularity bit, 0 = byte, 1 = page
;   next bit is the default bit, 1 = 32-bit instructions
;   next 2 bits should be 0
;   next 4 bits are the segment limit, bits 19 to 16
; next byte is the segment base, bits 31 to 24

descriptors label qword
        dd 2 dup(0)  ; first a null descriptor
; code descriptor
; base address is 00000000h
; limit is fffffh (4096-byte pages, ie 4 gig)
; page granular
; default 32-bit instructions
; present
; privilege level = 00
; is a segment descriptor
; is executable
; is not "conforming"
; is readable
; has not been accessed
        dw 0ffffh      ; limit, bits 15..0
        dw 0000h       ; base, bits 15..0
        db 00h         ; base, bits 23..16
        db 10011010b   ; access byte
        db 11001111b   ; granularity/limit byte
        db 00h         ; base, bits 31..24
; data descriptor
; same as above except for the access byte
        dw 0ffffh
        dw 0000h 
        db 00h   
        db 10010010b   ; not code, goes up, is writable
        db 11001111b
        db 00h

; this gdtinfo:gdtdata will point to the descriptors, declared
; above.        
gdtinfo dw 0017h   ; length (-1) of table (limit)
gdtdata dd ?       ; base address

; this idtinfo:idtdata points to the bottom 256 bytes of
; memory.  At this stage I haven't bothered to fill them with
; anything useful.
idtinfo dw 00ffh   ; length (-1) of table (limit)
idtdata dd 0       ; make it point at 00000000
counter db 0
_DATA   ends
_BSS    segment word public 'BSS'
_BSS    ends
_STACK  segment word stack 'STACK'
        db 1000h dup(?)
_STACK  ends

DGROUP  group   _DATA,_BSS

_TEXT segment word use16 public 'CODE'
        assume cs:_TEXT,ds:DGROUP
start:
        mov ax, DGROUP
        mov ds, ax               

        jmp bypass
final:
        mov ax,0b800h
        mov es,ax
        mov byte ptr es:[2], 'X'
        mov cr0, ebx
;        jmp $+2
;       jmp 8:next
        db 66h
        db 0eah
joffs   dd ?
jseg    dw 8
bypass:          
        mov eax, 0              
        mov ebx, offset DGROUP:descriptors
        mov ax, ds
        shl eax, 4
        add eax, ebx
        mov gdtdata, eax

        mov eax, 0
        mov ebx, offset next
        mov ax, _TEXT32 ;mov ax, cs
        shl eax, 4
        add eax, ebx
        mov joffs, eax

        cli
        db 66h
        lgdt fword ptr gdtinfo
        db 66h
        lidt fword ptr idtinfo
        mov ebx, cr0
        or ebx, 1
        jmp final

_TEXT ends

_TEXT32 segment word use32 public 'CODE'
assume cs:_TEXT32, ds:DGROUP
next:
        mov ax, 0010h 
        mov ss, ax
        mov gs, ax
        mov fs, ax
        mov es, ax
        mov ds, ax

        xor esi,esi          ; Use esi as address fixup
        mov si,DGROUP
        shl esi,4

        mov ah,00000100b     ; Video attribute
        cld
again:                       ; Common mistake
        mov ecx,80 * 25      ; Size of screen in words must use ECX as we're
                             ; in 32bit protected mode
        mov edi,0b8000h
;        mov al,[esi+counter] ; Use this just for test of addressing
;        inc [esi+counter]    ; Don't realy need memory for storage

        mov byte ptr [edi],'M'
        
;        rep stosw
        jmp again
        ;mov eax, 0b8000h
        ;mov byte ptr ds:[eax], 'M'
        ;mov byte ptr ds:[eax+2],'M'
        ;mov byte ptr ds:[eax+4],'M'
        ;jmp next

_TEXT32 ends

end start
