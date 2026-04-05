# Architecture de localisation syntaxique

Date: 2026-03-24

## Carte logique officielle

Cette carte fait foi pour les evolutions futures, meme si l'arborescence physique historique reste provisoirement en place.

- `lang/` : UI du programme
- `keywords/` : syntaxe localisee et tables runtime des mots-cles
- `bin/help/` : aide runtime longue
- `bin/ejemplos/` : exemples localises
- `ejercicios/` : exercices

## Regles de separation

- `lang/*.txt` contient uniquement les chaines d'interface
- `keywords/<lang>.ini` contient les formes syntaxiques localisees, les alias, les tooltips courts, la categorie et le mapping d'aide
- `bin/help/<lang>/...` reste la source de l'aide longue
- `bin/ejemplos/<lang>/...` reste la source des exemples et exercices localises

## Portee actuelle

- la phase initiale ne traite que `LEER` et `ESCRIBIR`
- le pipeline doit rester scalable sans re-dupliquer les tables dans plusieurs `.cpp`
- si un fallback temporaire existe, il doit rester minimal et clairement transitoire

## Contrainte d'evolution

Toute nouvelle evolution doit respecter cette separation fonctionnelle, meme si le deplacement physique des dossiers est reporte.
