#!/usr/bin/perl

use strict;
use warnings;



while (<>){
    chomp;
    my$inputFile=$_;
    m#/data/P4/Main(.*)#;
    my$input=$1;
    my($path, $name)=$input=~m#(.*/)(.*)#;
    print "Path = $path, name = $name\n";
    `mkdir -p /data/SyntaxHighlighted$path`;
    my$output="/data/SyntaxHighlighted$path/$name.html";
    `echo "~~~~ {#mycode .cpp .numberLines } " > "/tmp/$name"` ;
    `cat "$inputFile" >> "/tmp/$name"`;
    `echo "~~~~" >> "/tmp/$name"` ;
    `pandoc "/tmp/$name" -s --highlight-style haddock -o "$output"`;
    unlink "/tmp/$name";
}
