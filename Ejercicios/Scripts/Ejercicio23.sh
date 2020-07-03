

if test $# -ne 2
then
	echo "Error en el numero de argumentos" >&2
	exit 1
fi

if ! test -d $1
then
	echo "Error el primer argumento no es directorio"
	exit 1
fi

if ! test -f $2
then
	echo "El segundo argumento no es un fichero"
	exit 1
fi

cd $1
for i in $(seq 1 100)
do
	gcc -o practica$i practica$i.c &> /dev/null
	
	if test $? -eq 0
	then
		./practica$i <$2> practica$i.c.salida 2> practica$i.c.errores
		rm practica$i
	else
		echo "Error al compilar practica$i" >&2
	fi
done
