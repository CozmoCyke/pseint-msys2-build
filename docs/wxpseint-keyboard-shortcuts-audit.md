# Audit complet des raccourcis clavier de wxPSeInt

Date: 2026-03-22

Perimetre audite:
- `wxPSeInt/` en priorite: menu principal, accelerateurs globaux, editeur `mxSource`, dialogues, panneaux, menus contextuels.
- Verification repo-wide complementaire: `lang/` pour la localisation et, en annexe, `psterm/` et `psdraw3/` comme composants connexes appeles par wxPSeInt.

Contraintes respectees:
- Aucun changement de comportement n'a ete applique.
- Rapport en lecture seule.
- Les raccourcis implicites/incertains sont marques `probable` quand necessaire.

## Methode

Recherches repo-wide effectuees sur:
- labels de menu avec `\tCtrl`, `\tAlt`, `\tShift`, `\tF...`
- `SetAcceleratorTable`, `wxAcceleratorEntry`, `wxAcceleratorTable`
- `EVT_MENU`, `EVT_CHAR_HOOK`, `EVT_KEY_DOWN`, `EVT_KEY_UP`
- menus contextuels (`PopupMenu`, `wxMenu`)
- capture clavier dans l'editeur et les dialogues
- fichiers de localisation `lang/*.txt`

## Source de verite reelle

Il y a 3 sources de verite distinctes, et elles divergent aujourd'hui:

1. Raccourcis de menu
- Source reelle: le suffixe `\tCtrl+...` ou `\tF...` dans le label du menu.
- Effet: ce suffixe sert a la fois d'affichage UI et d'accelerateur wxWidgets.
- Risque: si la traduction remplace le label par une valeur sans suffixe `\t...`, le raccourci peut disparaitre visuellement et cesser d'etre actif.

2. Accelerateurs globaux
- Source reelle: `wxPSeInt/mxMainWindow.cpp:1648-1659` via `SetAcceleratorTable`.
- Effet: actifs meme sans menu correspondant.

3. Captures clavier locales / implicites
- Source reelle: handlers `EVT_KEY_DOWN`, `EVT_CHAR_HOOK`, `ControlDown()`, `SetEscapeId`, `SetAffirmativeId`, `SetDefault`.
- Effet: actifs seulement dans certains widgets ou dialogues.

## Constat majeur

Le probleme principal ne vient pas seulement des menus: il vient surtout de la localisation.

Dans `wxPSeInt/LocalizationManager.cpp`:
- `normalize_key_ws()` supprime `&` et tout ce qui suit `\t` avant lookup (`:50-56`).
- `Translate()` utilise cette cle normalisee (`:197`).
- `LoadFile()` stocke les traductions dans une map sur cette meme cle normalisee (`:164`), donc la derniere occurrence gagne.

Consequence directe:
- une entree `Abrir...\tCtrl+O=Open...\tCtrl+O`
- et plus bas une entree `Abrir...=Open...`

finissent par partager exactement la meme cle normalisee, et la seconde ecrase la premiere.

Donc dans les UI localisees EN/FR:
- le suffixe `\tCtrl+...` disparait,
- le raccourci n'est plus seulement "cache": il est probablement mort car wxWidgets derive l'accelerateur du label de menu final.

## Inventaire complet

### A. Menu principal et labels de menu

