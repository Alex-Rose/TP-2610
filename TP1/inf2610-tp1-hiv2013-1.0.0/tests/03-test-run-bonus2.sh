#!/bin/bash
#
# This bash script aims to run the program in a normal way
#

. ./util.sh

# We verify that BONUS 2 is activated, else we just quit without error, that's a bonus !
if [ $(grep -c "^#define BONUS2" ../src/inf2610.h) -eq 0 ]; then
	cecho "Bonus 2 is not active (use the define in 'inf2610.h' to change that)"
	exit 0
fi

# We simply run the program...
cecho "Run with SIGINT after 0.1sec..."
MEMOIRE="../memoire/memoire.txt"
NUMCOPROC=3
timeout 2m ../src/processeur $NUMCOPROC $MEMOIRE $JOURNAL > $STDOUT 2>&1 &

CPID=$!
sleep 0.1
kill -s SIGINT $CPID
wait $CPID

RETOUR=$?
if [ $RETOUR -ne 0 ]; then
	cecho "The program seems to be in error."
	end 1
fi

# We verify that there is nothing on the STDOUT
cecho "STDOUT verification..."
if [ -s $STDOUT ]; then
	cecho "The STDOUT is not empty - is DEBUG activated ?!"
	end 1
fi

#####
# We now compare the memory file and the generated log
#
cecho "Memory and journal comparison..."

# We verify that the log file exists
if [ ! -f "$JOURNAL" ]; then
	cecho "\e[1;31mERROR\e[00m the log file '$JOURNAL' hadn't been created"
	end 1
fi

# There is currently no error... :)
ERROR=0

# Load MEM file
MEM=$(cat $MEMOIRE)

# Prepare the counters for the coprocessors
COPRO_IN=()
COPRO_OP=()
for i in $(seq 1 $NUMCOPROC); do
	COPRO_IN+=( 0 )
	COPRO_OP+=( 0 )
done

# A variable to signal when we start reading the statistics
STATISTICS=0

# A variable to signal that we read the SIGINT message
GOTSIGINT=0

# Prepare REGEX
REGEX_OPERATION="Le coprocesseur ([0-9]+) a trait[ée] l'op[ée]ration (d'addition|de soustraction|de modulo|de division|de multiplication) de ([0-9]+) et ([0-9]+) et a obtenu ([0-9.]+) comme r[ée]sultat."
REGEX_INTERRUPTION="Le coprocesseur ([0-9]+) a trait[ée] une interruption."
REGEX_STATISTICS="Le coprocesseur ([0-9]+) a trait[ée] une moyenne de ([0-9.]+) op[ée]ration\(s\) par cycle."
REGEX_SIGINT="Signal SIGINT reçu."

