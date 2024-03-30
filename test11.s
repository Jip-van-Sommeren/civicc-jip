_fun0__foo_foo
    esr 3
    iloadc 0
    istore 1
    iloadc_0
    istore 0
    iloadc_0
    istore 0
_lab3_while:
    iload_0
    iloadc 0
    ilt
branch_f _lab4_end
    iinc_1 0
    isrg
    jsr     0 _fun0_foo
    isrg
    jsr     0 _fun0_foo
    jump _lab3_while
_lab4_end:
_lab4_while:
    iload_0
    iloadc 0
    ilt
branch_f _lab5_end
    iinc_1 0
    isrg
    jsr     0 _fun0_foo
    jump _lab4_while
_lab5_end:
_lab5_while:
    iload_0
    iloadc_0
    igt
branch_f _lab6_end
    idec_1 0
    isrg
    jsr     0 _fun0_foo
    jump _lab5_while
_lab6_end:
    return
    
.const int 10
