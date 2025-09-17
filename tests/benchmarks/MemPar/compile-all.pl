#!/usr/bin/perl

# Visits every directory, calls make, and then executes the benchmark
# (Designed for making sure every kernel compiles/runs after modifications)
#
# Written by Tomofumi Yuki, 01/15 2015
#

my $TARGET_DIR = ".";

if ($#ARGV != 0 && $#ARGV != 1) {
   printf("usage perl compile-all.pl target-dir\n");
   exit(1);
}



if ($#ARGV >= 0) {
   $TARGET_DIR = $ARGV[0];
}

my $OUTFILE = "";
if ($#ARGV == 1) {
   $OUTFILE = $ARGV[1];
}


my @categories = ('cnn_test',
                  'dotproduct_test',
                  'fft_test',
                  'spmv_test',
                  'stream_test',
                  'topk_test',
                  'hist_test');


foreach $cat (@categories) {
   my $target = $TARGET_DIR.'/'.$cat;
   opendir DIR, $target or die "directory $target not found.\n";
   my $kernel = $cat;
   my $targetDir = $target.'/'.$dir;
   my $command = "cd $targetDir; make clean; make;";
    # Check if Makefile exists before running make
   if (-f "$targetDir/Makefile" || -f "$targetDir/makefile") {
       my $command = "cd $targetDir; make clean; make;";
       print($command."\n");
       system($command);
   } else {
       print("No Makefile found in $targetDir, skipping...\n");
   }
   closedir DIR;
}

