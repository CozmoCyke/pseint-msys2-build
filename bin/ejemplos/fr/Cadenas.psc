??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// This example shows how to use the functions for handling 
// character strings. It requires the language profile to
// enable these functions. If you get
// errors in the functions when trying to run it, review your configuration in the
// "Language Options" item in the "Configure" menu.
{Proceso} sin_titulo
	
	{Definir frase,vocales Como Caracter}{;}
	{Definir i,j,cantVocales Como Entero}{;}
	
	// the user enters a phrase, we are going to count how many vowels it has
	Escribir "Entrez une phrase"{;}
	Leer frase{;}
	// convert the whole phrase to lowercase so we do not have to check each vowel twice
	frase<-Minusculas(frase){;}
	
	// list of letters we are going to search for
	Vocales<-"aeiou??????"{;}
	cantvocales<-0{;}
	
	// compare all letters in the phrase with the vowels and count matches
	Para i<-{1#0} hasta Longitud(frase){#-1} Hacer
		Para j<-{1#0} hasta Longitud(vocales){#-1} Hacer
			Si Subcadena(frase,i,i)=Subcadena(vocales,j,j) Entonces
				cantVocales<-cantVocales+1{;}
			FinSi
		FinPara
	FinPara
	
	Escribir "The phrase contains ",cantVocales," vowels."{;}
	
	
{FinProceso}
