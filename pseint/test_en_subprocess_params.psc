Subprocess IncrementCopy(a By Value)
    a <- a + 1
    Write a
EndSubprocess

Subprocess IncrementRef(b By Reference)
    b <- b + 2
EndSubprocess

Algorithm TestSubprocessParams
    Define x As Integer
    x <- 10
    IncrementCopy(x);
    IncrementRef(x);
    Write x
EndAlgorithm
