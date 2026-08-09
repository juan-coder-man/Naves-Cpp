# Juego de Nave con C++
Destruye y esquiva asteroides

## DESCRIPCIÓN DEL PROYECTO

Este proyecto es un juego de naves en C++. Evita chocar con los asteroides y destrúyelos para sumar puntos. La dificultad sube con el nivel.

![prueba](https://user-images.githubusercontent.com/49569043/113387035-1e9a6100-9351-11eb-8f40-05a49aa4ba0d.JPG)

## CONTROLES

- Flechas: mover la nave
- `a`: disparar
- `p`: pausar / reanudar
- Menú: 1 jugar, 2 cómo jugar, 3 salir

## EJECUTAR EL PROYECTO

Con **Visual Studio Community** el compilador es `cl` (MSVC), no `g++`. Desde una PowerShell normal de Cursor (o cualquier terminal):

```powershell
cmd /c "\"C:\Program Files\...\vcvars64.bat\" && cl /EHsc /Fe:nave.exe nave.cpp"
.\nave.exe
```

Opcional (solo si tienes MinGW instalado):

```powershell
g++ -o nave.exe nave.cpp
.\nave.exe
```

## REFERENCIAS

Esta aplicación se basó en el tutorial *Curso Juego de Naves en C++* del canal de YouTube *codigofacilito*: https://www.youtube.com/playlist?list=PLpOqH6AE0tNj8W0EGpoGG2CEMDJTt4ihZ
