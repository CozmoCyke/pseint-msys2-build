"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// calculates the roots of a quadratic equation

{Proceso} Resolvente
	
	{Definir a,b,c,disc,preal,pimag,r1,r2 Como Reales}{;}
	// load data
	Escribir "Enter coefficient A:"{;}
	Leer a{;}
	Escribir "Enter coefficient B:"{;}
	Leer b{;}
	Escribir "Enter coefficient C:"{;}
	Leer c{;}
	
	// determine whether they are real or imaginary
	disc <- b^2-4*a*c{;}
	Si disc<0 Entonces
		// if they are imaginary
		preal<- (-b)/(2*a){;}
		pimag<- rc(-disc)/(2*a){;}
		Escribir "Root 1: ",preal,"+",pimag,"i"{;}
		Escribir "Root 2: ",preal,"-",pimag,"i"{;}
	SiNo
		Si disc=0 Entonces // check whether they are equal or different
			r <- (-b)/(2*a){;}
			Escribir "Root 1 = Root 2: ",r{;}
		SiNo
			r1 <- ((-b)+rc(disc))/(2*a){;}
			r2 <- ((-b)-rc(disc))/(2*a){;}
			Escribir "Root 1: ",r1{;}
			Escribir "Root 2: ",r2{;}
		FinSi
	FinSi
	
{FinProceso}
