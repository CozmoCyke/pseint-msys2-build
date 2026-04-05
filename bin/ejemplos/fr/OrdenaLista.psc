??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

//   Enter a list of names (the list ends
// when a blank name is entered) without allowing
// repeated names, and then sort and display it

{Proceso} OrdenaLista
	
	{Definir nombre,lista,aux Como Cadenas}{;}
	{Definir se_repite Como Logico}{;}
	{Definir cant,i,j,pos_menor Como Enteros}{;}
	Dimension lista[200]{;}
	
	Escribir "Entrez les noms (ligne vide pour terminer) :"{;}
	
	// read the list
	cant<-0{;}
	Leer nombre{;}
	Mientras nombre<>"" Hacer
		{cant<-cant+1#lista[cant]<-nombre}{;}
		{lista[cant]<-nombre#cant<-cant+1}{;}
		Repetir // read a name and check that it is not already in the list
			Leer nombre{;}
			se_repite<-Falso{;}
			Para i<-{1#0} Hasta {cant#cant-1} Hacer
				Si nombre=lista[i] Entonces
					se_repite<-Verdadero{;}
				FinSi
			FinPara
		Hasta Que {~} se_repite
	FinMientras
	
	// sort
	Para i<-{1#0} Hasta {cant-1#cant-2} Hacer
		// find the smallest between i and cant
		pos_menor<-i{;}
		Para j<-i+1 Hasta {cant#cant-1} Hacer
			Si lista[j]<lista[pos_menor] Entonces
				pos_menor<-j{;}
			FinSi
		FinPara
		// swap the one at i with the smallest one found
		aux<-lista[i]{;}
		lista[i]<-lista[pos_menor]{;}
		lista[pos_menor]<-aux{;}
	FinPara	
	
	// show the sorted list
	Escribir "La liste triée est :"{;}
	Para i<-{1#0} Hasta {cant#cant-1} Hacer
		Escribir "   ",lista[i]{;}
	FinPara
	
{FinProceso}
