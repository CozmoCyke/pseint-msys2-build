Process buy_items
    Define n, i As Integer;
    Define amount, price As Real;
    amount <- 0;
    
    Write "Number of items:";
    Read n;
    
    Write "Prices:";
    For i <- 1 To n Do
        /// complete the missing pseudocode here
        /// to read each price and add it to amount
    EndFor
    
    If amount >= 500 Then
        /// complete the missing pseudocode here
        /// to apply the discount to amount
    EndIf
    
    Write "Amount to pay: $", amount;
EndProcess
