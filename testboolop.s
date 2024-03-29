_fun0__printBool_printBool
    bload_0
    branch_f _lab1_else
    isrg
    jsre 0
    jump _lab2_end
_lab1_else:
    isrg
    jsre 0
_lab2_end:
    isrg
    jsre 1
    return
    
_fun1__t_t
    isrg
    jsre 0
    bloadc_t
    breturn
    
_fun2__f_f
    isrg
    jsre 0
    bloadc_f
    breturn
    
main:
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsr     1 _fun0_printBool
    isrg
    jsre 1
    iloadc_0
    ireturn
    
.const int 5
.const float 5.000000
.exportfun "main" int main
.importfun "printInt" void
.importfun "printNewlines" void
