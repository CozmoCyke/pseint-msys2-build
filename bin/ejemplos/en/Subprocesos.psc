"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// function that receives nothing and returns nothing
{SubProceso} Saludar
	Escribir "Hello world!"{;}
{FinSubProceso}

// function that receives an argument by value and returns its double
{SubProceso} res <- CalcularDoble(num) 
	{Definir res como real}{;}
	res <- num*2{;} // returns the double
{FinSubProceso}

// function that receives an argument by reference and modifies it
{SubProceso} Triplicar(num por referencia) 
	num <- num*3{;} // modifies the variable by tripling its value
{FinSubProceso}

// main process, which invokes the functions declared above
{Proceso} PruebaFunciones
	
	{Definir x Como Real}{;}
	
	Escribir "Calling the Saludar function:"{;}
	Saludar{;} // since it receives no arguments, the empty parentheses may be omitted
	
	Escribir "Enter a numeric value for x:"{;}
	Leer x{;}
	
	Escribir "Calling the CalcularDoble function (pass by value)"{;}
	Escribir "The double of ",x," is ", CalcularDoble(x){;}
	Escribir "The original value of x is ",x{;}
	
	Escribir "Calling the Triplicar function (pass by reference)"{;}
	Triplicar(x){;}
	Escribir "The new value of x is ", x{;}
	
{FinProceso}
