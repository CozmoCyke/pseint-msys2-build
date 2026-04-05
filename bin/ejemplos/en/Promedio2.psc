"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Calculates the average of a list of N values using a SubProcess

{SubProceso} prom <- Promedio ( arreglo, cantidad )
	{Definir i como Entero}{;}
	{Definir suma como Real}{;}
	{Definir prom como Real}{;}
	suma <- 0{;}
	Para i<-{1#0} Hasta cantidad{#-1} Hacer
		suma <- suma + arreglo[i]{;}
	FinPara
	prom <- suma/cantidad{;}
{FinSubProceso}

{Proceso} Principal

	{Definir i,N como Entero}{;}
	{Definir acum,datos,prom como Reales}{;}
	Dimension datos[100]{;}
	Escribir "Enter the amount of data:"{;}
	Leer n{;}
	
	Para i<-{1#0} Hasta n{#-1} Hacer
		Escribir "Enter data item ",i{#+1},":"{;}
		Leer datos[i]{;}
	FinPara
	
	Escribir "The average is: ",Promedio(datos,n){;}
	
{FinProceso}
