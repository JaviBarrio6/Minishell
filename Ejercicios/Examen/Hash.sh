

if test $# -gt 2 
then	
	echo "Error numero de argumentos" >&2
	exit 1
fi

if ! test -d $1 
then
	echo "$1 no es directorio" >&2
	exit 1
fi

cd $1

ficheros = $(ls)

for i in ficheros
do
	if test -f $i && test -r $i
	then
		user = $(ls -l $i | cut -f3)
		
		resume = $(sha256sum $i)
		
		echo $user $resume >> $2
	fi
done
