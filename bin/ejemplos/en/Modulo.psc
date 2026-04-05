"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

{Proceso} Modulo
	{Definir N,M Como Enteros}{;}
	Escribir "Enter the number: "{;}
	Leer N{;}
	Escribir "Enter the divisor: "{;}
	Leer M{;}
	Si N {%} M = 0 Entonces
		Escribir M," is an exact divisor of ",N,"."{;}
	SiNo
		Escribir "The remainder of dividing ",N," by ",M," is: ",N {%} M{;}
	FinSi
{FinProceso}
