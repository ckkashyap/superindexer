#!/usr/bin/perl

use strict;
use warnings;


my($numberOfProcesses)=@ARGV;


use HTTP::Daemon;
use HTTP::Status;
use CGI;


my $d = HTTP::Daemon->new (LocalPort => 8080, Reuse => 1) || die;
print "Please contact me at: <URL:", $d->url, ">\n";
while (my $c = $d->accept) {
#    unless(fork) {
	while (my $r = $c->get_request) {
	    my $cgi = new CGI($r->url->query);
	    my $res = HTTP::Response->new(RC_OK);
	    my $searchString = $cgi->param('search') || "nothin";

	    my@children;
	    for my $processNumber (1..$numberOfProcesses) {
		my$file;
		open ($file, "perl search.pl $processNumber $searchString | ");
		push @children, $file;
	    }
	    my$result="";
	    for my $processNumber (1..$numberOfProcesses) {
		my$file=$children[$processNumber-1];
		my@list=<$file>;
		$result.=join "", @list;
		open TMP, ">/data/tmp/$processNumber.output";
		print TMP @list;
	    }
	    
	    $res->content($result);
	    $c->send_response($res);
	    $c->close;	
	}
	undef($c);
#	exit;
#    }
}
