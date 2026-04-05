??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

{Proceso} Modulo
	{Definir N,M Como Enteros}{;}
	Escribir "Entrez le nombre : "{;}
	Leer N{;}
	Escribir "Entrez le diviseur : "{;}
	Leer M{;}
	Si N {%} M = 0 Entonces
		Escribir M," est un diviseur exact de ",N,"."{;}
	SiNo
		Escribir "Le reste de la division de ",N," par ",M," est : ",N {%} M{;}
	FinSi
{FinProceso}
