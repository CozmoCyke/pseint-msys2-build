"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas etre executes"

// Montre comment realiser un menu simple avec les structures Repeter-Hasta Que et Segun

{Proceso} sin_titulo
	{Definir OP Como Entero}{;}
	Repetir
		// afficher le menu
		Limpiar Pantalla{;}
		Escribir "Menu de recommandations"{;}
		Escribir "   1. Litterature"{;}
		Escribir "   2. Films"{;}
		Escribir "   3. Musique"{;}
		Escribir "   4. Jeux video"{;}
		Escribir "   5. Quitter"{;}
		// saisir une option
		Escribir "Choisissez une option (1-5) : "{;}
		Leer OP{;}
		// traiter cette option
		Segun OP Hacer
			1:
				Escribir "Lectures recommandees :"{;}
				Escribir " + En attendant Tito et autres recits de football (Eduardo Sacheri)"{;}
				Escribir " + La Strategie Ender (Orson Scott Card)"{;}
				Escribir " + Le Reve des heros (Adolfo Bioy Casares)"{;}
			2:
				Escribir "Films recommandes :"{;}
				Escribir " + Matrix (1999)"{;}
				Escribir " + Le Dernier Samourai (2003)"{;}
				Escribir " + Cars (2006)"{;}
			3:
				Escribir "Albums recommandes :"{;}
				Escribir " + Despedazado por mil partes (La Renga, 1996)"{;}
				Escribir " + Bufalo (La Mississippi, 2008)"{;}
				Escribir " + Gaia (Mago de Oz, 2003)"{;}
			4:
				Escribir "Jeux video classiques recommandes :"{;}
				Escribir " + Day of the Tentacle (LucasArts, 1993)"{;}
				Escribir " + Terminal Velocity (Terminal Reality/3D Realms, 1995)"{;}
				Escribir " + Death Rally (Remedy/Apogee, 1996)"{;}
			5:
				Escribir "Merci et a bientot"{;}
			De otro modo:
				Escribir "Option non valide"{;}
		FinSegun
		Escribir "Appuyez sur Entree pour continuer"{;}
		Esperar Tecla{;}
	Hasta Que OP=5
{FinProceso}
