SousProgramme IncrementerCopie(a Par Valeur)
    a <- a + 1
    Ecrire a
FinSousProgramme

SousProgramme IncrementerReference(b Par Reference)
    b <- b + 2
FinSousProgramme

Algorithme TestSousProgrammeParams
    Définir x Comme Entier
    x <- 10
    IncrementerCopie(x);
    IncrementerReference(x);
    Ecrire x
FinAlgorithme
