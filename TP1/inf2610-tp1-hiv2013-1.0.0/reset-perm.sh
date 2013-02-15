#!/bin/bash
# This file aims to help reset all the executable permissions
# on the Polytechniques' computer labs for all files of this
# practical work.
#
#     2013 (C)  Raphaël Beamonte <raphael.beamonte@gmail.com>
#

EXECPERM=(
	"./configure"
	"./config.guess"
	"./libtool"
	"./compile"
	"./install-sh"
	"./config.status"
	"./missing"
	"./config.sub"
	"./depcomp"
	"./tests/util.sh"
	"./tests/03-test-run-bonus2.sh"
	"./tests/02-test-run-bonus1.sh"
	"./tests/01-test-run.sh"
)

for file in ${EXECPERM[@]}; do
	if [ -f "$file" ]; then
		chmod 755 $file
	fi
done

echo "All executable permissions reset. Enjoy ;)"

