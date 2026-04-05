# Nettoyage cible des fichiers de langue apres la Phase 1

Date: 2026-03-22

Perimetre:
- `lang/en.txt`
- `lang/fr.txt`
- `lang/es.txt`

Objectif:
- reduire les incoherences de labels de menu et de raccourcis
- sans refactor global
- sans changer les handlers, IDs ou le comportement voulu

## Strategie retenue

Nettoyage volontairement conservateur:
- correction des entrees exactes de menu manifestement fausses ou incompletes
- ajout des entrees exactes avec raccourci quand seul un libelle bare existait encore
- conservation des formes bare utiles aux toolbars, dialogues et autres contexts non-menu
- aucune suppression agressive quand une entree pouvait encore servir ailleurs

## Tableau par langue

### `lang/en.txt`

| Cle source | Traduction avant | Type | Action probable | Recommandation | Action prise |
| --- | --- | --- | --- | --- | --- |
| `&Ayuda` | `&Help1` | exacte / erronee | menu Aide | corriger le libelle exact | corrige en `&Help` |
| `Comentar Lineas\tCtrl+D` | absente | normalisee uniquement | menu Editer > commenter | ajouter une entree exacte coherente | ajoutee |
| `Descomentar Lineas\tCtrl+Shift+D` | absente | normalisee uniquement | menu Editer > decommenter | ajouter une entree exacte coherente | ajoutee |
| `Mover Hacia Arriba\tCtrl+T` | absente | normalisee uniquement | menu Editer > monter lignes | ajouter une entree exacte coherente | ajoutee |
| `Mover Hacia Abajo\tCtrl+Shift+T` | absente | normalisee uniquement | menu Editer > descendre lignes | ajouter une entree exacte coherente | ajoutee |
| `Comprobar Sintaxis Mientras Escribe\tF12` | absente | normalisee uniquement | menu Configurer > assistance | ajouter une entree exacte coherente | ajoutee |
| `Predefinir Entrada...\tCtrl+F9` | absente | normalisee uniquement | menu Executer | ajouter une entree exacte coherente | ajoutee |
| `Abrir...` / `&Abrir...\tCtrl+O` | `Open...` / `&Open...\tCtrl+O` | doublon exact + bare | menu + toolbar/dialogues | conserver les 2 formes, elles servent a des contexts differents | laisse intact |
| `Guardar` / `&Guardar\tCtrl+S` | `Save` / `&Save\tCtrl+S` | doublon exact + bare | menu + toolbar | conserver les 2 formes | laisse intact |
| `Ejecutar` / `Ejecutar\tF9` | `Run` / `Run\tF9` | doublon exact + bare | menu + toolbar | conserver les 2 formes | laisse intact |

### `lang/fr.txt`

| Cle source | Traduction avant | Type | Action probable | Recommandation | Action prise |
| --- | --- | --- | --- | --- | --- |
| `Guardar &Como...\tCtrl+Shift+S` | `Guardar &Como...\tCtrl+Shift+S` | exacte / non traduite | menu Fichier > enregistrer sous | corriger la traduction exacte | corrigee en `Enregistrer &sous...\tCtrl+Shift+S` |
| `Imprimir...\tCtrl+P` | `Imprimir...\tCtrl+P` | exacte / non traduite | menu Fichier > imprimer | corriger la traduction exacte | corrigee |
| `Indice...\tF1` | `Indice...\tF1` | exacte / partiellement traduite | menu Aide > index | aligner avec la forme bare deja presente | corrigee en `Index...\tF1` |
| `Pantalla Completa\tF11` | `Pantalla Completa\tF11` | exacte / non traduite | menu Presentation > plein ecran | corriger la traduction exacte | corrigee |
| `Seleccionar Todo\tCtrl+A` | `Seleccionar Todo\tCtrl+A` | exacte / non traduite | menu Editer > selectionner tout | corriger la traduction exacte | corrigee |
| `Verificar Sintaxis\tShift+F9` | `Verificar Sintaxis\tShift+F9` | exacte / non traduite | menu Executer > verifier | corriger la traduction exacte | corrigee |
| `Comentar Lineas\tCtrl+D` | absente | normalisee uniquement | menu Editer > commenter | ajouter une entree exacte coherente | ajoutee |
| `Descomentar Lineas\tCtrl+Shift+D` | absente | normalisee uniquement | menu Editer > decommenter | ajouter une entree exacte coherente | ajoutee |
| `Mover Hacia Arriba\tCtrl+T` | absente | normalisee uniquement | menu Editer > monter lignes | ajouter une entree exacte coherente | ajoutee |
| `Mover Hacia Abajo\tCtrl+Shift+T` | absente | normalisee uniquement | menu Editer > descendre lignes | ajouter une entree exacte coherente | ajoutee |
| `Comprobar Sintaxis Mientras Escribe\tF12` | absente | normalisee uniquement | menu Configurer > assistance | ajouter une entree exacte coherente | ajoutee |
| `Predefinir Entrada...\tCtrl+F9` | absente | normalisee uniquement | menu Executer | ajouter une entree exacte coherente | ajoutee |
| `Abrir...` / `&Abrir...\tCtrl+O` | `Ouvrir...` / `&Ouvrir...\tCtrl+O` | doublon exact + bare | menu + toolbar/dialogues | conserver les 2 formes | laisse intact |

