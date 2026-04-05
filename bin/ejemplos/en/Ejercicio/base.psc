Proceso compra_articulos
	Definir n, i Como Entero;
	Definir monto, precio Como Real;
	monto <- 0;
	Escribir "Number of items:";
	Leer n;
	Escribir "Prices:";
	Para i <- 1 Hasta n Hacer
		/// complete the missing pseudocode here
		/// to read each price and add it to monto
	FinPara
	Si monto >= 500 Entonces
		/// complete the missing pseudocode here
		/// to apply the discount to monto
	FinSi
	Escribir "Amount to pay: $", monto;
FinProceso
