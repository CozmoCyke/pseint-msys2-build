"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// This example shows the use of expressions, operators, and mathematical functions

{Proceso} Matematicas

   {Definir EligeSalir,EsPrimo Como Logico}{;}
   {Definir N,Nu,eleccion,f,r Como Numeros}{;}

   EligeSalir<-Falso{;}
   Escribir 'Ingresar un n�mero:'{;}
   Leer N{;}
   Repetir
      Escribir ' '{;}
      Escribir 'Press a key to continue'{;}
          Esperar Tecla{;}
          Limpiar Pantalla{;}
      Escribir 'Elija una opci�n:'{;}
      Escribir '  1 - Sine, cosine, arctangent'{;}
      Escribir '  2 - Lograritmo natural, funci�n exponencial'{;}
      Escribir '  3 - Truncate, round'{;}
      Escribir '  4 - Ra�z cuadrada'{;}
      Escribir '  5 - Absolute value'{;}
      Escribir '  6 - Separate integer and decimal parts'{;}
      Escribir '  7 - Compute factorial'{;}
      Escribir '  8 - Check whether it is prime'{;}
      Escribir '  9 - Ingresar otro n�mero'{;}
      Escribir '  0 - Exit'{;}
      Escribir ' '{;}
      Leer eleccion{;}
      Segun eleccion Hacer
         1: 
            Escribir 'Sine:',Sen(N){;}
            Escribir 'Cosine:',Cos(N){;}
            Escribir 'Arctangent:',Atan(N){;}
         2: 
            Si N<=0
               Entonces Escribir 'The number must be greater than zero!'{;}
            SiNo
               Escribir 'Natural logarithm: ',ln(N){;}
               Escribir 'Funci�n exponencial: ',exp(N){;}
            FinSi
         3: 
            Escribir 'Truncate: ',trunc(N){;}
            Escribir 'Round: ',redon(N){;}
         4: Escribir 'Sqrt: ',rc(N){;}
         5: Escribir 'Abs value: ',abs(N){;}
         6: 
            Escribir 'Integer part: ',Trunc(n){;}
            Escribir 'Decimal part: ',n-Trunc(n){;}
         7: 
            Si N<>Trunc(N)
               Entonces
                  Escribir 'The number must be an integer!'{;}
               SiNo
                  Si abs(N)>50
                     Entonces Escribir 'Result too large!'{;}
                     SiNo
                        r<-1; f<-1{;}
                        Mientras f<=abs(N) Hacer
                           Si N<0 
                              Entonces r<-(-f)*r{;}
                           SiNo
                              r<-f*r{;}
                           FinSi
                           f<-f+1{;}
                        FinMientras
                        Escribir 'Factorial:',r{;}
                  FinSi
            FinSi
         8: 
            Si N<>Trunc(N) Entonces
               Escribir 'The number must be an integer!'{;}
            SiNo
               Si N<0  entonces 
                  Nu<-N*(-1){;}
               SiNo
                  Nu<-N{;}
               FinSi
               Si N mod 2 = 0 Entonces 
                  Escribir 'Prime number:',Nu=2{;}
                  Si Nu<>2 Entonces
                     Escribir N,'=2x',N/2{;}
                  FinSi
               SiNo
                  EsPrimo<-Nu<>1{;} 
                  Nu<-RC(Nu){;}
                  Divisor<-3{;}
                  Mientras Divisor<=Raiz(Nu) {&} EsPrimo Hacer
                     Si N mod Divisor = 0 Entonces 
                        EsPrimo<-Falso{;}
                     Sino
						Divisor<-Divisor+2{;}
					FinSi
                  FinMientras
                  Escribir 'Prime number:',EsPrimo{;}
                  Si N>1 {&} {~} EsPrimo Entonces 
                     Escribir N,'=',Divisor,'x',N/Divisor{;}
                  FinSi
               FinSi
            FinSi
         9:
            Escribir 'Ingrese un n�mero:'{;}
            Leer N{;}
         0: EligeSalir<-Verdadero{;}
         De Otro Modo:
            Escribir 'Invalid option!'{;}
      FinSegun
   Hasta que EligeSalir
{FinProceso}
