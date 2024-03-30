main:
    esr 5
    iloadc 0
    istore 4
    iloadc_0
    istore 3
    iloadc 0
    istore 0
    iloadc 1
    istore 1
    iloadc 1
    istore 2
_lab0_while:
    iload_0
    iload_1
    ilt
    branch_f _lab1_end
    iinc_1 3
    idec_1 0
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 2
    jsre 4
    jump _lab0_while
_lab1_end:
    isrg
    iloadc_1
    jsre 5
    iloadc 1
    istore 1
_lab2_while:
    iload_0
    iload_1
    igt
    branch_f _lab3_end
    idec_1 4
    idec_1 0
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 2
    jsre 4
    jump _lab2_while
_lab3_end:
    isrg
    iloadc_1
    jsre 5
    iloadc_0
    ireturn
    
.const int 10
.const int 5
.const int 3
.exportfun "main" int main
.importfun "printInt" void int
.importfun "printFloat" void float
.importfun "scanInt" int
.importfun "scanFloat" float
.importfun "printSpaces" void int
.importfun "printNewlines" void int