| Action | Raccourci affiche dans le code | Raccourci reellement cable | Fichier / ligne | Portee | Visible dans l'UI ? | Localisation correcte ? | Remarques |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Nouveau | `Ctrl+N` | via label de menu | `wxPSeInt/mxMainWindow.cpp:259` | globale | ES: oui, EN/FR: non | non | EN/FR ont des doublons normalises (`lang/en.txt:3` ecrase par `:141`, `lang/fr.txt:3` ecrase par `:141`) |
| Ouvrir | `Ctrl+O` | via label de menu | `wxPSeInt/mxMainWindow.cpp:260` | globale | ES: oui, EN/FR: non | non | meme probleme de doublon de traduction |
| Enregistrer | `Ctrl+S` | via label de menu | `wxPSeInt/mxMainWindow.cpp:261` | globale | ES: non, EN/FR: non | non | ES perd deja le suffixe via `lang/es.txt:4`; EN/FR ecrases plus bas |
| Enregistrer sous | `Ctrl+Shift+S` | via label de menu | `wxPSeInt/mxMainWindow.cpp:262` | globale | ES: non, EN/FR: non/probable | non | ES `lang/es.txt:5` est tronque; EN/FR ont doublons et/ou traduction incoherente |
| Editer le diagramme de flux | `F7` | via label de menu | `wxPSeInt/mxMainWindow.cpp:263` | globale | ES: oui, EN/FR: non | non | EN `lang/en.txt:144`, FR `lang/fr.txt:144` sans `\tF7` |
| Imprimer | `Ctrl+P` | via label de menu | `wxPSeInt/mxMainWindow.cpp:264` | globale | ES: oui, EN/FR: non/probable | non | FR conserve meme le texte espagnol plus haut puis l'ecrase plus bas |
| Fermer | `Ctrl+W` | via label de menu | `wxPSeInt/mxMainWindow.cpp:278` | globale | ES: oui, EN/FR: non | non | doublons EN/FR |
| Quitter | `Alt+F4` | via label de menu | `wxPSeInt/mxMainWindow.cpp:280` | globale | ES: oui, EN/FR: non | non | doublons EN/FR |
| Annuler | `Ctrl+Z` | via label de menu | `wxPSeInt/mxMainWindow.cpp:286` | globale | ES: oui, EN/FR: non | non | doublons EN/FR |
| Retablir | `Ctrl+Shift+Z` | via label de menu | `wxPSeInt/mxMainWindow.cpp:287` | globale | ES: oui, EN/FR: non | non | doublons EN/FR |
| Couper | `Ctrl+X` | via label de menu | `wxPSeInt/mxMainWindow.cpp:289` | globale / editeur | ES: oui, EN/FR: non | non | doublons EN/FR |
| Copier | `Ctrl+C` | via label de menu | `wxPSeInt/mxMainWindow.cpp:290` | globale / editeur | ES: oui, EN/FR: non | non | doublons EN/FR |
| Coller | `Ctrl+V` | via label de menu | `wxPSeInt/mxMainWindow.cpp:291` | globale / editeur | ES: oui, EN/FR: non | non | doublons EN/FR |
| Monter les lignes | `Ctrl+T` | via label de menu | `wxPSeInt/mxMainWindow.cpp:292` | editeur | ES: oui, EN/FR: non | non | EN `lang/en.txt:172`, FR `lang/fr.txt:172` sans suffixe |
| Descendre les lignes | `Ctrl+Shift+T` | via label de menu | `wxPSeInt/mxMainWindow.cpp:293` | editeur | ES: oui, EN/FR: non | non | EN `:174`, FR `:174` sans suffixe |
| Rechercher | `Ctrl+F` | via label de menu | `wxPSeInt/mxMainWindow.cpp:295` | editeur / globale | ES: oui, EN/FR: non | non | EN `:175`, FR `:175` ecrasent les versions avec suffixe |
| Rechercher precedent | `Shift+F3` | via label de menu | `wxPSeInt/mxMainWindow.cpp:296` | editeur / globale | ES: oui, EN/FR: non | non | EN `:176`, FR `:176` sans suffixe |
| Rechercher suivant | `F3` | via label de menu | `wxPSeInt/mxMainWindow.cpp:297` | editeur / globale | ES: oui, EN/FR: non | non | EN `:177`, FR `:177` sans suffixe |
| Remplacer | `Ctrl+R` | via label de menu | `wxPSeInt/mxMainWindow.cpp:298` | editeur / globale | ES: oui, EN/FR: non | non | doublons EN/FR |
| Selectionner tout | `Ctrl+A` | via label de menu | `wxPSeInt/mxMainWindow.cpp:300` | editeur | ES: oui, EN/FR: non | non | EN `:179`, FR `:179` sans suffixe |
| Dupliquer lignes | `Ctrl+L` | via label de menu | `wxPSeInt/mxMainWindow.cpp:301` | editeur | ES: oui, EN/FR: non | non | EN `:180`, FR `:180` sans suffixe |
| Supprimer lignes | `Ctrl+Shift+L` | via label de menu | `wxPSeInt/mxMainWindow.cpp:302` | editeur | ES: oui, EN/FR: non | non | EN `:181`, FR `:181` sans suffixe |
| Commenter lignes | `Ctrl+D` | via label de menu | `wxPSeInt/mxMainWindow.cpp:303` | editeur | ES: oui, EN/FR: non | non | EN `:182`, FR `:182` sans suffixe |
| Decommenter lignes | `Ctrl+Shift+D` | via label de menu | `wxPSeInt/mxMainWindow.cpp:304` | editeur | ES: oui, EN/FR: non | non | EN `:183`, FR `:183` sans suffixe |
| Corriger l'indentation | `Ctrl+I` | via label de menu | `wxPSeInt/mxMainWindow.cpp:306` | editeur | ES: oui, EN/FR: non | non | EN `:184`, FR `:184` sans suffixe |
| Emprolijar Algoritmo | `Ctrl+Shift+I` | commente | `wxPSeInt/mxMainWindow.cpp:307` | aucune | non | n/a | raccourci mort/commented-out, a ne pas documenter comme actif |
| Verifier syntaxe pendant la saisie | `F12` | via label de menu check-item | `wxPSeInt/mxMainWindow.cpp:318` | globale | ES: oui, EN/FR: non | non | EN `:191`, FR `:191` sans suffixe |
| Plein ecran | `F11` | via label de menu check-item | `wxPSeInt/mxMainWindow.cpp:323` | globale | ES: oui, EN/FR: non | non | EN `:194`, FR `:194` sans suffixe |
| Executer | `F9` | via label de menu | `wxPSeInt/mxMainWindow.cpp:347` | globale | ES: oui, EN/FR: non | non | EN `:210`, FR `:210` sans suffixe |
| Executer pas a pas | `F5` | via label de menu | `wxPSeInt/mxMainWindow.cpp:348` | globale | ES: oui, EN/FR: non | non | EN `:211`, FR `:211` sans suffixe |
| Execution expliquee | aucun | aucun raccourci explicite | `wxPSeInt/mxMainWindow.cpp:349` | globale | n/a | oui | action menu sans raccourci |
| Verifier la syntaxe | `Shift+F9` | via label de menu | `wxPSeInt/mxMainWindow.cpp:350` | globale | ES: oui, EN/FR: non | non | EN `:213`, FR `:213` sans suffixe |
| Predefinir l'entree | `Ctrl+F9` | via label de menu | `wxPSeInt/mxMainWindow.cpp:352` | globale | ES: oui, EN/FR: non | non | EN `:214`, FR `:214` sans suffixe |
| Index de l'aide | `F1` | via label de menu | `wxPSeInt/mxMainWindow.cpp:356` | globale | ES: oui, EN/FR: non | non | EN `:216`, FR `:216` sans suffixe |
| Aide rapide | `Ctrl+F1` | via label de menu | `wxPSeInt/mxMainWindow.cpp:358` | globale / editeur | ES: oui, EN/FR: non | non | EN/FR ont aussi des doublons bare qui ecrasent |

