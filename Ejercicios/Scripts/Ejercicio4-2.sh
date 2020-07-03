

if test $# -ne 2
then
	echo "Error en el numero de argumentos"
	exit 1
fi

if ! test -d $1
then
	echo "$1 no es directorio"
	exit 1
fi

cd $1 

ficheros = $(ls)

for i in ficheros
do

	if test -f $i && test -r $i
	then
		user = $(ls -l $i | cut -f3)
		h = $(sha256sum $i)
		echo $h $user >> $2
	fi

done

