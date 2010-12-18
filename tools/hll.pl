#!/usr/bin/perl -w

################################################################################
# HLL - Heuristic Lint issue Locator.
#
# Prints descriptions for given lint error numbers by extracting parts from
# PC-Lint's 'msg.txt' file.
#
# Written 2009 by Ralf Holly (ralf.holly_at_approxion.com).
# You can freely use and redistribute this script; use it at your own risk!
#
# 2009-07-21 V1.10 Small bugfix related to multi-matching (issue number regex)
###############################################################################

use strict;
use warnings;

my $msgfile = shift @ARGV;
&help unless defined $msgfile and -e $msgfile;

while (my $issue_number = shift @ARGV) {
    open (MSG_FILE, "<$msgfile") or die "Cannot open $msgfile\n";
    my $found = 0;
    while(<MSG_FILE>) {
        # If lint issue nummer found
        if (m/^(\d+)\s+\S+/) {
            # If searched-for issue found
            if ($1 =~ /\b$issue_number\b/) {
                    $found = 1;
            # If end of searched-for issue message; that is
            # beginning of a new lint issue
            } else {
                $found = 0;
            }
        }
   
        if ($found) {
            print $_;
        }
    }
    close (MSG_FILE);
}

sub help {
print <<HERE;
HLL - Heuristic Lint issue Locator.

Prints descriptions for given lint error numbers by extracting parts from
PC-Lint's 'msg.txt' file.
Written 2009 by Ralf Holly (ralf.holly_at_approxion.com).
You can freely use and redistribute this script; use it at your own risk!

Usage:
  hll <msg-file> [number] ...
  msg-file:
      Path to PC-Lint's 'msg.txt' file
  number:
      Lint error number (Perl regular expressions supported)
Example:
    hll.pl c:\\lint\\msg.txt 407 609
    hll.pl c:\\lint\\msg.txt 5.. 60[0-9]
Hint:
    In order to save passing the path to your 'msg.txt' file, invoke HLL
    from a shell script, batch file or via an abbreviation.
HERE
 exit 1;
}

