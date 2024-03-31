_fun5_bor:
    isrl
    jsr 0 _fun6_bor_baz_inner
    return
    
_fun6_bor_baz_inner:
    isrl
    jsr 0 _fun7_bor_baz_inner_b
    return
    
_fun7_bor_baz_inner_b:
    isrg
    iloadc 0
    jsre 0
    isrg
    iloadc_1
    jsre 2
    return
    
main:
    esr 2
    iloadc 3
    istore 0
    iload_0
    istore 1
    isrl
    iload_0
    jsr 1 _fun0_main_bar
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrg
    iload_0
    jsr 1 _fun3_baz
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrg
    iload_0
    jsr 1 _fun5_bor
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrl
    jsr 0 _fun1_main_boz
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrg
    iload_1
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrl
    jsr 0 _fun2_main_biz
    isrg
    iload_0
    jsre 0
    isrg
    iloadc 4
    jsre 2
    isrg
    iload_1
    jsre 0
    isrg
    iloadc_1
    jsre 2
    iloadc_0
    ireturn
    
_fun0_main_bar:
    iloadc 1
    istore 0
    isrg
    iload_0
    jsre 0
    isrg
    iloadc_1
    jsre 2
    return
    
_fun1_main_boz:
    esr 1
    iloadn 1 0
    istore 0
    iinc_1 0
    isrg
    iload_0
    jsre 0
    isrg
    iloadc_1
    jsre 2
    return
    
_fun2_main_biz:
    iloadc 2
    istoren 1 0
    return
    
_fun3_baz:
    isrl
    jsr 0 _fun4_baz_baz_inner
    return
    
_fun4_baz_baz_inner:
    esr 1
    iloadc 5
    istore 0
    isrg
    iload_0
    jsre 0
    isrg
    iloadc_1
    jsre 2
    return
    
.const int 33333
.const int 11111
.const int 55555
.const int 123
.const int 2
.const int 22222
.exportfun "main" int main
.importfun "printInt" void int
.importfun "printSpaces" void int
.importfun "printNewlines" void int
