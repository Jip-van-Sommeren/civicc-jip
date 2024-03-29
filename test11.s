_fun0__foo_foo
    isrg
    jsre 0
    isrl
    jsr0 _fun1_foo_bar
    isrg
    jsr     0 _fun5_baz
    return
    
_fun1__foo_bar_bar
    isrg
    jsre 0
    isrl
    jsr0 _fun2_bar_bar
    isrg
    jsr     0 _fun5_baz
    isrl
    jsr0 _fun3_foobar_foobar
    return
    
_fun2__foo_bar_bar_bar
    isrg
    jsre 0
    isrl
    jsr0 _fun3_foobar_foobar
    return
    
_fun3__foo_foobar_foobar
    return
    
_fun4__bar_bar
    isrg
    jsre 0
    return
    
_fun5__baz_baz
    isrg
    jsre 0
    isrg
    jsr     0 _fun4_bar
    return
    
main:
    isrg
    jsr     0 _fun0_foo
    isrg
    jsr     0 _fun4_bar
    isrg
    jsr     0 _fun5_baz
    iloadc_0
    ireturn
    
.const int 2
.const int 3
.const int 4
.const int 5
.exportfun "main" int main
.importfun "printInt" void