### B. Accelerateurs globaux hors menus

| Action | Raccourci affiche | Raccourci reellement cable | Fichier / ligne | Portee | Visible dans l'UI ? | Localisation correcte ? | Remarques |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Onglet precedent | aucun | `Ctrl+Shift+Tab` | `wxPSeInt/mxMainWindow.cpp:1650` | globale | non | n/a | accel cache, non documente |
| Onglet suivant | aucun | `Ctrl+Tab` | `wxPSeInt/mxMainWindow.cpp:1651` | globale | non | n/a | accel cache, non documente |
| Onglet precedent | aucun | `Ctrl+PageUp` | `wxPSeInt/mxMainWindow.cpp:1652` | globale | non | n/a | doublon fonctionnel du precedent |
| Onglet suivant | aucun | `Ctrl+PageDown` | `wxPSeInt/mxMainWindow.cpp:1653` | globale | non | n/a | doublon fonctionnel du suivant |
| Options du langage / "Do That" | aucun | `Ctrl+Shift+F5` | `wxPSeInt/mxMainWindow.cpp:1654`, handler `:1682` | globale | non | n/a | raccourci cache, nom d'action peu parlant |
| Pas de debug | aucun | `F6` | `wxPSeInt/mxMainWindow.cpp:1655`, handler `:1856` | globale / debug | non | n/a | raccourci cache, pas reflechi dans les menus |
| Renommer variable | visible dans menu contextuel editeur | `Alt+Shift+Enter` | `wxPSeInt/mxMainWindow.cpp:1656`, `wxPSeInt/mxSource.cpp:2236` | editeur | partiellement | non | visible seulement dans un menu contextuel, pas dans le menu principal ni le panneau variables |
| Ajuster / reinitialiser le zoom | visible dans menu contextuel editeur | `Ctrl+0` | `wxPSeInt/mxMainWindow.cpp:1657`, `wxPSeInt/mxSource.cpp:2255` | editeur | partiellement | partiellement | accel global mais expose seulement dans le popup editeur |

