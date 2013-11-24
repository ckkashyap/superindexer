#!/usr/bin/perl

use strict;
use warnings;


#my($numberOfProcesses)=@ARGV;


use HTTP::Daemon;
use HTTP::Status;
use CGI;
use Time::HiRes qw/ time sleep /;


my $d = HTTP::Daemon->new (LocalPort => 9090, Reuse => 1) || die;
print "Please contact me at: <URL:", $d->url, ">\n";
while (my $c = $d->accept) {
#    unless(fork) {
	while (my $r = $c->get_request) {
	    my$startTime=time;
	    my $cgi = new CGI($r->url->query);
	    my $res = HTTP::Response->new(RC_OK);
	    my $searchString = $cgi->param('search') || "nothin";
	    my $prefix = $cgi->param('prefix') || "";
	    $searchString=~s/ /%20/g;


	    my@children;
	    for my $machine ("no1slu398", "no1slu399", "no1slu400") {
		my$file;
		open ($file, "curl http://$machine:8080/?search=$searchString | ");
		push @children, $file;
	    }
	    my@result;
	    my$result="";
	    my$count=0;
	    my$resultCounter=0;
	    for my $machine ("no1slu398", "no1slu399", "no1slu400") {
		my$file=$children[$count];
		$count++;
		my@list=<$file>;
		for(@list){
		    chomp;
		    s#/data/P4/Main/##;
		    my$path=$_;
		    $resultCounter++;
		    $_=qq(<a href="http://no1slu399/acrobat/$path.html">$resultCounter> $path</a><br>\n );
		    		    
		}
		push @result, @list;
	    }
	    my$endTime=time;
	    my$diff=$endTime-$startTime;
	    my$diffString=sprintf "$resultCounter results in %0.3f seconds\n<br>", $diff;
	    $result=join "", @result;
	    $res->content("<html>".$diffString.$result."</html>");
	    $c->send_response($res);
	    $c->close;	
	}
	undef($c);
#	exit;
#    }
}
