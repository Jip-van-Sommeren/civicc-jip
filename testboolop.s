test:
    iload_0
    iloadc_0
    ieq
    branch_f _lab1_end
    iloadc 0
    istore 0
_lab1_end:
    iload_0
    isrg
    iload_0
    iloadc_1
    isub
    jsr 1 _fun0_factorial
    ipop
    imul
    ireturn
    
_fun0_factorial:
    iload_0
    iloadc_0
    ieq
    branch_f _lab2_end
    iloadc_1
    ireturn
_lab2_end:
    iload_0
    isrg
    iload_0
    iloadc_1
    isub
    jsr 1 _fun0_factorial
    ipop
    imul
    ireturn
    
main:
    esr 1
    iloadc_0
    istore 0
_lab2_while:
    iload_0
    iloadc 1
    ilt
branch_f _lab3_end
    iinc_1 0
    isrg
    isrg
    iload_-1
    jsr 1 _fun0_factorial
    ipop
    jsre 0
    isrg
    iloadc_1
    jsre 2
    jump _lab2_while
_lab3_end:
    isrg
    iloadc_1
    jsre 1
    iloadc_0
    ireturn
    
.const int 6
.const int 2
.exportfun "test" int test
.exportfun "main" int main
.importfun "printInt" void int
.importfun "printNewlines" void int
.importfun "printSpaces" void int
