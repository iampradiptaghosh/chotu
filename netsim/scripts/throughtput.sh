#!/bin/sh
FIRST_LINE=$(head -n 1 $1.send)
LAST_LINE=$(tail -n 1 $1.send)
START=$(echo $FIRST_LINE | awk 'BEGIN{ FS="("}{print $1}')
END=$(echo $LAST_LINE | awk 'BEGIN{ FS="("}{print $1}')
SENT_DATA=$(expr $(echo $LAST_LINE | awk 'BEGIN{ FS=":"}{print $3}' | awk 'BEGIN{ FS="/"}{print $3}' ) - 1 )
DURATION=$(expr $END - $START)
BYTE_PER_SEC=$(expr $SENT_DATA / $(expr $DURATION / 1000000))
KBPS=$(expr $(expr $BYTE_PER_SEC \* 8) / 1000)
echo "START     : $START"
echo "END       : $END"
echo "DURATION  : $DURATION"
echo "DATA      : $SENT_DATA bytes"
echo "TROUTHPUT : $KBPS Kbps"
