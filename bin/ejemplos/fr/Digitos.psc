??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// Splits an integer into its chiffres

{Proceso} Digitos
	
	{Definir i,digito,pot,n,aux,cont Como Enteros}{;}
	Escribir "Entrez un entier positif :"{;}
	Leer n{;}
	
	// first, count how many chiffres
	cont <- 0{;} 
	aux <- n{;}
	Mientras aux>0 hacer // while it is not zero
		cont <- cont + 1{;} // count how many chiffres
		aux <- trunc(aux/10){;} // divide par 10 and discard the remainder
	FinMientras
	Escribir "Le nombre a ",cont," chiffres"{;}
	
	// then, display them one par one 
	aux<-n{;}
	Para i<-1 hasta cont Hacer
		pot <- 10^(cont-i){;} // how much to divide par to get the first digit
		digito <- trunc (aux / pot){;} // get the digit
		aux <- aux - digito*pot{;} // remove that digit from the number
		Escribir "Digit ",i," is ",digito{;}
	FinPara
	
{FinProceso}
