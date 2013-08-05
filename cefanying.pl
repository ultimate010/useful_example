#!/usr/bin/perl
use strict;

main();
sub main{
  my $avgTime = 0;
  my $testCount = 0;
  my $wrongCount = 0;
  while(1){
    my $a = int(rand(10));
    my $b = int(rand(10));
    my $c = int(rand(10));
    my $op1 = int(rand(2));
    my $op2 = int(rand(2));
    my $sum = $a;
    print "$a ";
    if($op1 == 0){
      print " + ";
      $sum += $b;
    }else{
      print " - ";
      $sum -= $b;
    }
    print "$b";
    if($op2 == 0){
      print " + ";
      $sum += $c;
    }else{
      print " - ";
      $sum -= $c;
    }
    print "$c = ?\n";
    print "Please input \n";
    my $time0 = time;
    my $line = <>;
    chomp $line;
    my $time1 = time;
    if($sum == $line){
      print "You are right\n";
    }else{
      print "You are wrong\n";
      print "ans = $sum\n";
      print "Time plus 3 second\n";
      $time1 += 3;
      $wrongCount++;
    }
    $time1 -= $time0;
    print "Time: $time1 second\n";
    $avgTime += $time1;
    $testCount++;
    print "Continue (q for quit)\n";
    $line = <>;
    chomp $line;
    if($line eq 'q'){
      last;
    }
  }
  print "You taken $testCount test\nTotal time is $avgTime second\n";
  $avgTime /= $testCount;
  print "Avg time is $avgTime second\n";
  print "Wrong times $wrongCount\n";
  $wrongCount*=3;
  print "Total plus time $wrongCount\n";
}
