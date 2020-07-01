
if test $# -gt 3 || test $# -lt 2
then
	echo "Error argumentos" >&2
	exit 1
fi

if test $# -eq 3
then
	if ! test -d $3
	then
		echo "$3 no es directorio">&2
		exit 1
	else
		dir = $3
	fi
else
	dir = "."
fi

ficheros = $(find $dir -type f)

for i in ficheros
do
	fallas = $(diff $1 $i | wc -l) 
	
	if test $fallas -eq 0
	then
		echo $i >> $2
	fi
done
	
	 
