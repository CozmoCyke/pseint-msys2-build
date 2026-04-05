??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// Calcule la moyenne d'une liste de N valeurs

{Proceso} Promedio

	{Definir i,N como Entero}{;}
	{Definir acum,dato,prom como Reales}{;}
	Escribir "Entrez le nombre de données :"{;}
	Leer n{;}
	
	acum<-0{;}
	
	Para i<-1 Hasta n Hacer
		Escribir "Entrez la donnée ",i,":"{;}
		Leer dato{;}
		acum<-acum+dato{;}
	FinPara
	
	prom<-acum/n{;}
	
	Escribir "The average est : ",prom{;}
	
{FinProceso}
