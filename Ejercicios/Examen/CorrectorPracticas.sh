
if ! test $# -ne 2
then
	echo "Error numero argumentos" >&2
	exit 1
fi

if ! test -d $1
then
	echo "$1 no es un directorio"
	exit 1
fi

if ! test -f $2
then
	echo "$2 no es un fichero"
	exit 1
fi

for i in $(seq 1 100)
do
	gcc -o practica$i practica$i.c &> /dev/null
	
	if test $? -eq 0
	then
		./practica$i < $2 > practica$i.c.salida 2> practica$i.c.errores
		
		rm practica$i 
	else
		echo "Error al compilar practica$i.c" >&2
	fi
done 
