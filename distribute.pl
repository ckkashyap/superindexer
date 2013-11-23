#!/usr/bin/perl

use strict;
use warnings;


my@machines=("no1slu398", "no1slu399", "no1slu400");

my$numberOfMachines=@machines;

my($fileList) = @ARGV;

my$numberOfFiles = `wc -l $fileList`;
($numberOfFiles)=$numberOfFiles=~m/^(\d+)/;

my$numberOfFilesInEachMachine = int ($numberOfFiles / $numberOfMachines);
my$remainder = $numberOfFiles % $numberOfMachines;

my @list=`cat $fileList`;

my$startIndex=0;
my$endIndex=$numberOfFilesInEachMachine-1;
for my $machine (@machines) {
    $endIndex+=$remainder if $machine eq "no1slu400";
    my@l = @list[$startIndex .. $endIndex];
    $startIndex+=$numberOfFilesInEachMachine;
    $endIndex+=$numberOfFilesInEachMachine;


    `ssh root\@$machine rm -rf /data/superindexer-db`;
    `ssh root\@$machine mkdir /data/superindexer-db`;
    open TMP, ">list.txt";
    print TMP @l;
    `scp list.txt root\@$machine:/data/superindexer-db`;
    `scp parallel.pl root\@$machine:/data/superindexer-db`;
    `scp superindexer root\@$machine:/data/superindexer-db`;
    `scp words.pl root\@$machine:/data/superindexer-db`;
    `scp search.pl root\@$machine:/data/superindexer-db`;
    `scp searchserver.pl root\@$machine:/data/superindexer-db`;
    print "Started in $machine\n";
    my $pid=fork;
    if(!$pid) {
	`ssh root\@$machine perl /data/superindexer-db/parallel.pl 20`;
	`ssh root\@$machine perl /data/superindexer-db/searchserver.pl 20`;
	exit;
    }
    print "Finished in $machine\n";  
}

