#!/usr/bin/perl

use strict;
use warnings;





sub breakWord {
    my($word)=@_;
    my@list;
    while($word=~m/(..........|.*)/g){
	push @list,$1;
    }
    return @list;
}

my($numberOfProcesses, $word)=@ARGV;

my@list=breakWord($word);

for my $i (1..$numberOfProcesses) {
    my$counter=0;
    my$indexFile="rootIndex.dat";
    my$output;
    for my $part (@list) {

	next if $part =~ m/^\s*$/;
	if($counter>0) {
	    $indexFile=sprintf "$i/$indexFile%09d.dat", $output;
	}
	$counter++;
	$output=`./superindexer search $i/$indexFile $part`;
	chomp$output;
    }
    next if $output eq "0";

    my$fileName=sprintf "$indexFile%09d.details", $output;

    my@result=`tail -n +2 $i/$fileName`;
    print @result;
}
