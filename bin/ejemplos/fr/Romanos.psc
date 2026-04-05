??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

{Proceso} Romanos
	
	// read the number
	{Definir Numero como Numerico}{;}
	Escribir "Entrez un nombre entre 1 et 1000"{;}
	Leer Numero{;}
	
	// check that it is valid for conversion
	{Definir SePuedeConvertir Como Logico}{;}
	SePuedeConvertir<-Verdadero{;}
	Si Numero=0 Entonces
		Escribir "Il n'existe pas de symbole pour représenter 0"{;}
		SePuedeConvertir<-Falso{;}
	FinSi
	Si Numero<>trunc(numero) Entonces
		Escribir "Le nombre doit être entier"{;}
		SePuedeConvertir<-Falso{;}
	FinSi
	Si Numero>1000 Entonces
		Escribir "Trop grand"{;}
		SePuedeConvertir<-Falso{;}
	FinSi
	Si Numero<0 Entonces
		Escribir "It must be positive"{;}
		SePuedeConvertir<-Falso{;}
	FinSi
	
	// perform the conversion
	Si SePuedeConvertir Entonces		
		Si Numero=1000 Entonces
			Escribir "M"{;}
		SiNo
			Dimension nu[10], nd[10], nc[10]{;} // notation for units, tens, and hundreds
			{Definir nu,nd,nc Como Caracter}{;}
			nu[{1#0}]<-''; nu[{2#1}]<-'I'; nu[{3#2}]<-'II'; nu[{4#3}]<-'III'; nu[{5#4}]<-'IV'; nu[{6#5}]<-'V'; nu[{7#6}]<-'VI'; nu[{8#7}]<-'VII'; nu[{9#8}]<-'VIII'; nu[{10#9}]<-'IX'{;}
			nd[{1#0}]<-''; nd[{2#1}]<-'X'; nd[{3#2}]<-'XX'; nd[{4#3}]<-'XXX'; nd[{5#4}]<-'XL'; nd[{6#5}]<-'L'; nd[{7#6}]<-'LX'; nd[{8#7}]<-'LXX'; nd[{9#8}]<-'LXXX'; nd[{10#9}]<-'XC'{;}
			nc[{1#0}]<-''; nc[{2#1}]<-'C'; nc[{3#2}]<-'CC'; nc[{4#3}]<-'CCC'; nc[{5#4}]<-'CD'; nc[{6#5}]<-'D'; nc[{7#6}]<-'DC'; nc[{8#7}]<-'DCC'; nc[{9#8}]<-'DCCC'; nc[{10#9}]<-'CM'{;}
			{Definir centenas,decenas,unidades como numeros}{;}
			centenas<-trunc(Numero/100) {%} 10{;}
			decenas<-trunc(Numero/10) {%} 10{;}
			unidades<-Numero {%} 10{;}
			Escribir nc[centenas{+1#}],nd[decenas{+1#}],nu[unidades{+1#}]{;}
		FinSi
	FinSi
	
{FinProceso}
