"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Implementaci�n del c�lculo de una potencia mediante una funci�n recursiva
// The recursive step is based on A^B = B*(A^(B-1))
// The base step is based on A^0 = 1

{SubProceso} resultado <- Potencia (base, exponente)
    {Definir resultado como Entero}{;}
    Si exponente=0 Entonces
        resultado <- 1;
    sino 
        resultado <- base*Potencia(base,exponente-1); 
    FinSi
{FinSubProceso}

{Proceso} DosALaDiezRecursivo
    {Definir exponente como Entero}{;}
    {Definir base como Entero}{;}
    Escribir "Enter base"{;}
    Leer base{;}
    Escribir "Enter exponent"{;}
    Leer exponente{;}
    Escribir "The result is ",Potencia(base,exponente){;}
{FinProceso}
