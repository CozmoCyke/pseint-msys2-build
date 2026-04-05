??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

{Proceso} TaTeTi
	
	// initializes two 3x3 matrices, one to store the visible mark, 
	// and another for a value associated with the mark; for one player it is 1, for
	// the other it is 2, so to determine who won you multiply par row, par 
	// column, and par diagonal; if the result is 1 the first player won,
	// if it is 8 the second won, if it is 0 there were still empty cells, and if
	// it is something else, all three marks are present but they are not from the same player
	{Definir Tab1 Como Entero}{;}
	{Definir Tab2 Como Caracter}{;}
	Dimension Tab1[3,3]{;}
	Dimension Tab2[3,3]{;}
	{Definir i, j, CantTurnos, Valor, Pos Como Entero}{;}
	{Definir Objetivo, aux_i, aux_j, aux_d1, aux_d2 Como Entero}{;}
	{Definir Terminado, HayGanador Como Logico}{;}
	{Definir Ficha Como Caracter;}{;}
	Para i <- {1#0} Hasta {3#2} Hacer
		Para j <- {1#0} Hasta {3#2} Hacer
			Tab1[i,j] <- 0{;}
			Tab2[i,j] <- " "{;}
		FinPara
	FinPara
	Terminado <- Falso{;}
	HayGanador <- Falso{;}
	CantTurnos <- 0{;}
	
	Mientras ~ Terminado hacer
		
		// draw the board
		Borrar Pantalla{;}
		Escribir " "{;}
		Escribir "      ||     ||     "{;}
		Escribir "   ",Tab2[{1#0},{1#0}],"  ||  ",Tab2[{1#0},{2#1}],"  ||  ",Tab2[{1#0},{3#2}]{;}
		Escribir "     1||    2||    3"{;}
		Escribir " =====++=====++======"{;}
		Escribir "      ||     ||     "{;}
		Escribir "   ",Tab2[{2#1},{1#0}],"  ||  ",Tab2[{2#1},{2#1}],"  ||  ",Tab2[{2#1},{3#2}]{;}
		Escribir "     4||    5||    6"{;}
		Escribir " =====++=====++======"{;}
		Escribir "      ||     ||     "{;}
		Escribir "   ",Tab2[{3#2},{1#0}],"  ||  ",Tab2[{3#2},{2#1}],"  ||  ",Tab2[{3#2},{3#2}]{;}
		Escribir "     7||    8||    9"{;}
		Escribir " "{;}
		
		Si ~ HayGanador y CantTurnos<9 Entonces
			
			// load helper values according to whose turn it is
			CantTurnos <- CantTurnos+1{;}
			Si CantTurnos%2=1 Entonces
				Ficha <- 'X'; Valor <-  1; Objetivo <- 1{;}
				Escribir "Player 1 turn (X)"{;}
			SiNo
				Ficha <- 'O'; Valor <-  2; Objetivo <- 8{;}
				Escribir "Player 2 turn (O)"{;}
			FinSi
			
			// ask for the position to place the mark and validate it
			Escribir "Enter the Position (1-9):"{;}
			
			Repetir
				Leer Pos{;}
				Si Pos<1 o Pos>9 Entonces
					Escribir "Incorrect position, enter it again: "{;}
					Pos <- 99;
				SiNo
					i <- trunc((Pos-1)/3){+1#}{;}
					j <- ((Pos-1) {%} 3){+1#}{;}
					Si Tab1[i,j]<>0 Entonces
						pos <- 99{;}
						Escribir "Incorrect position, enter it again: "{;}
					FinSi
				FinSi
			Hasta Que Pos<>99
			// store the mark in tab2 and the value in tab1
			Tab1[i,j] <- Valor{;}
			Tab2[i,j] <- Ficha{;}
			
			// check whether someone won par looking for the target product on the board
			aux_d1 <- 1; aux_d2 <- 1{;}
			Para i <- {1#0} hasta {3#2} hacer
				aux_i <- 1; aux_j <- 1{;}
				aux_d1 <- aux_d1*Tab1[i,i]{;}
				aux_d2 <- aux_d2*Tab1[i,{4#2}-i]{;}
				Para j <- {1#0} hasta {3#2} hacer
					aux_i <- aux_i*Tab1[i,j]{;}
					aux_j <- aux_j*Tab1[j,i]{;}
				FinPara
				Si aux_i=Objetivo o aux_j=Objetivo Entonces
					HayGanador <- Verdadero{;}
				FinSi
			FinPara
			Si aux_d1=Objetivo o aux_d2=Objetivo Entonces
				HayGanador <- Verdadero{;}
			FinSi
			
		SiNo
			
			Si HayGanador Entonces
				Escribir "There is a winner: " sin saltar{;}
				Si CantTurnos%2=1 Entonces
					Escribir "Player 1!"{;}
				SiNo
					Escribir "Player 2!"{;}
				FinSi
			SiNo
				Escribir "Tie!"{;}
			FinSi
			Terminado <- Verdadero{;}
			
		FinSi
		
	FinMientras
	
{FinProceso}
