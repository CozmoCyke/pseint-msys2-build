"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Calculates the average of a list of N values

{Proceso} Promedio

	{Definir i,N como Entero}{;}
	{Definir acum,dato,prom como Reales}{;}
	Escribir "Enter the amount of data:"{;}
	Leer n{;}
	
	acum<-0{;}
	
	Para i<-1 Hasta n Hacer
		Escribir "Enter data item ",i,":"{;}
		Leer dato{;}
		acum<-acum+dato{;}
	FinPara
	
	prom<-acum/n{;}
	
	Escribir "The average is: ",prom{;}
	
{FinProceso}
