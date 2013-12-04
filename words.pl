#!/usr/bin/perl

use strict;
use warnings;

use Time::HiRes qw/ time sleep /;;


sub breakWord {
    my($word)=@_;
    my@list;
    while($word=~m/(..........|.*)/g){
	push @list,$1;
    }
    return @list;
}

my $fn=$ARGV[0];

my$type=`file "$fn" | grep text`;
exit unless $type;

print "OK\n";
exit;

$fn=~m#.*/(.*)#;
my$bn=$1;


open FILE, $fn;

my%lookup = (
    "for" => 1,
    "while" => 1,
    "include" => 1,
    );

my %goodWords = ($bn=>1);
while(<FILE>) {
    next if /^\s*$/;
    my@words=split/[^a-zA-Z0-9_]/;
    my@properWords = grep {(3 <= length $_) && (100 >= length $_)} grep {!$lookup{$_}} grep {m/^\S+$/} @words;
    next unless @properWords;
    for my $word (@properWords) {
	next if $word =~ m/^\s*$/;
	$goodWords{lc $word}=1;
    }
}

my$l = keys %goodWords;


for my $word (keys %goodWords) {
    my$indexFile="rootIndex.dat";
    my @wordList = breakWord($word);
    my $output;
    my $counter=0;
    for my $part (@wordList) {
	next if $part =~ m/^\s*$/;
	if($counter>0) {
	    $indexFile=sprintf "$indexFile%09d.dat", $output;
	}
	$counter++;
	$output=`../superindexer add $indexFile $part`;
	chomp$output;

    }
    my$fileName=sprintf "$indexFile%09d.details", $output;
    if (-e $fileName) {
	open FILE, ">>$fileName";
	print FILE $fn,"\n";	    
    } else {
	open FILE, ">$fileName";
	print FILE $word,"\n";
	print FILE $fn,"\n";	    	
    }
}

