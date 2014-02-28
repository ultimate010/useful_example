#!/usr/bin/perl
use strict;
use warnings;
use Encode;
use utf8;
#use open ":encoding(gbk)", ":std"; 用这个可以全局替换

open(my $in,"<:encoding(utf8)","newClass.txt");
open(my $out, ">:encoding(utf-8)","newOut.txt");
my @arr = ();

sub output{
    if(scalar @arr <= 1){
    }elsif(scalar @arr <= 2){
        pop @arr;
        my $str = join("#",@arr);
        print $out "$str\n";
    }else{
        pop @arr; pop @arr;
        my $str = join("#",@arr);
        print $out "$str\n";
    }
}

while(my $line = <$in>){
    chomp $line;
    $line =~s/^\s+|\s+$//g;
    if($line =~/^\d+:\>?(.*)/){
        #输出
        output();
        @arr = ();
        my $word = "";
        $word = $1;
        $word =~s/^\s+|\s+$//g;
        if(length($word) != 0){
            push(@arr,$word);
        }
    }else{
        #读取数据
        if(length($line) != 0){
            my $word = "";
            if($line =~/^\>(.*)/){
                $word = $1;
            }else{
                $word = $line;
            }
            $word =~s/^\s+|\s+$//g;
            if(length($word) != 0){
                push(@arr,$word);
            }
        }
    }
}

output();

close($out);
close($in);
