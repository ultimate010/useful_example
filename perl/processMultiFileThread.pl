#!/usr/bin/perl
use threads;
use Thread::Queue;
use Encode;


$MAXPROCESS = 16;
my $myQueue = Thread::Queue->new();
my $logQueue = Thread::Queue->new();
my $startQueue = Thread::Queue->new(); #开始log记录


if(@ARGV != 2){
  die "Usage Inputpath outPath\n";
}

sub isWriting{
  my $file = shift;
  my $command = "fuser $file | wc -l ";
  my @result = readpipe($command);
  if(@result != 0){
    return 0;
  }else{
    return 1;
  }
}

#每隔10秒扫描目录,发现新的文件,输出
sub produce{
  my @par = @_;
  $totalFileCount = 0;
  $command = "find  $par[1] -name miniblog* -type f -print";
  my %tempHash = ();
  open(in,"./log.processed"); #or die "Can not open log\n";
  while($line = <in>){
    chomp $line;
    $line = decode("utf8",$line);
    $tempHash{$line}++; #放入已经处理的文件
  }
  close(in);
  print "Load processed done\n";
  $startQueue->enqueue("abc");
  open(out,">>./log.processed");
  while(1){
    @result = readpipe($command);
    if($totalFileCount < @result){
      #有新的文件
      foreach $file (@result){
	chomp $file;
	#判断文件是否正在写入
	if(isWriting($file)){
	  next;
	}
	$totalFileCount++;
	if(defined $tempHash{$file}){
	}else{
	  $tempHash{$file}++;
	  $myQueue->enqueue($file);
	  print out "$file\n";
	  print "Add Process $file\n";
	}
      }
    }
    sleep(10);
  }
  close(out);
}

sub consume{
  my @par = @_;
  while(my $file = $myQueue->dequeue()){
    #Process new file
    my $LinkCount = 0;
    my %HashLink = ();
    open(in,"$file") or die "$par[0]:\tCan not open $file \n";
    while($line = <in>){
      chomp $line;
      $line = decode("utf8",$line);
      if($line =~/^\@FILTER:(.*)/){
	#print $1."\n";
	$num = $1;
	if($num & 0x100){
	  $skip = 1;
	}else{
	  $skip = 0;
	}
      }
      if($line =~/^\@LURLS:(.+)/){
	if($skip == 1){
	  next;
	}
	$urls = $1;
	@arr = split(/[ \t]+/,$1);
	foreach $url (@arr){
	  $LinkCount++;
	  $HashLink{$url}++;
	}
      }
    }
    close(in);
    my $processed = $file;
    #print $file."\n";
    if($file =~/.*\/([^\/]+)$/){
      $file = $1;
      #print "$par[0]:\tNew $file\n";
      #<stdin>;
    }
    open(out,">$par[1]/$file") or die "$par[0]:\tCan not open $par[1]/$file\n";
    print out encode("utf8","$file\t$LinkCount\n");
    foreach $key ( sort { $HashLink{$b} <=> $HashLink{$a} } keys %HashLink){
      print out encode("utf8","$key\t$HashLink{$key}\n");
    }
    close(out);
    $logQueue->enqueue($processed); #已经处理过的文件
  }
}


sub logger{
  while(my $abc = $startQueue->dequeue()){
    print "Start logger\n";
    my @par = @_;
    open(out,">>./log.processed") or die "Can not open logger\n";
    while(my $file = $logQueue->dequeue()){
      print out "$file\n";
    }
    close(out);
  }
}

#start threads
print "Start producer\n";
my $producer = threads->create(\&produce,"Producer",$ARGV[0]);
for($i = 0;$i < $MAXPROCESS;$i++){
  print "Start consumer$i\n";
  push @consumers , threads->create(\&consume,"Consumer$i","$ARGV[1]");
}

my $log = threads->create(\&logger,"Logger");

foreach $consumer(@consumers){
  $consumer->join();
}
$producer->join();
$log->join();


