_fun0_factorial:
    iload_0
    iloadc_0
    ieq
    branch_f _lab1_end
    iloadc_1
    ireturn
_lab1_end:
    iload_0
    isrg
    iload_0
    iloadc_1
    isub
    jsr 1 _fun0_factorial
    imul
    ireturn
    
main:
    esr 1
    iloadc_0
    istore 0
_lab1_while:
    iload_0
    iloadc 0
    ilt
    branch_f _lab2_end
    isrg
    isrg
    iload_0
    jsr 1 _fun0_factorial
    jsre 0
    isrg
    iloadc_1
    jsre 2
    iinc_1 0
    jump _lab1_while
_lab2_end:
    isrg
    iloadc_1
    jsre 1
    iloadc_0
    ireturn
    
.const int 6
.exportfun "main" int main
.importfun "printInt" void int
.importfun "printNewlines" void int
.importfun "printSpaces" void int
