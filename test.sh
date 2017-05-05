#!/bin/sh                                                                                               

LAST=$1
PASS=true
for ((i=1;i<=$LAST;i++)); do
    ./chess <in"$i".txt > out.txt
    DIFF=$(diff out.txt out"$i".txt)
    if [ "$DIFF" != "" ]; then
       echo "chess: FAILED test $i!"
       PASS=false
       break
    fi
done
if [ $PASS = true ]; then
    echo "chess: PASSED all tests"
fi
