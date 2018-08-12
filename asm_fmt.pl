#!/usr/bin/perl -w

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
