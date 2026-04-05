"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// To run this example correctly, you must have
// flexible syntax enabled in your language profile

{Proceso} sin_titulo	


	{Definir A,i,elemento Como Enteros;}
	// declare an array of 10 elements
	Dimension A[10]{;}
	
	// go through the 10 elements assigning random integers
	para cada elemento de A Hacer
		// elemento takes the content of each position in the array
		// and if elemento is modified the array is modified too
		elemento <- azar(100){;}
	FinPara
	
	Escribir "The array elements are:"{;}
	// go through the 10 elements using subscripts and show them on screen
	para i desde {1#0} hasta {10#9} Hacer
		escribir "Posici�n " i ": " A[i]{;}
	FinPara
	
	Escribir ""{;} // leave a blank line
	
	Escribir "In reverse order:"{;}
	// go through the 10 elements in reverse order and show them on the same line
	para i desde {10#9} hasta {1#0} Hacer
		escribir sin bajar A[i] " "{;}
	FinPara
	
{FinProceso}
