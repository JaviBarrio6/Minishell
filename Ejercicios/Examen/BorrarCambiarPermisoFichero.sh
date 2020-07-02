
if test $# -ne 0
then
	echo "Error numero de argumentos" >&2
	exit 1
fi

if test $1 != "borrar" && $1 != "permisoW"
then
	echo "Error argumentos" >&2
	exit 1
fi

if ! test -f $2
then
	echo "$2 no es un fichero" >&2
	exit 1
fi

if test $1 == "borrar" 
then
	rm $2
	
	if test $? -ne 0
	then
		echo "Error al borrar fichero" >&2
		exit 1
	else
		echo "Fichero borrado con exito"
		exit 0
	fi
fi 

if test $1 == "permisoW"
then
	chmod ug+w $2
	echo "Permisos otorgados a $2"
	exit 0
fi
