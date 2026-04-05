"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Finds the two largest values in a list of N data

{Proceso} Mayores

	{Definir datos,n,may1,may2,i Como Numeros}{;}
	// first declare an array with 200 elements
	Dimension datos[200]{;}
	
	// then ask the user to enter the amount of data, 
	// which must not be greater than 200
	Escribir "Enter the amount of data (from 2 to 200):"{;}
	Leer n{;}
	
	// read the data one by one and store them in the array
	Para i<-{1#0} Hasta {n#n-1} Hacer
		Escribir "Enter data item ",{i#i+1},":"{;}
		Leer datos[i]{;}
	FinPara

	// compare the first two and keep them as may1 (the larger of the
	// two) and may2 (the second largest).
	Si datos[{1#0}]>datos[{2#1}] Entonces
		may1<-datos[{1#0}]{;}
		may2<-datos[{2#1}]{;}
	SiNo
		may1<-datos[{2#1}]{;}
		may2<-datos[{1#0}]{;}
	FinSi
	// go through the remaining elements looking for one greater than may1 or may2
	Para i<-{3#2} Hasta {n#n-1} Hacer
		Si datos[i]>may1 Entonces // if there is a value greater than may1
			may2<-may1{;} // since may1 was the largest, it moves to second place
			may1<-datos[i]{;} // and the new value takes first place (largest overall)
		SiNo // if it was not larger than may1, it may still be larger than may2
			Si datos[i]>may2 Entonces // if it is greater than the second largest we had
				may2<-datos[i]{;} // store it as the second largest
			FinSi
		FinSi
	FinPara
	
	// show the results
	Escribir "The largest is: ",may1{;}
	Escribir "The second largest is: ",may2{;}
	
{FinProceso}