### C. Editeur, composants contextuels et touches speciales

| Action | Raccourci affiche | Raccourci reellement cable | Fichier / ligne | Portee | Visible dans l'UI ? | Localisation correcte ? | Remarques |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Ouvrir le menu contextuel au clavier | aucun | touche `Menu` / `WXK_MENU` | `wxPSeInt/mxSource.cpp:2499-2505` | editeur | non | n/a | raccourci implicite/captures clavier |
| Fermer calltip / autocompletion / aide rapide | aucun | `Esc` | `wxPSeInt/mxSource.cpp:2506-2508` | editeur | non | n/a | implicite utile, totalement cache |
| Zoom par molette | aucun | `Ctrl+Wheel` | `wxPSeInt/mxSource.cpp:2203-2210` | editeur | non | n/a | implicite, non documente |
| Renommer variable depuis popup editeur | `Alt+Shift+Enter` | commande `mxID_VARS_RENAME` | `wxPSeInt/mxSource.cpp:2235-2237` | editeur | oui, popup seulement | non | absent du popup du panneau variables |
| Ajuster le zoom depuis popup editeur | `Ctrl+0` | commande `mxID_ZOOM_TO_SELECTION` | `wxPSeInt/mxSource.cpp:2255` | editeur | oui, popup seulement | partiellement | expose un accel global cache |
| Commenter / decommenter / indenter / selectionner tout / undo / redo / cut / copy / paste dans popup editeur | aucun | via `EVT_MENU` de l'editeur | `wxPSeInt/mxSource.cpp:78-90`, popup `:2242-2253` | editeur | non dans popup | n/a | actions existantes mais popup n'affiche pas leurs raccourcis existants |
| Meme actions dans panneau variables | aucun | via `EVT_MENU` panneau | `wxPSeInt/mxVarWindow.cpp:23-26`, popup `:157-161` | panneau variables | non | n/a | incoherent avec l'editeur: renommer n'y affiche pas `Alt+Shift+Enter` |
| Copier dans `mxHtmlWindow` | aucun | menu contextuel sans accel | `wxPSeInt/mxHtmlWindow.cpp:19-21` | aide/html | non | n/a | pas de `Ctrl+C` affiche dans le menu contextuel |
| Copier table / aller a la ligne dans Prueba de Escritorio | aucun | commandes contextuelles uniquement | `wxPSeInt/mxDesktopTestGrid.cpp:168-170` | panneau specifique | non | n/a | pas de raccourci explicite detecte |

### D. Dialogues et raccourcis implicites

