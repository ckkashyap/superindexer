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

my$processNummber = shift;

my %result;
my $firstTime=1;
for my $word (@ARGV) {

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
	$output=`./superindexer search $processNummber/$indexFile $part`;
	chomp$output;
    }
    exit if $output eq "0";

    my$fileName=sprintf "$indexFile%09d.details", $output;

    my@res=`tail -n +2 $processNummber/$fileName`;
    my $num = @res;
    if ($firstTime) {
	$firstTime=0;
	for(@res) {
	    $result{$_}=1;
	}
    }else{
	my%temp;
	for(@res) {
	    $temp{$_}=1;
	}
	for(keys %result) {
	    delete $result{$_} unless $temp{$_};
	}
    }

}


for (keys %result) {
    print;
}