### `lang/es.txt`

| Cle source | Traduction avant | Type | Action probable | Recommandation | Action prise |
| --- | --- | --- | --- | --- | --- |
| `Generar Log` | `Generate Log` | bare / mauvaise langue | action bare non-menu | corriger en espagnol | corrigee |
| `Guardar Como...` | `Guardar Com...` | bare / incomplete | bare + fallback menu | corriger pour eviter un label tronque | corrigee |
| `&Guardar\tCtrl+S` | absente | exacte absente | menu Fichier > enregistrer | ajouter la forme exacte pour reduire l'ambiguite | ajoutee |
| `Guardar &Como...\tCtrl+Shift+S` | absente | exacte absente | menu Fichier > enregistrer sous | ajouter la forme exacte pour reduire l'ambiguite | ajoutee |
| reste du fichier | tres minimal | volontaire | UI espagnole basee surtout sur les sources | ne pas gonfler artificiellement le fichier en Phase 1 | laisse intact |

## Suppressions / fusions effectuees

Aucune suppression agressive.

Raison:
- les formes bare restent utiles en dehors des menus principaux
- maintenant que la Phase 1 cote code distingue lookup exact et fallback normalise, le risque de collision comportementale est beaucoup plus faible
- supprimer ces formes sans cartographie complete de tous les usages serait premature

## Cas laisses volontairement intacts

- doublons exact + bare qui servent encore a des contexts differents
- entrees bare repetes a l'identique plus loin dans les fichiers quand leur suppression n'apporte pas de gain immediat et pourrait brouiller l'historique
- traductions non liees aux menus/raccourcis

## Entrees encore ambigues a traiter en Phase 2

### Commun a `en` et `fr`
- plusieurs formes bare existent encore en double ou en triple dans le fichier
- certaines cles proches ne different que par la casse ou la ponctuation, par exemple `Seleccionar Todo` / `Seleccionar todo`
- des blocs melangent encore menu, toolbar, dialogues et aide dans un meme fichier plat

### `lang/en.txt`
- duplications bare repetitives pour `Abrir...`, `Guardar`, `Guardar Como...`, `Buscar Anterior`, `Buscar Siguiente`, `Cortar`, `Copiar`, `Corregir Indentado`
- `Buscar=Find` apparait plus d'une fois

### `lang/fr.txt`
- memes duplications bare que `en`
- plusieurs cles source espagnoles sont conservees comme cles, ce qui reste normal dans l'architecture actuelle mais complique le nettoyage manuel

### `lang/es.txt`
- fichier encore extremement minimal
- pour l'instant, la plupart des labels espagnols proviennent des sources du code plutot que du fichier de langue
- un enrichissement complet serait de la Phase 2 ou d'un chantier de localisation separe

## Resume

- Le nettoyage applique reduit la dette directement utile aux menus et raccourcis.
- Les formes exactes avec accelerateur sont maintenant plus completes et plus coherentes.
- Les formes bare potentiellement utiles ont ete conservees.
- Les vrais restes de dette sont surtout des duplications textuelles et le melange de contexts dans les fichiers plats, a traiter plus proprement en Phase 2.
