"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Finds the first N prime numbers

{Proceso} Primos
	
	{Definir cant_a_mostrar,n,cant_mostrados,i Como Entero}{;}
	{Definir es_primo Como Logico}{;}
	Escribir "Enter how many prime numbers to show:"{;}
	Leer cant_a_mostrar{;}
	
	Escribir "1: 2"{;} // the first prime is 2, the others are all odd...
	cant_mostrados <- 1{;}
	n<-3{;}            // ...starting from 3
	
	Mientras cant_mostrados<cant_a_mostrar Hacer
		
		es_primo <- Verdadero{;} // assume it is prime until a divisor is found
		
		Para i<-3 hasta rc(n) con paso 2 Hacer // we already know it is odd
			Si n {%} i = 0 entonces // if the division is exact...
				es_primo <- Falso{;}  // ...then it is no longer prime
			FinSi
		FinPara
		
		Si es_primo Entonces
			cant_mostrados <- cant_mostrados + 1{;}
			Escribir cant_mostrados, ": ",n{;}
		FinSi
		
		n <- n + 2 {;}
		
	FinMientras
	
{FinProceso}
