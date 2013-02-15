#!/bin/bash

# get bash name
NAME=$(ps -o comm= $$)

# We want to work in the directory of the bash script
cd $(dirname $0)

# prepare journal and stdout file names
JOURNAL="$(basename $0)-journal.txt"
STDOUT="$(basename $0)-stdout.txt"

# end function
end () {
	rm -f $JOURNAL $STDOUT
	exit $1
}

# cecho command
COLOR=$((31 + $((16#$(echo $NAME | md5sum | cut -c1-5))) % 6))
cecho () {
	echo -e "\e[00;${COLOR}m$(basename $0):\e[00m $1"
}

