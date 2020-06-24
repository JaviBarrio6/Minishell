

if test $# -lt 2 || test $# -gt 3
then
	echo "Error en el numero de argumentos" >&2
	exit 1
fi

if test $# -eq 3
then
	if test -d $3
	then
		dir = $3
	else
		echo "$3 no es un directorio" >&2
		exit 1
	fi
else
	dir = "."
fi

ficheros = $(find $dir -type f)

for i in $ficheros
do
	N = $(diff $1 $i | wc -l)
	if test N -eq 0
	then
		echo $i >> $2
	fi
done
