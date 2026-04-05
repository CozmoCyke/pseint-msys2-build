"OPEN THESE EXAMPLES FROM HELP ( HELP MENU -> EXAMPLES ) otherwise they cannot be executed"

// Example problem:

//    Enter the prices of 5 items and the quantities sold
// by a company in its 4 branches. Report:
//    * The total quantities of each item.
//    * The quantity of items in branch 2.
//    * The quantity of item 3 in branch 1.
//    * The total revenue of each branch.
//    * The company's total revenue.
//    * The branch with the highest revenue.
   
{Proceso} Sucursales
   
   {Definir Prec,Cant,I,J,Suma,NumMayor,Suc2,MayorRec,TotSuc,TotEmp Como Numeros}{;}
   
   // Initialize Prec=price array and Cant=quantity array
   Dimension Prec[5], Cant[4,5]{;}
   
   // Read prices
   Para I<-{1#0} Hasta {5#4} Hacer
      Escribir 'Enter price for item ',{I#I+1},':'{;}
      Leer Prec[I]{;}
   FinPara
   
   // Read quantities
   Para J<-{1#0} Hasta {4#3} Hacer
      Para I<-{1#0} Hasta {5#4} Hacer
         Escribir 'Enter quantity of item ',{I#I+1},', in branch ',{J#J+1},':'{;}
         Leer Cant[J,I]{;}
      FinPara
   FinPara
   
   // Sum quantities by item
   Escribir 'Quantities by item:'{;}
   Para I<-{1#0} Hasta {5#4} Hacer
      Suma<-Cant[{1#0},I]+Cant[{2#1},I]+Cant[{3#2},I]+Cant[{4#3},I]{;}
      Escribir 'Total item ',{I#I+1},':',Suma{;}
   FinPara
   
   // Report total items in Branch 2
   Suc2<-0{;}
   Para I<-{1#0} Hasta {5#4} Hacer
      Suc2<-Suc2+Cant[{2#1},I]{;}
   FinPara
   Escribir 'Total Branch 2:',Suc2{;}
   
   // Report Branch 1, Item 3:
   Escribir 'Branch 1, Item 3:',Cant[{1,3#0,2}]{;}
   
   
   // Accumulate the total of each branch (TotSuc) and
   // the company total (TotEmp)
   MayorRec<-0; NumMayor<-0; TotEmp<-0{;}
   Para J<-{1#0} Hasta {4#3} Hacer
      TotSuc<-0{;}
      Para I<-{1#0} Hasta {5#4} Hacer
         TotSuc<-TotSuc+(Cant[J,I]*Prec[i]){;}
      FinPara
      Escribir 'Revenue of branch ',J,':',TotSuc{;}
      Si TotSuc>MayorRec entonces 
		 MayorRec<-TotSuc; NumMayor<-{J#J+1}{;} 
      FinSi
      TotEmp<-TotEmp+TotSuc{;}
   FinPara
   Escribir 'Total company revenue:',TotEmp{;}
   Escribir 'Branch with Highest Revenue:',NumMayor{;}

{FinProceso}

