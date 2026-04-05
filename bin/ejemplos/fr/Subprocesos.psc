??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// fonction qui ne reçoit rien et ne renvoie rien
{SubProceso} Saludar
	Escribir "Bonjour le monde !"{;}
{FinSubProceso}

// function that receives an argument par value and returns its double
{SubProceso} res <- CalcularDoble(num) 
	{Definir res como real}{;}
	res <- num*2{;} // returns the double
{FinSubProceso}

// function that receives an argument par reference and modifies it
{SubProceso} Triplicar(num por referencia) 
	num <- num*3{;} // modifies the variable par tripling its value
{FinSubProceso}

// main process, which invokes the functions declared above
{Proceso} PruebaFunciones
	
	{Definir x Como Real}{;}
	
	Escribir "Appel de la fonction Saludar :"{;}
	Saludar{;} // since it receives no arguments, the empty parentheses may be omitted
	
	Escribir "Entrez une valeur numérique pour x :"{;}
	Leer x{;}
	
	Escribir "Calling the CalcularDoble function (pass par value)"{;}
	Escribir "The double of ",x," is ", CalcularDoble(x){;}
	Escribir "The original value of x is ",x{;}
	
	Escribir "Calling the Triplicar function (pass par reference)"{;}
	Triplicar(x){;}
	Escribir "The new value of x is ", x{;}
	
{FinProceso}
