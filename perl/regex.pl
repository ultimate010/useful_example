PERL支持PERL4和标准UNIX模式匹配操作所没有的一些模式匹配能力。其语法为：(?pattern)，其中c是一个字符，pattern是起作用的模式或子模式。
1、不存贮括号内的匹配内容
在PERL的模式中，括号内的子模式将存贮在内存中，此功能即取消存贮该括号内的匹配内容，如/(?:a|b|c)(d|e)f\1/中的\1表示已匹配的d或e，而不是a或b或c。
2、内嵌模式选项
通常模式选项置于其后，有四个选项：i、m、s、x可以内嵌使用，语法为：/(?option)pattern/，等价于/pattern/option。
3、肯定的和否定的预见匹配
肯定的预见匹配语法为/pattern(?=string)/，其意义为匹配后面为string的模式，相反的，(?!string)意义为匹配后面非string的模式，如：
$string = "5abc343";
$string =~ /abc(?=[0-9])/;
已匹配的模式，此处为abc，而不是abc8
4、模式注释
PERL5中可以在模式中用?#来加注释，如：
if $string =~ /(?i)[a-z]{2,3}(?# match two or three alphabeticcharacters)/( {
...
}
