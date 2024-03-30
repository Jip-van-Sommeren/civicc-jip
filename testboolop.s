_fun0_printBool:
    bload_0
    branch_f _lab0_else
    isrg
    iloadc_1
    jsre 0
    jump _lab1_end
_lab0_else:
    isrg
    iloadc_0
    jsre 0
_lab1_end:
    isrg
    iloadc_1
    jsre 1
    return
    
_fun1_t:
    isrg
    iloadc_1
    jsre 0
    bloadc_t
    breturn
    
_fun2_f:
    isrg
    iloadc_0
    jsre 0
    bloadc_f
    breturn
    
main:
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun2_f
    beq
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun1_t
    beq
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun1_t
    beq
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun2_f
    beq
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun2_f
    bne
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun1_t
    bne
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun1_t
    bne
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun2_f
    bne
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    isrg
    jsr 0 _fun1_t
    branch_f _lab1_otherwise
    isrg
    jsr 0 _fun2_f
    jump _lab2_end
_lab1_otherwise:
    bloadc_f
_lab2_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    branch_f _lab3_otherwise
    isrg
    jsr 0 _fun1_t
    jump _lab4_end
_lab3_otherwise:
    bloadc_f
_lab4_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    branch_f _lab5_otherwise
    isrg
    jsr 0 _fun1_t
    jump _lab6_end
_lab5_otherwise:
    bloadc_f
_lab6_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    branch_f _lab7_otherwise
    isrg
    jsr 0 _fun2_f
    jump _lab8_end
_lab7_otherwise:
    bloadc_f
_lab8_end:
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    isrg
    jsr 0 _fun1_t
    branch_f _lab9_otherwise
    bloadc_t
    jump _lab10_end
_lab9_otherwise:
    isrg
    jsr 0 _fun2_f
_lab10_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    branch_f _lab11_otherwise
    bloadc_t
    jump _lab12_end
_lab11_otherwise:
    isrg
    jsr 0 _fun1_t
_lab12_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    branch_f _lab13_otherwise
    bloadc_t
    jump _lab14_end
_lab13_otherwise:
    isrg
    jsr 0 _fun1_t
_lab14_end:
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    branch_f _lab15_otherwise
    bloadc_t
    jump _lab16_end
_lab15_otherwise:
    isrg
    jsr 0 _fun2_f
_lab16_end:
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun2_f
    bmul
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun1_t
    bmul
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun1_t
    bmul
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun2_f
    bmul
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun2_f
    badd
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun1_t
    badd
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun1_t
    isrg
    jsr 0 _fun1_t
    badd
    jsr 1 _fun0_printBool
    isrg
    isrg
    jsr 0 _fun2_f
    isrg
    jsr 0 _fun2_f
    badd
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    iloadc 0
    iloadc_0
    ine
    branch_f _lab17_otherwise
    bloadc_t
    jump _lab18_end
_lab17_otherwise:
    bloadc_f
_lab18_end:
    jsr 1 _fun0_printBool
    isrg
    iloadc_0
    iloadc_0
    ine
    branch_f _lab19_otherwise
    bloadc_t
    jump _lab20_end
_lab19_otherwise:
    bloadc_f
_lab20_end:
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    isrg
    floadc 1
    floadc_0
    fne
    branch_f _lab21_otherwise
    bloadc_t
    jump _lab22_end
_lab21_otherwise:
    bloadc_f
_lab22_end:
    jsr 1 _fun0_printBool
    isrg
    floadc_0
    floadc_0
    fne
    branch_f _lab23_otherwise
    bloadc_t
    jump _lab24_end
_lab23_otherwise:
    bloadc_f
_lab24_end:
    jsr 1 _fun0_printBool
    isrg
    iloadc_1
    jsre 1
    iloadc_0
    ireturn
    
.const int 5
.const float 5.000000
.exportfun "main" int main
.importfun "printInt" void int
.importfun "printNewlines" void int
