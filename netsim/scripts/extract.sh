#!/bin/sh

ShowUsage()
{
        echo "Usage :"
        echo "----------------------------------------------------------------------"
        echo " $ ./extract.sh FILE NODE"
        echo "----------------------------------------------------------------------"

	echo " For example, "
	echo " The following steps will generates the outputs for node 1 of testcase"
	echo " $ ./netsim_app -f example/testcase 2>temp"
	echo " $ ./extract.sh temp 1"
	
        exit 1
}

if [ "$#" != "2" ]
then
        ShowUsage
fi

grep "( $2):" $1 > $2.tmp
grep "SEND" $2.tmp > $2.send
grep "RECV" $2.tmp > $2.recv

echo "Done."
