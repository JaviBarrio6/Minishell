
if test $# -lt 1 || test $# -gt 2
then
	echo "Error numero de argumentos" >&2
	exit 1
fi

if ! test -d $1
then
	echo "$1 no es un directorio" >&2
	exit 1
fi

if test $# -eq 2
then
	if ! test -d $2
	then
		echo "$2 no es un directorio" >&2
		exit 1
	fi
	
	dir = $2
else
	dir = "."
fi

ficheros = $(find $1 -type f -name "*.mp3")

for i in $ficheros
do
	cp $i $dir
done


