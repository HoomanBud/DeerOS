type _load_idt, @function
global _load_idt
_load_idt:
    movl 4(%esp), %eax
    lidt (%eax)
    ret