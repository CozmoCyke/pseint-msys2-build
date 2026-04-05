Proceso compra_articulos
	Definir n, i Como Entero;
	Definir monto, precio Como Real;
	monto <- 0;
	Escribir "Number of items:";
	Leer n;
	Escribir "Prices:";
	Para i <- 1 Hasta n Hacer
		Leer precio;
		monto <- monto + precio;
	FinPara
	Si monto >= 500 Entonces
		monto <- monto * 0.95;
	FinSi
	Escribir "Amount to pay: $", monto;
FinProceso
