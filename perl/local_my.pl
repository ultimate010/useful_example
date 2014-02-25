my: lexically scoped variable
local:Dynamically scoped variable

内部->外部：
（1）my和local都只在一个block里有效，出去就失效；
（2）但是local的变量可以继续在这个block中调用的子程序中存在；
（3）如果有与外界同名的变量，两者在block退出后都不影响外界同名变量；
外部->内部：
（1）外部设置my、local、缺省均队内有效，但是同名变量外部my，在block内部local是不允许的。因为二者在block中调用的子程序中均有效，会冲突。
（2）如果在一个block中有一个my修饰的变量和外界的一个变量同名，而且又需要在这个block中使用外界变量时，两个办法：
第一个办法，用main的package修饰这个变量名，$main::global
第二个办法，用our修饰，our $global，那么该block中接下来出现的所有$global都是外界的global
（3）编写脚本时，注意作用域，防止外部影响内部。
