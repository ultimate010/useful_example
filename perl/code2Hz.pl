#!/usr/bin/perl
use Encode;
$abc = '\266\340\265\304';
@arr = $abc =~/\d+/g;
for($i = 0; $i < @arr; $i += 2){
  $a = $arr[$i];
  $b = $arr[$i+1];
  $d = sprintf("%x",(oct($a))) . sprintf("%x",(oct($b)));
  print encode("utf8",decode("gbk",pack("H*",$d)));
}



