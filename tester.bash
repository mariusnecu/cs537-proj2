#!/bin/bash
suite=$1
tests=`~cs537-1/testing/p2/run-test.sh -list | grep "${suite}-" | sed 's/:.*$//'`
for tst in $tests; do ./run-test.sh $tst; done
