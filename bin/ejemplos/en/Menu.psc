"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Shows how to make a simple menu with the Repeat-Until and According To structures

{Proceso} sin_titulo
	{Definir OP Como Entero}{;}
	Repetir
		// show menu
		Limpiar Pantalla{;}
		Escribir "Recommendations menu"{;}
		Escribir "   1. Literature"{;}
		Escribir "   2. Movies"{;}
		Escribir "   3. Music"{;}
		Escribir "   4. Video games"{;}
		Escribir "   5. Exit"{;}
		// enter an option
		Escribir "Choose an option (1-5): "{;}
		Leer OP{;}
		// process that option
		Segun OP Hacer
			1:
				Escribir "Recommended readings:"{;}
				Escribir " + Waiting for Tito and other soccer stories (Eduardo Sacheri)"{;}
				Escribir " + Ender's Game (Orson Scott Card)"{;}
				Escribir " + The Dream of Heroes (Adolfo Bioy Casares)"{;}
			2:
				Escribir "Recommended movies:"{;}
				Escribir " + Matrix (1999)"{;}
				Escribir " + The Last Samurai (2003)"{;}
				Escribir " + Cars (2006)"{;}
			3:
				Escribir "Recommended albums:"{;}
				Escribir " + Torn to Pieces (La Renga, 1996)"{;}
				Escribir " + Buffalo (La Mississippi, 2008)"{;}
				Escribir " + Gaia (Mago de Oz, 2003)"{;}
			4:
				Escribir "Recommended classic video games:"{;}
				Escribir " + Day of the Tentacle (LucasArts, 1993)"{;}
				Escribir " + Terminal Velocity (Terminal Reality/3D Realms, 1995)"{;}
				Escribir " + Death Rally (Remedy/Apogee, 1996)"{;}
			5:
				Escribir "Thank you, come back soon"{;}
			De otro modo:
				Escribir "Invalid option"{;}
		FinSegun
		Escribir "Press enter to continue"{;}
		Esperar Tecla{;}
	Hasta Que OP=5
{FinProceso}
