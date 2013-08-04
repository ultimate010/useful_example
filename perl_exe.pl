#!/usr/bin/perl
#Zaks Wang
#2013-5-12
#perl调用系统命令

@args=("ls","/");
system(@args) ==0 or die "Error:$?\n";

exec('foo') or print STDERR "Couldn't exec foo:$!";

@result=readpipe("ls -l /");
print @result;
