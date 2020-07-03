
if test $# -gt 1
then
	echo "Error numero de argumentos"
	exit 1
fi

if test -d $1
then
	dir = $1
else
	echo "$1 no es un directorio"
	exit 1
fi

if test $# -eq 0
then
	dir = "."
fi

ficheros = $(find $dir -type f)
total = 0

for i in $ficheros
do
	if test -x $i
	then
		total = $((total + 1))
	fi
done

echo $total
