??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// If you have trouble running this example, select the "Flexible" profile
// in the "Language Options" dialog, which you can open from the Configure menu.

// This example shows some of the possible variations when using
// flexible syntax, additional instructions, and colloquial-language
// expressions for conditions.

Algoritmo sin_titulo
	
	Definir a, b Como Entero
	
	Imprimir "Entrez 3 nombres (les deux premiers doivent être entiers) :"
	Leer a b c
	
	Si a Es Mayor Que b Y a Es Mayor Que c Entonces
		Escribir a " est le plus grand"
	SiNo 
		Si b Es Mayor Que c Entonces
			Escribir b " est le plus grand"
		SiNo
			Escribir c " est le plus grand"
		FinSi
	FinSi

	
	Mostrar "Appuyez sur une touche pour continuer"
	Esperar Tecla
	Limpiar Pantalla
	
	Si a Es Par
		Escribir a " est pair"
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