| Action | Raccourci affiche | Raccourci reellement cable | Fichier / ligne | Portee | Visible dans l'UI ? | Localisation correcte ? | Remarques |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Evaluer expression | aucun | `Enter` / `Numpad Enter` | `wxPSeInt/mxEvaluateDialog.cpp:12`, `:97-99` | dialogue evaluation | non | n/a | explicite, utile, cache |
| Fermer dialogue d'evaluation | aucun | `Esc` | `wxPSeInt/mxEvaluateDialog.cpp:97-99` | dialogue evaluation | non | n/a | explicite, cache |
| Validation par bouton par defaut | probable | `Enter` via `SetAffirmativeId(wxID_OK)` | `wxPSeInt/mxEvaluateDialog.cpp:44` | dialogue evaluation | non | n/a | implicite wxWidgets |
| Fermer fenetre d'aide | aucun | `Esc` | `wxPSeInt/mxHelpWindow.cpp:32`, `:309-310` | fenetre aide | non | n/a | explicite, cache |
| Fermer dialogue de recherche/remplacement | probable | `Esc` via `SetEscapeId(wxID_CANCEL)` | `wxPSeInt/mxFindDialog.cpp:70` | dialogue recherche | non | n/a | implicite wxWidgets |
| Rechercher suivant dans dialogue | probable | `Enter` sur bouton par defaut | `wxPSeInt/mxFindDialog.cpp:103` | dialogue recherche | non | n/a | actif seulement en mode Find |
| Remplacer dans dialogue | probable | `Enter` sur bouton par defaut | `wxPSeInt/mxFindDialog.cpp:139` | dialogue remplacement | non | n/a | actif seulement en mode Replace |
| Autres dialogues standard | probable | `Esc`/`Enter` via `SetEscapeId`/`SetDefault` | ex. `mxConfig.cpp:55-56`, `mxFontsConfig.cpp:72-73`, `mxInputDialog.cpp:58-59`, `mxProfile.cpp:224-225`, `mxAboutWindow.cpp:44-45` | dialogues divers | non | n/a | comportement standard, non documente |

## Problemes detectes

### P0 - Racine du probleme: la localisation ecrase les accelerateurs de menu

Impact:
- les raccourcis de menu sont souvent invisibles dans les UI EN/FR,
- et tres probablement inactifs, car l'accelerateur wxWidgets depend du label final du menu.

Preuves:
- normalisation des cles: `wxPSeInt/LocalizationManager.cpp:50-56`
- stockage par cle normalisee: `wxPSeInt/LocalizationManager.cpp:164`
- lookup par cle normalisee: `wxPSeInt/LocalizationManager.cpp:197`
- exemples de doublons qui s'ecrasent:
  - EN: `lang/en.txt:3` puis `lang/en.txt:141`
  - EN: `lang/en.txt:42` puis `lang/en.txt:175`
  - FR: `lang/fr.txt:3` puis `lang/fr.txt:141`
  - FR: `lang/fr.txt:42` puis `lang/fr.txt:175`
  - ES: `lang/es.txt:4-5` suppriment deja les suffixes pour Save / Save As

### P1 - Plusieurs raccourcis reels sont totalement caches

Raccourcis caches detectes:
- `Ctrl+Tab`, `Ctrl+Shift+Tab`, `Ctrl+PageUp`, `Ctrl+PageDown`
- `Ctrl+Shift+F5`
- `F6`
- `Alt+Shift+Enter`
- `Ctrl+0`
- `Esc` dans l'editeur, l'aide, l'evaluation
- `Ctrl+Wheel` dans l'editeur

### P1 - Menus contextuels incomplets / incoherents

Problemes:
- le popup editeur n'affiche pas les raccourcis de nombreuses commandes qui en ont deja un dans le menu principal
- le popup du panneau variables n'affiche pas `Alt+Shift+Enter`
- le popup HTML n'affiche pas `Ctrl+C`

### P1 - Incoherence entre "action existante" et "visibilite UI"

Exemples:
- `Ctrl+0` est accelere globalement mais visible seulement dans le popup editeur
- `Alt+Shift+Enter` est accelere globalement mais visible seulement dans un popup de l'editeur
- `F6` est cable mais n'apparait nulle part

### P1 - Bug technique dans la table d'accelerateurs

Dans `wxPSeInt/mxMainWindow.cpp:1649-1658`:
- le tableau est declare `wxAcceleratorEntry entries[9];`
- seuls `entries[0]` a `entries[7]` sont initialises
- `wxAcceleratorTable accel(9, entries);` consomme 9 entrees

