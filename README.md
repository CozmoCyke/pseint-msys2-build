# PSeInt – Build MSYS2 (Windows)

Compilation complète de PSeInt avec :
- MSYS2 (MINGW64)
- wxWidgets 3.2
- GCC (g++)

## ✅ Fonctionnel

- pseint.exe
- wxPSeInt.exe
- psdraw3.exe
- psexport.exe
- pseval.exe

## 🔧 Corrections apportées

- Ajout de `-lwsock32` pour zockets
- Correction include wx.rc (windres)
- Conversion wxString → char*
- Adaptation wxWidgets 3.2

## ▶️ Compilation

```bash
mingw32-make ARCH=w32

## 📁 Sortie

##⚠️ Notes

Nécessite MSYS2 + mingw64

wxWidgets installé via pacman