# And go looping !
while read line; do
	if [ $GOTSIGINT -ne 0 ]; then
		cecho "\e[1;31mERROR\e[00m there is a line after the SIGINT message in the log file."
		ERROR=1
	fi

	if [[ $line =~ ^${REGEX_OPERATION}$ ]]; then
		# We verify that the statistics haven't started yet
		if [ $STATISTICS -gt 0 ]; then
			cecho "\e[1;31mERROR\e[00m statistics before another type of line of content"
			ERROR=1
		fi

		# Si on matche une opération, on la vérifie et on la retire du fichier mémoire
		coprocesseur=${BASH_REMATCH[1]}
		valeur1=${BASH_REMATCH[3]}
		valeur2=${BASH_REMATCH[4]}
		resultat=$(echo "print '%.1f' % float(${BASH_REMATCH[5]})" | python)
		case ${BASH_REMATCH[2]} in
			"d'addition")
				operation="A"
				calc=$(echo "print '%.1f' % (float($valeur1)+float($valeur2))" | python)
				;;
			"de soustraction")
				operation="S"
				calc=$(echo "print '%.1f' % (float($valeur1)-float($valeur2))" | python)
				;;
			"de modulo")
				operation="O"
				calc=$(echo "print '%.1f' % (float($valeur1)%float($valeur2))" | python)
				;;
			"de division")
				operation="D"
				calc=$(echo "print '%.1f' % (float($valeur1)/float($valeur2))" | python)
				;;
			"de multiplication")
				operation="M"
				calc=$(echo "print '%.1f' % (float($valeur1)*float($valeur2))" | python)
				;;
			*)
				ERROR=1
				cecho "Unknown operation: ${BASH_REMATCH[2]}"
				;;
		esac

		# We verify the number of the coprocessor
		if [ $coprocesseur -lt 1 ] || [ $coprocesseur -gt $NUMCOPROC ]; then
			cecho "\e[1;31mERROR\e[00m the coprocessor '$coprocesseur' should not exist."
			ERROR=1
		fi

		# We verify that the line exists in the memory file, if so, we remove this
		# line so another one will not match the same, and we'll be able to match
		# double treated lines
		memline="$coprocesseur $operation $valeur1 $valeur2"
		if [ $(echo "$MEM" | grep -c "^${memline}$") -gt 0 ]; then
			# Remove _only_ the first occurency of the line
			MEM=$(echo "$MEM" | sed "0,/^${memline}$/{//d;}")
		else
			cecho "\e[1;31mERROR\e[00m the line '$memline' does not appear in the memory file."
			ERROR=1
		fi

		# Now we verify the result
		if [ $resultat != $calc ]; then
			cecho "\e[1;31mERROR\e[00m in calculation of '$operation $valeur1 $valeur2' from c.$coprocesseur: '$resultat' (in journal) instead of '$calc' (real)"
			ERROR=1
		fi

		# We add an operation to the counter of this coprocessor
		((COPRO_OP[$(($coprocesseur - 1))]++))
	elif [[ $line =~ ^${REGEX_INTERRUPTION}$ ]]; then
		# We verify that the statistics haven't started yet
		if [ $STATISTICS -gt 0 ]; then
			cecho "\e[1;31mERROR\e[00m statistics before another type of line of content"
			ERROR=1
		fi

		coprocesseur=${BASH_REMATCH[1]}

		# We add an interruption to the counter of this coprocessor
		((COPRO_IN[$(($coprocesseur - 1))]++))
	elif [[ $line =~ ^${REGEX_STATISTICS}$ ]]; then
		coprocesseur=${BASH_REMATCH[1]}
		mean=$(echo "print '%.1f' % float(${BASH_REMATCH[2]})" | python)
		STATISTICS=1

		# We verify the value of the mean for that coprocessor
		calc=$(echo "print '%.1f' % float(${COPRO_OP[$(($coprocesseur - 1))]}/${COPRO_IN[$(($coprocesseur - 1))]})" | python)

		if [ $mean != $calc ]; then
			cecho "\e[1;31mERROR\e[00m in calculation of the mean operation/cycle of c.$coprocesseur: '$mean' (in journal) instead of '$calc' (real)"
			ERROR=1
		fi
	elif [[ $line =~ ^${REGEX_SIGINT}$ ]]; then
		GOTSIGINT=1
	else
		cecho "\e[1;31mERROR\e[00m unknown line type: $line"
		ERROR=1
	fi
done < $JOURNAL

# If there is not SIGINT message...
if [ $GOTSIGINT -eq 0 ]; then
	cecho "\e[1;31mERROR\e[00m Hm... There seems to be no SIGINT at all, here !"
	ERROR=1
fi

# If error, we perhaps want to check the log file
if [ $ERROR -ne 0 ]; then
	NEWNAME="$(basename $0)-$(date +%Y%m%d-%H%M%S).txt"
	cecho "Log file '$JOURNAL' copied as '$NEWNAME' for debug purposes. Good luck ! ;)"
	cp "$JOURNAL" "$NEWNAME"
fi

end $ERROR
