#!/usr/bin/perl

use strict;
use warnings;


my($numberOfProcesses)=@ARGV;


sub breakWord {
    my($word)=@_;
    my@list;
    while($word=~m/(..........|.*)/g){
	push @list,$1;
    }
    return @list;
}


my ($word)=@ARGV[0];

my@list=breakWord($word);


my$counter=0;
my$indexFile="rootIndex.dat";
my$output;
for my $part (@list) {

    next if $part =~ m/^\s*$/;
    if($counter>0) {
	$indexFile=sprintf "$indexFile%09d.dat", $output;
    }
    $counter++;
    $output=`../superindexer add $indexFile $part`;
    chomp$output;
}

my$fileName=sprintf "$indexFile%09d.details", $output;

my@result=`cat $fileName`;
print @result;
