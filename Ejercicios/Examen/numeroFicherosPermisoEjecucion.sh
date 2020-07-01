
if test $# -gt 1
then
	echo "Error argumentos" >&2
	exit 1
fi

if test $# -eq 0
then
	if ! test -d $1
	then
		echo "$1 no es directorio" >&2
		exit 1
	fi
	
	dir = $1
else
	dir = "."
fi

ficheros = $(find $dir -type f)
total = 0

for i in $ficheros
do
	if test -x $i
	then
		total = $((total +1))
	fi
done

echo $total
