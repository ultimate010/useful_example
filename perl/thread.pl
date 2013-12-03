#!/usr/bin/perl
use threads;
use Thread::Queue;


$MAXPROCESS = 10;
my $myQueue = Thread::Queue->new();


sub produce{
  my $name = shift;
  while(1){
    my $r  = int(rand(100));
    $myQueue->enqueue($r);
    printf("$name produce $r\n");
    sleep(1);
  }
}

sub consume{
  my @par = @_;
  while(my $r = $myQueue->dequeue()){
    printf("$par[0]\t$par[1]\t$r\n");
  }
}

my $producer1 = threads->create(\&produce,"producer");

for($i = 0;$i < $MAXPROCESS;$i++){
  my $consumer = threads->create(\&consume,"Consumer$i","path");
  $consumer->join();
}
$consumer->join();

