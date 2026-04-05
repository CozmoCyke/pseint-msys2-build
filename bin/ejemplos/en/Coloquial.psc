"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// If you have trouble running this example, select the "Flexible" profile
// in the "Language Options" dialog, which you can open from the Configure menu.

// This example shows some of the possible variations when using
// flexible syntax, additional instructions, and colloquial-language
// expressions for conditions.

Algoritmo sin_titulo
	
	Definir a, b Como Entero
	
	Imprimir "Enter 3 numbers (the first two must be integers):"
	Leer a b c
	
	Si a Es Mayor Que b Y a Es Mayor Que c Entonces
		Escribir a " is the largest"
	SiNo 
		Si b Es Mayor Que c Entonces
			Escribir b " is the largest"
		SiNo
			Escribir c " is the largest"
		FinSi
	FinSi

	
	Mostrar "Press a key to continue"
	Esperar Tecla
	Limpiar Pantalla
	
	Si a Es Par
		Escribir a " is even"
	SiNo
		Escribir a " is odd"
	FinSi
	
	Escribir Sin Bajar "The divisors of " b " are: "
	Para i Desde 1 Hasta b-1
		Si b Es Multiplo De i
			Escribir sin bajar i " "
		Fin Si
	Fin Para
	Escribir b
	
	Si c Es Entero 
		Mostrar C " is an integer"
	SiNo
		Mostrar C " is real"
	FinSi
	
FinAlgoritmo
