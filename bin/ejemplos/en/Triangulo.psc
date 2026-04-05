"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

//    Reads the three sides of a right triangle and determines 
// whether they match (by Pythagoras), and if so 
// computes the area

{Proceso} TrianguloRectangulo
	

	// load data
	{Definir l1,l2,l3 Como Real}{;}
	Escribir "Enter side 1:"{;}
	Leer l1{;}
	Escribir "Enter side 2:"{;}
	Leer l2{;}
	Escribir "Enter side 3:"{;}
	Leer l3{;}
	
	// find the hypotenuse (largest side)
	{Definir cat1,cat2,hip Como Real}{;}
	Si l1>l2 Entonces
		cat1<-l2{;}
		Si l1>l3 Entonces
			hip<-l1{;}
			cat2<-l3{;}
		SiNo
			hip<-l3{;}
			cat2<-l1{;}
		FinSi
	SiNo
		cat1<-l1{;}
		Si l2>l3 Entonces
			hip<-l2{;}
			cat2<-l3{;}
		SiNo
			hip<-l3{;}
			cat2<-l2{;}
		FinSi
	FinSi
	
	// check whether it satisfies Pythagoras
	Si hip^2 = cat1^2 + cat2^2 Entonces
		// calculate area
		{Definir area como real;}
		area<-(cat1*cat2)/2{;}
		Escribir "The area is: ",area{;}
	SiNo
		Escribir "It is not a right triangle."{;}
	FinSi
	
{FinProceso}
