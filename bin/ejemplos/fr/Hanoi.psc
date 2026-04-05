??"OUVREZ CES EXEMPLES DEPUIS L'AIDE ( MENU AIDE -> EXEMPLES ) sinon ils ne peuvent pas être exécutés"

//    The goal of the game is to move the disks from tower 1 to tower 3 in the
// smallest number of moves possible. A larger disk cannot be placed 
// on top of a smaller one

//     There is a matrix that represents the towers; each column contains
//  numbers that represent the sizes of the disks in those towers (only
//  the values up to the number of disks in that tower matter).
//  The cant_discos vector tells how many disks each tower has. 

{Proceso} Hanoi
	
	{Definir torres,cant_discos,discos,i Como Enteros}{;}
	Dimension torres[3,10], cant_discos[3]{;}
	
	// ask for and validate how many disks to place on the first tower
	Escribir "Entrez le nombre de disques (1-8) :"{;}
	leer discos{;}
	mientras discos<1 {|} discos>8 Hacer
		Escribir "Le nombre de disques doit être supérieur à 0 et inférieur à 5 :"{;}
		leer discos{;}
	finmientras

	// initialize the data
	cant_discos[{1#0}]<-discos{;}
	cant_discos[{2#1}]<-0{;}
	cant_discos[{3#2}]<-0{;}
	Para i<-{1#0} Hasta {discos#discos-1} hacer
		torres[{1#0},i]<-discos-i+1{;}
	FinPara
	
	
	// play!
	{Definir j,t0,t1,t2,disco_a_mover,cant_movs Como Enteros}{;}
	{Definir puede_mover Como Logico}{;}
	cant_movs<-0{;}
	Mientras cant_discos[{3#2}]<>discos Hacer // while not all disks are on the third tower, the game continues
		
		Limpiar Pantalla{;}
		
		Para i<-{1#0} Hasta {3#2} Hacer // draw the three towers
			escribir "Tower ",i{#+1}{;}
			si cant_discos[i]=0 Entonces
				Escribir ""{;}
			sino
				para j<-cant_discos[i]{#-1} hasta {1#0} con paso -1 Hacer // go through the disks in the tower from top to bottom
					segun torres[i,j] Hacer // draw each disk
						1: Escribir "                   XX"{;}
						2: Escribir "                 XXXXXX"{;}
						3: Escribir "               XXXXXXXXXX"{;}
						4: Escribir "             XXXXXXXXXXXXXX"{;}
						5: Escribir "           XXXXXXXXXXXXXXXXXX"{;}
						6: Escribir "         XXXXXXXXXXXXXXXXXXXXXX"{;}
						7: Escribir "       XXXXXXXXXXXXXXXXXXXXXXXXXX"{;}
						8: Escribir "     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"{;}
					finsegun
				FinPara
			finsi
			Escribir "   ----------------------------------"{;}
			Escribir ""{;}
		FinPara
		
		// request a move
		Escribir "Move from tower: "{;}
		leer t1{;}
		Escribir "to tower: "{;}
		leer t2{;}
		
		si t1<1 {|} t1>3 {|} t2<1 {|} t2>3 Entonces // check that the tower number is valid
			Escribir "Invalid move"{;}
			Esperar Tecla{;}
		sino
			Si cant_discos[t1{#-1}]=0 Entonces // check that the source tower has at least one disk
				Escribir "Invalid move"{;}
				Esperar Tecla{;}
			SiNo
				disco_a_mover <- torres[t1{#-1},cant_discos[t1{#-1}]{#-1}]{;} // get the size of the disk to be moved
				puede_mover<-verdadero{;}
				Si cant_discos[t2{#-1}]<>0 entonces // check that the destination tower is empty or has only larger disks
					Si torres[t2{#-1},cant_discos[t2{#-1}]{#-1}]<disco_a_mover Entonces
						puede_mover<-Falso{;}
					FinSi
				FinSi
				Si puede_mover Entonces // if all checks passed, move it
					cant_movs <- cant_movs+1{;}
					cant_discos[t2{#-1}]<-cant_discos[t2{#-1}]+1{;}
					torres[t2{#-1},cant_discos[t2{#-1}]{#-1}] <- disco_a_mover{;}
					cant_discos[t1{#-1}]<-cant_discos[t1{#-1}]-1{;}
				SiNo
					Escribir "Invalid move"{;}
					Esperar Tecla{;}
				FinSi
			FinSi
		FinSi
	
	FinMientras
	
	// show result
	Limpiar Pantalla{;}
	Escribir "Game finished in ",cant_movs," moves!"{;}
	
{FinProceso}
