#!/usr/bin/perl -w

# Takes an asm file and pads all // comments out to 40chars
while(<>) {
        if(m/^(.+?)\s*(\/\/ .*)?$/) {
                my ($code, $comment) = ($1, $2);

                if($comment) {
                        printf "%- 40s %s\n", $code, $comment;
                }
                else {
                        print "$code\n";
                }
        }
        else {
                print;
        }
}