Conclusion:
- il y a tres probablement une entree non initialisee passee a wxWidgets.
- Ce n'est pas seulement une question d'UI: c'est une anomalie technique a corriger.

### P2 - Raccourcis commentes / morts

- `wxPSeInt/mxMainWindow.cpp:307` contient `Ctrl+Shift+I` pour `mxID_EDIT_BEAUTIFY_CODE`, mais la ligne est commentee.
- Il ne faut pas le presenter comme actif.

### P2 - Traductions incorrectes ou incoherentes

Exemples:
- `lang/es.txt` est manifestement incomplet et tronque `Guardar Como...`
- `lang/fr.txt` contient encore plusieurs valeurs espagnoles
- des doublons avec et sans `\t...` coexistent dans EN/FR, ce qui garantit des divergences futures

## Differences Windows / Linux detectables

- `Alt+F4` pour Quitter est un raccourci tres Windows-centric, mais il est defini explicitement dans le label de menu de wxPSeInt.
- `Ctrl+PageUp/PageDown`, `Ctrl+Tab`, `Ctrl+Shift+Tab` devraient etre multiplateformes via `wxAcceleratorTable`.
- Le popup via touche `Menu` (`WXK_MENU`) est surtout pertinent sur claviers PC/Windows; sur certaines plateformes il sera peu discoverable voire absent.
- Rien d'autre dans wxPSeInt ne montre de divergence conditionnelle explicite de raccourcis.

## Recommandations concretes

### 1. Faire apparaitre tous les raccourcis existants sans changer le comportement

Option minimale et sure recommandee:
- corriger la traduction au moment du rendu, pas action par action
- si la source contient `\tShortcut` et que la traduction resolue n'en contient pas, reattacher le suffixe original

Pourquoi:
- corrige d'un coup tous les menus localises
- n'ajoute aucun nouveau comportement
- reduit le risque de divergence future
- permet de garder les fichiers de traduction incomplets sans casser l'affichage des accelerateurs

Patch minimal propose, conceptuellement, dans `wxPSeInt/LocalizationManager.cpp`:
- apres lookup de la traduction:
  - si `raw` contient `\t`
  - et si la valeur traduite ne contient pas `\t`
  - alors retourner `translated + raw.Mid(raw.Find('\t'))`

Effet attendu:
- les raccourcis deja prevus dans les labels redeviennent visibles
- et redeviennent tres probablement actifs dans les menus EN/FR
- aucun handler ni ID n'est modifie

### 2. Centraliser la definition des raccourcis

Etat actuel:
- une partie vit dans les labels de menu
- une autre dans `SetAccelerators()`
- une autre encore dans des popups et handlers locaux

Recommandation:
- creer un petit registre central des commandes clavier, par exemple une table `CommandShortcutSpec`
- y stocker:
  - `command id`
  - `default shortcut`
  - `scope`
  - `visible in menu`
  - `visible in context menu`
  - `help/documentation label`

Puis:
- generer les labels de menu depuis cette table
- generer `wxAcceleratorTable` depuis cette table
- generer la page d'aide "Raccourcis clavier" depuis cette table

### 3. Gérer la localisation sans casser les accelerateurs

Recommandation:
- ne jamais faire dependre l'existence d'un accelerateur de la traduction libre
- separer le "texte traduit" et le "suffixe accelerateur"

Deux strategies propres:
- soit conserver le suffixe original automatiquement si la traduction n'en fournit pas
- soit stocker dans la table centrale `caption_key` et `shortcut_text` separement puis recomposer le label

La deuxieme option est la plus robuste a moyen terme.

### 4. Documenter les raccourcis

Ajouter une page d'aide dediee:
- "Raccourcis clavier"
- sections:
  - General
  - Edition
  - Navigation entre onglets
  - Debug
  - Dialogues
  - Menus contextuels

## Plan de correction minimal et sur

### Phase 1 - Rendre visibles les raccourcis deja existants

Objectif:
- zero changement fonctionnel volontaire
- remettre l'UI a niveau

