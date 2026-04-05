??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// calcule les racines d'une équation du second degré

{Proceso} Resolvente
	
	{Definir a,b,c,disc,preal,pimag,r1,r2 Como Reales}{;}
	// load data
	Escribir "Entrez le coefficient A :"{;}
	Leer a{;}
	Escribir "Entrez le coefficient B :"{;}
	Leer b{;}
	Escribir "Entrez le coefficient C :"{;}
	Leer c{;}
	
	// determine whether they are real or imaginary
	disc <- b^2-4*a*c{;}
	Si disc<0 Entonces
		// if they are imaginary
		preal<- (-b)/(2*a){;}
		pimag<- rc(-disc)/(2*a){;}
		Escribir "Racine 1 : ",preal,"+",pimag,"i"{;}
		Escribir "Racine 2 : ",preal,"-",pimag,"i"{;}
	SiNo
		Si disc=0 Entonces // check whether they are equal or different
			r <- (-b)/(2*a){;}
			Escribir "Root 1 = Racine 2 : ",r{;}
		SiNo
			r1 <- ((-b)+rc(disc))/(2*a){;}
			r2 <- ((-b)-rc(disc))/(2*a){;}
			Escribir "Racine 1 : ",r1{;}
			Escribir "Racine 2 : ",r2{;}
		FinSi
	FinSi
	
{FinProceso}
