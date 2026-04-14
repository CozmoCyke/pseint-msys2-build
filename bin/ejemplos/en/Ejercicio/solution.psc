Process buy_items
    Define n, i As Integer;
    Define amount, price As Real;
    amount <- 0;
    
    Write "Number of items:";
    Read n;
    
    Write "Prices:";
    For i <- 1 To n Do
        Read price;
        amount <- amount + price;
    EndFor
    
    If amount >= 500 Then
        amount <- amount * 0.95;
    EndIf
    
    Write "Amount to pay: $", amount;
EndProcess
