"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// this is the simplest example in this help, 
// it takes two numbers, adds them, and shows the result

{Proceso} Suma

    {Definir A,B,C como Reales}{;}

    // to read a value, a message is shown to the user
    // with the Escribir instruction, and then the value is read into 
    // a variable (A for the first, B for the second) with 
    // the Leer instruction

    Escribir "Enter the first number:"{;}
    Leer A{;}

    Escribir "Enter the second number:"{;}
    Leer B{;}


    // now the sum is calculated and the result is stored in the
    // variable C using assignment (<-)
    
    C <- A+B{;}


    // finally, the result is shown, preceded by a 
    // message for the user, all in a single
    // Escribir instruction
    
    Escribir "The result is: ",C{;}

{FinProceso}
