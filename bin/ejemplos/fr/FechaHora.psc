??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

{Proceso} EjemploFechaHora
	
	fa <- FechaActual(){;} // returns a single integer in YYYYMMDD format
	anio <- trunc(fa/10000){;}
	mes <- trunc(fa/100)%100{;}
	dia <- fa%100{;}
	
	Mientras Verdadero Hacer
		Borrar Pantalla{;}
		Escribir "Date actuelle : ", dia, "/", mes, "/", anio{;}
		
		ha <- HoraActual(){;} // returns a single integer in HHMMSS format
		hora <- trunc(ha/10000){;}
		min <- trunc(ha/100)%100{;}
		seg <- ha%100{;}
		Escribir "Heure actuelle : ", hora, ":", min, ":", seg{;}
		
		Esperar 1 Segundo{;}
	FinMientras
	
{FinProceso}
