"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Simple game that asks the user to guess a number in 10 attempts

{Proceso} Adivina_Numero

	{Definir intentos,num_secreto,num_ingresado Como Entero}{;}
	intentos<-10{;}
	num_secreto <- azar(100)+1{;}
	
	Escribir "Guess the number (from 1 to 100):"{;}
	Leer num_ingresado{;}
	Mientras num_secreto<>num_ingresado {&} intentos>1 Hacer
		Si num_secreto>num_ingresado Entonces
			Escribir "Too low"{;}
		SiNo 
			Escribir "Too high"{;}
		FinSi
		intentos <- intentos-1{;}
		Escribir "You have ",intentos," attempts left:"{;}
		Leer num_ingresado{;}
	FinMientras
	
	Si num_secreto=num_ingresado Entonces
		Escribir "Exactly! You guessed it in ",11-intentos," attempts."{;}
	SiNo
		Escribir "The number was: ",num_secreto{;}
	FinSi
	
{FinProceso}
