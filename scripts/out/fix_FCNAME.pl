#!/usr/bin/perl
use strict;
our $idx=0;

sub patch {
    my ($f) = @_;
    $idx++;
    system "mv $f /tmp/_$idx.tmp";
    my $cnt = 0;
    open Out, ">$f" or die "Can't write $f.\n";
    open In, "/tmp/_$idx.tmp" or die "Can't open /tmp/_$idx.tmp.\n";
    while(<In>){
        if(/FCNAME/ && !/#(define|undef)/){
            s/FCNAME/__func__/g;
            $cnt++;
        }
        print Out $_;
    }
    close In;
    close Out;
    system "rm /tmp/_$idx.tmp";
    return $cnt;
}

my $root = "$ENV{HOME}/work/mpich-github";
chdir $root or die;
my $grep_cmd="find . -name '*.[ch]' |xargs grep FCNAME |grep -v ':#'";
my %files;
open In, "$grep_cmd |" or die "Can't open $grep_cmd |.\n";
while(<In>){
    if(/^\.\/(src\S*):/){
        $files{$1}++;
    }
}
close In;
my @files = sort keys %files;
foreach my $f (@files){
    print "patching $f... ";
    my $cnt = patch($f);
    print "$cnt lines changed\n";
}
