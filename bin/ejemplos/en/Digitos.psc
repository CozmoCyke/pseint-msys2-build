"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Splits an integer into its digits

{Proceso} Digitos
	
	{Definir i,digito,pot,n,aux,cont Como Enteros}{;}
	Escribir "Enter a positive integer:"{;}
	Leer n{;}
	
	// first, count how many digits
	cont <- 0{;} 
	aux <- n{;}
	Mientras aux>0 hacer // while it is not zero
		cont <- cont + 1{;} // count how many digits
		aux <- trunc(aux/10){;} // divide by 10 and discard the remainder
	FinMientras
	Escribir "The number has ",cont," digits"{;}
	
	// then, display them one by one 
	aux<-n{;}
	Para i<-1 hasta cont Hacer
		pot <- 10^(cont-i){;} // how much to divide by to get the first digit
		digito <- trunc (aux / pot){;} // get the digit
		aux <- aux - digito*pot{;} // remove that digit from the number
		Escribir "Digit ",i," is ",digito{;}
	FinPara
	
{FinProceso}
