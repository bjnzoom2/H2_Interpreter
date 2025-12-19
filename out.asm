global main
section .text
main:
    ; Windows x64 requires 16-byte stack alignment. For simple returns this
    ; minimal sequence is fine because there are no calls that require shadow space.
    mov eax, 0 ; return value in EAX (lower 32 bits)
    ret