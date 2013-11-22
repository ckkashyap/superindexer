#!/usr/bin/perl

use strict;
use warnings;


chdir "/data/superindexer-db";

my@list=`cat list.txt`;


my($numberOfProcesses) = @ARGV;

my$numberOfFiles = `wc -l list.txt`;
($numberOfFiles)=$numberOfFiles=~m/^(\d+)/;

my$numberOfFilesInEachProcess = int ($numberOfFiles / $numberOfProcesses);
my$remainder = $numberOfFiles % $numberOfProcesses;

my$startIndex=0;
my$endIndex=$numberOfFilesInEachProcess-1;
for my $process (1..$numberOfProcesses) {
    $endIndex+=$remainder if $process eq $numberOfProcesses;
    my@l = @list[$startIndex .. $endIndex];
    $startIndex+=$numberOfFilesInEachProcess;
    $endIndex+=$numberOfFilesInEachProcess;
    print "@l";
    print "-"x10,"\n";

    my $pid=fork;

    if(!$pid) {
	mkdir $process;
	chdir $process;
	for my $f (@l) {
	    chomp $f;
	    `perl ../words.pl $f`;
	}
	`touch DONE`;
	exit;
    }   
    
}

