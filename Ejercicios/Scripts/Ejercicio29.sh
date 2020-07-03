

if test $# -lt 1 || test $# -gt 2
then
	echo "Error en el numero de argumentos" >&2
fi

if ! test -d $1
then
	echo "$1 no es directorio"
fi

if test $# -eq 2
then
	if ! test -d $2
	then 
		echo "$2 no es directorio"
	fi
	
	di = $2

else
	di = $pwd
fi

ficheros = $(find $1 -type f -name "*.mp3")
for i in $ficheros
do
	cp $i $dir
done
	

