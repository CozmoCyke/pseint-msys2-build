??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// Petit jeu dans lequel l'utilisateur doit deviner un nombre en 10 essais

{Proceso} Adivina_Numero

	{Definir intentos,num_secreto,num_ingresado Como Entero}{;}
	intentos<-10{;}
	num_secreto <- azar(100)+1{;}
	
	Escribir "Devinez le nombre (de 1 à 100) :"{;}
	Leer num_ingresado{;}
	Mientras num_secreto<>num_ingresado {&} intentos>1 Hacer
		Si num_secreto>num_ingresado Entonces
			Escribir "Trop petit"{;}
		SiNo 
			Escribir "Trop grand"{;}
		FinSi
		intentos <- intentos-1{;}
		Escribir "Il vous reste ",intentos," essais :"{;}
		Leer num_ingresado{;}
	FinMientras
	
	Si num_secreto=num_ingresado Entonces
		Escribir "Exact ! Vous l'avez trouvé en ",11-intentos," essais."{;}
	SiNo
		Escribir "Le nombre était : ",num_secreto{;}
	FinSi
	
{FinProceso}
