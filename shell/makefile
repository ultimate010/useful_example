object:=${a} a.o b.o c.o#定义变量object，
#但是a变量必须之前定义有效
object=${a} a.o b.o c.o #定义变量object,
${a}=aaaa #a可以是后面定义
object?=abc #如果object之前已经定义，这条语句什
#么都不做，否则定义object为abc
foo := a.o b.o c.o
bar := $(foo:.o=.c)
#这个示例中，我们先定义了一个“$(foo)”变量，而第二行的意思是把“$(foo)”中所
#有以“.o”字串“结尾”全部替换成“.c”，所以我们的“$(bar)”的值就是“a.c b.c
#c.c”。
