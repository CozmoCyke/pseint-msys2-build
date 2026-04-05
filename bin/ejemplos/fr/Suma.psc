??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

// c'est l'exemple le plus simple de cette aide, 
// it takes two numbers, adds them, and shows the result

{Proceso} Suma

    {Definir A,B,C como Reales}{;}

    // to read a value, a message is shown to the user
    // with the Escribir instruction, and then the value is read into 
    // a variable (A for the first, B for the second) with 
    // the Leer instruction

    Escribir "Entrez le premier nombre :"{;}
    Leer A{;}

    Escribir "Entrez le deuxième nombre :"{;}
    Leer B{;}


    // now the sum is calculated and the result is stored in the
    // variable C using assignment (<-)
    
    C <- A+B{;}


    // finally, the result is shown, preceded par a 
    // message for the user, all in a single
    // Escribir instruction
    
    Escribir "The result est : ",C{;}

{FinProceso}
