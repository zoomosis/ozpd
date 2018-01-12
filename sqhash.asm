; SQHASH.ASM

; Creates a Squish hash value for use with the Squish messagebase
; format.

; NOTE: Uses 32 bit registers, therefore requires 386+ processor

; Original code written 24-Aug-1995.
; Author: Rowan Crowe, 3:635/727@fidonet
;                      rowan@jelly.freeway.dialix.oz.au

;        * Unconditionally released to the public domain *


; This is the algorithm from SQDEV.PRN:
;
; ===================================================================
; Set "hash" to a value of 0
;
; For each 8-bit character "ch" in the To: field, repeat:
;
;     -    Shift "hash" left by four bytes.
;     -    Convert "ch" to lowercase
;     -    Increment the hash by the ASCII value of "ch"
;
;     -    Set "g" to the value of "hash"
;     -    Perform a  bitwise AND  on  "g", using  a mask  of
;          0xf0000000.
;
;     -    If "g" is non-zero:
;
;          -    Perform a bitwise OR on "hash" with the value
;               of "g".
;          -    Shift "g" right by 24 bits.
;          -    Perform a bitwise OR on "hash" with the value
;               of "g".
;
; Perform a bitwise AND on "hash" with a value of 0x7fffffff.
; ===================================================================

.386
CODE SEGMENT
ASSUME CS:CODE

_SquishHash PROC Far
; DS:SI -> string to hash
;    CX  = length of string
; Returns:
;    EAX = hash value

; -- Conversion reference --
;  EAX = "hash"
;  EBX = "ch"
;  EDX = "g"
; --------------------------

        xor   eax, eax        ; Set "hash" to a value of 0
@HashLoop:
        movzx ebx, byte ptr ds:[si]
        inc   si
        shl   eax, 4          ; Shift "hash" left by four bytes.
        cmp   bl, 'A'
        jb    @KeepCase
        cmp   bl, 'Z'
        ja    @KeepCase
        add   bl, 32          ; Convert "ch" to lowercase
@KeepCase:
        add   eax, ebx        ; Increment the hash by the ASCII value
                              ; of "ch"
        mov   edx, eax        ; Set "g" to the value of "hash"
        and   edx, 0f0000000h ; Perform a bitwise AND on "g", using a
                              ; mask of 0xf0000000.
        jz    @HashCont       ; If "g" is non-zero:
        or    eax, edx        ; Perform a bitwise OR on "hash" with
                              ; the value of "g".
        shr   edx, 24         ; Shift "g" right by 24 bits.
        or    eax, edx        ; Perform a bitwise OR on "hash" with
                              ; the value of "g".
@HashCont:
        and   eax, 07fffffffh ; Perform a bitwise AND on "hash" with
                              ; a value of 0x7fffffff.

        loop  @HashLoop
        ret
ENDP

CODE ENDS
END

