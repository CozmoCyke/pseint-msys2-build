??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// Calcule la moyenne d'une liste de N valeurs à l'aide d'un sous-processus

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
	Escribir "Entrez le nombre de données :"{;}
	Leer n{;}
	
	Para i<-{1#0} Hasta n{#-1} Hacer
		Escribir "Entrez la donnée ",i{#+1},":"{;}
		Leer datos[i]{;}
	FinPara
	
	Escribir "The average est : ",Promedio(datos,n){;}
	
{FinProceso}
