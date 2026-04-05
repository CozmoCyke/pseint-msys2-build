SubProceso IncrementaCopia(a Por Copia)
    a <- a + 1
    Escribir a
FinSubProceso

SubProceso IncrementaReferencia(b Por Referencia)
    b <- b + 2
FinSubProceso

Algoritmo TestSubprocesoParams
    Definir x Como Entero
    x <- 10
    IncrementaCopia(x);
    IncrementaReferencia(x);
    Escribir x
FinAlgoritmo
