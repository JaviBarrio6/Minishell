

if test $# -gt 1
then
	echo "Error argumentos" >&2
	exit 1
fi

if test $# -eq 0
then
	
	dir = "."	

else 

	if ! test -d $1
	then
		echo "$1 no es un directorio" >&2
		exit 1
	fi
	
	dir = $1
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
	
fi