Actions:
- corriger `wxPSeInt/LocalizationManager.cpp` pour preserver le suffixe `\t...` quand la traduction l'a perdu
- completer les popups qui reexposent des actions deja raccourcies:
  - `wxPSeInt/mxSource.cpp`
  - `wxPSeInt/mxVarWindow.cpp`
  - `wxPSeInt/mxHtmlWindow.cpp`

Effet:
- les menus et popups affichent les raccourcis existants
- les menus localises recuperent les accelerateurs perdus

### Phase 2 - Centraliser les definitions

Actions:
- introduire une table centrale des raccourcis
- remplacer les litteraux `\tCtrl+...` disperses
- reconstruire `SetAcceleratorTable()` depuis cette table

Correctif technique associe:
- corriger le bug `entries[9]` / `8 entrees initialisees`

### Phase 3 - Ajouter une page d'aide "Raccourcis clavier"

Actions:
- nouvelle page dans l'aide HTML
- lien depuis le menu Aide
- contenu genere ou maintenu a partir de la table centrale

### Phase 4 - Ajouter un dialogue / ecran "Keyboard Shortcuts"

Optionnel mais utile:
- fenetre listant tous les raccourcis par categorie
- recherche texte
- indication du scope

## Proposition de patch minimal

Patch minimal recommande en premier:

1. `wxPSeInt/LocalizationManager.cpp`
- reattacher automatiquement le suffixe `\t...` d'origine si la traduction resolue ne le contient pas

2. `wxPSeInt/mxSource.cpp`
- afficher dans le menu contextuel les raccourcis deja existants pour:
  - commenter / decommenter
  - indenter
  - selectionner tout
  - undo / redo
  - cut / copy / paste
  - renommer variable
  - ajuster zoom

3. `wxPSeInt/mxVarWindow.cpp`
- afficher `Alt+Shift+Enter` sur "Renombrar variable..."

4. `wxPSeInt/mxHtmlWindow.cpp`
- afficher `Ctrl+C` sur "Copiar"

5. `wxPSeInt/mxMainWindow.cpp`
- corriger la table d'accelerateurs (`8` entrees reelles aujourd'hui, pas `9`)
- a ce stade, pas besoin de changer les raccourcis eux-memes

## Fichiers a corriger en premier

Priorite 1:
- `wxPSeInt/LocalizationManager.cpp`
- `wxPSeInt/mxMainWindow.cpp`

Priorite 2:
- `wxPSeInt/mxSource.cpp`
- `wxPSeInt/mxVarWindow.cpp`
- `wxPSeInt/mxHtmlWindow.cpp`

Priorite 3:
- `lang/en.txt`
- `lang/fr.txt`
- `lang/es.txt`

Remarque:
- les fichiers `lang/*.txt` doivent etre nettoyes, mais ce n'est pas le meilleur premier levier. Tant que la normalisation ecrase les variantes avec et sans tab, la divergence reviendra facilement.

## Annexe - Composants connexes trouves hors wxPSeInt

Ces raccourcis existent dans le depot, mais appartiennent a des composants annexes:

### `psterm/`
- `Ctrl+V` et `Ctrl+C` via `psterm/mxConsole.cpp:100-104`

### `psdraw3/`
- `F1`, `F2`, `F3`, `F4`, `F5`, `F7`, `F9`, `F11`, `F12`
- fleches de deplacement
- detection de `Shift`, `Alt`, `Ctrl`
- references: `psdraw3/EntityEditor.cpp:226-241`, `psdraw3/Canvas.cpp:155-169`

Ces raccourcis ne font pas partie du shell principal wxPSeInt, mais ils meritent un audit separe si l'objectif UX couvre aussi les executables secondaires.

## Resume executif

- Le depot contient bien de nombreux raccourcis clavier.
- Le symptome "ils ne sont pas affiches" est reel.
- La cause dominante est la localisation: les traductions ecrasent les labels avec accelerateurs.
- En EN/FR, beaucoup de raccourcis de menu sont probablement non seulement invisibles, mais aussi inactifs.
- Plusieurs accelerateurs globaux et contextuels existent sans aucune visibilite UI.
- Un patch minimal, sur et sans changement de comportement peut commencer par `LocalizationManager.cpp` puis les menus contextuels, avant toute centralisation.
