

if test $# -ne 2
then
	echo "Error numero de argumentos" >&2
	exit 1
fi

if test -d $1
then
	echo "$1 no es un directorio" >&2
	exit 1
fi 

cd $1
ficheros = $(ls)

for i in ficheros
do
	if test -f && test -r $i
	then
		user = $(ls -l | cut -f3)
		h = $(sha256sum $1)
		echo $h $user >> $2
	fi
done
