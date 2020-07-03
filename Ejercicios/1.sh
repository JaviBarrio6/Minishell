

if test $# -lt 2
then
	echo "Error numero de argumentos" >&2
	exit 1
fi

for i in $#
do
	if ! test -d $i
	then
		echo "$i no es un directorio" >&2
		exit 1
	fi
done

cd $1

ficherosValidos = $(find -type f -name fic_*_*_*.sec)
	
for j in $ficherosValidos
do
	for i in $#
	do
	if ! test $1
	then
		cp $j $i 
	fi
	done
done

