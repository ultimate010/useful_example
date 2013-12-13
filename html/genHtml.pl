#!/usr/bin/perl
$str =  '<li><img src="SSS" rel="group2" data-glisse-big="MMM" class="tl" title="Photo by Angry pig, for ZJY"></li>';
$path = "./picture_m";
$pathS = "./picture_s/";
$cmd = "find $path/*";
$cmdS = "find $pathS/*";
@arr = readpipe($cmd);
@arrS = readpipe($cmdS);
for($i = 0;$i < @arr;$i++){
  $key = $arr[$i];
  $keyS = $arrS[$i];
  chomp $key;chomp $keyS;
  $tmpStr = $str;
  $tmpStr =~s/MMM/$key/;
  $tmpStr =~s/SSS/$keyS/;
  print $tmpStr."\n";
}

