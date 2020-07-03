
if test $# -ne 2
then
	echo "Error en el numero de argumentos" >&2
fi 

if test $1 != "borrar" && test $1 1= "permisoW"
then 
	echo "$1 incorrecto" >&2
fi

if ! test -f $2 
then
	echo "$2 no es un fichero" >&2
fi

if test $1 == "borrar"
then
	rm $2
	if $? -eq 0
	then
		echo "Fichero borrado"
		exit 0
	else
		echo "Error al borrar fichero" >&2
		exit 1
	fi
fi

if test $1 == "permisoW"
then
	chmod ug + w $2
	echo "Permisos otorgados al fichero"
	exit 0
fi
