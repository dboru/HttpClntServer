#!/bin/bash

for i in {1..20}; 
do 
cat test test > test2 && mv test2 test ;
done

#perl -e 'print "a" x 1024' > foo1K.txt
#perl -e 'print "a" x 10240' > foo10K.txt
#perl -e 'print "a" x 102400' > foo100K.txt
#perl -e 'print "a" x 512000' > foo500K.txt
#perl -e 'print "a" x 1048576' > foo1M.txt
#perl -e 'print "a" x 10485760' > foo10M.txt

