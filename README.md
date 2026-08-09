# Juego de Nave con C++

Destruye y esquiva asteroides

## DESCRIPCIÓN DEL PROYECTO

Este proyecto es un juego de naves en C++. Evita chocar con los asteroides y destrúyelos para sumar puntos. La dificultad sube con el nivel.

prueba

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



### Compilar con Docker (sin instalar MinGW en el host)

Requiere Docker. El contenedor usa MinGW para generar `nave.exe`; el juego se ejecuta en Windows, no dentro del contenedor.

```powershell
docker build -t naves-cpp .
docker create --name naves-out naves-cpp /nave.exe
docker cp naves-out:/nave.exe .
docker rm naves-out
.\nave.exe
```

Con BuildKit, también puedes exportar el binario a una carpeta local:

```powershell
docker build --output type=local,dest=./out .
.\out\nave.exe
```



## REFERENCIAS

Esta aplicación se basó en el tutorial *Curso Juego de Naves en C++* del canal de YouTube *codigofacilito*: [https://www.youtube.com/playlist?list=PLpOqH6AE0tNj8W0EGpoGG2CEMDJTt4ihZ](https://www.youtube.com/playlist?list=PLpOqH6AE0tNj8W0EGpoGG2CEMDJTt4ihZ)