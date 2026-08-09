#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <list>

using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

#define LIM_HOR_MIN 2
#define LIM_HOR_MAX 88
#define LIM_VER_MIN 2
#define LIM_VER_MAX 28

#define NAVE_ANCHO 5
#define NAVE_ALTO 3

#define BHORIZONTAL 205
#define BVERTICAL 186
#define BESQ_SUP_IZQ 201
#define BESQ_SUP_DER 187
#define BESQ_INF_IZQ 200
#define BESQ_INF_DER 188

#define PUNTOS_POR_NIVEL 50
#define ASTEROIDES_BASE 6
#define ASTEROIDES_MAX 14
#define SLEEP_BASE 30
#define SLEEP_MIN 12

void posicionarCursor(int x, int y)
{
    HANDLE handleConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicionCursor;
    posicionCursor.X = (SHORT)x;
    posicionCursor.Y = (SHORT)y;
    SetConsoleCursorPosition(handleConsola, posicionCursor);
}

void ocultarCursor()
{
    HANDLE handleConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO informacionCursor;
    informacionCursor.dwSize = 100;
    informacionCursor.bVisible = FALSE;
    SetConsoleCursorInfo(handleConsola, &informacionCursor);
}

void pintar_limites()
{
    for (int i = LIM_HOR_MIN; i <= LIM_HOR_MAX; i++) {
        posicionarCursor(i, LIM_VER_MIN);
        printf("%c", BHORIZONTAL);
        posicionarCursor(i, LIM_VER_MAX);
        printf("%c", BHORIZONTAL);
    }
    for (int i = LIM_VER_MIN; i <= LIM_VER_MAX; i++) {
        posicionarCursor(LIM_HOR_MIN, i);
        printf("%c", BVERTICAL);
        posicionarCursor(LIM_HOR_MAX, i);
        printf("%c", BVERTICAL);
    }
    posicionarCursor(LIM_HOR_MIN, LIM_VER_MIN);
    printf("%c", BESQ_SUP_IZQ);
    posicionarCursor(LIM_HOR_MAX, LIM_VER_MIN);
    printf("%c", BESQ_SUP_DER);
    posicionarCursor(LIM_HOR_MIN, LIM_VER_MAX);
    printf("%c", BESQ_INF_IZQ);
    posicionarCursor(LIM_HOR_MAX, LIM_VER_MAX);
    printf("%c", BESQ_INF_DER);
}

template <typename T>
void limpiarLista(list<T*>& elementos)
{
    for (typename list<T*>::iterator it = elementos.begin(); it != elementos.end(); ++it) {
        delete *it;
    }
    elementos.clear();
}

class NAVE
{
    int x, y;
    int corazones;
    int vidas;

public:
    NAVE(int _x, int _y, int _corazones, int _vidas)
        : x(_x), y(_y), corazones(_corazones), vidas(_vidas) {}

    int X() { return x; }
    int Y() { return y; }
    int VIDAS() { return vidas; }
    int CORAZONES() { return corazones; }

    void choque() { corazones--; }
    void pintar();
    void borrar();
    void mover(int dx, int dy);
    void pintar_hud(int puntos, int nivel);
    void morir();
};

void NAVE::pintar()
{
    posicionarCursor(x, y);
    printf("  A  ");
    posicionarCursor(x, y + 1);
    printf(" <#> ");
    posicionarCursor(x, y + 2);
    printf("/ ^ \\");
}

void NAVE::borrar()
{
    posicionarCursor(x, y);
    printf("     ");
    posicionarCursor(x, y + 1);
    printf("     ");
    posicionarCursor(x, y + 2);
    printf("     ");
}

void NAVE::mover(int dx, int dy)
{
    int nuevaX = x + dx;
    int nuevaY = y + dy;

    if (nuevaX < LIM_HOR_MIN + 1) nuevaX = LIM_HOR_MIN + 1;
    if (nuevaX > LIM_HOR_MAX - NAVE_ANCHO) nuevaX = LIM_HOR_MAX - NAVE_ANCHO;
    if (nuevaY < LIM_VER_MIN + 1) nuevaY = LIM_VER_MIN + 1;
    if (nuevaY > LIM_VER_MAX - NAVE_ALTO) nuevaY = LIM_VER_MAX - NAVE_ALTO;

    if (nuevaX != x || nuevaY != y) {
        borrar();
        x = nuevaX;
        y = nuevaY;
        pintar();
    }
}

void NAVE::pintar_hud(int puntos, int nivel)
{
    posicionarCursor(LIM_HOR_MIN + 2, LIM_VER_MIN - 1);
    printf("Puntos %-5d  Nivel %-2d  Vidas %d  Salud ", puntos, nivel, vidas);
    for (int i = 0; i < 3; i++) {
        printf("%c", i < corazones ? 3 : ' ');
    }
    printf("   ");
}

void NAVE::morir()
{
    if (corazones != 0) return;

    borrar();
    posicionarCursor(x, y);
    printf("  *  ");
    posicionarCursor(x, y + 1);
    printf(" *** ");
    posicionarCursor(x, y + 2);
    printf("  *  ");
    Sleep(200);

    borrar();
    posicionarCursor(x, y);
    printf(" * * ");
    posicionarCursor(x, y + 1);
    printf("*   *");
    posicionarCursor(x, y + 2);
    printf(" * * ");
    Sleep(200);

    borrar();
    posicionarCursor(x, y);
    printf("*   *");
    posicionarCursor(x, y + 1);
    printf("     ");
    posicionarCursor(x, y + 2);
    printf("*   *");
    Sleep(200);

    borrar();
    vidas--;
    corazones = 3;
    pintar();
}

class ASTEROIDE
{
    int x, y;

    static int posX();
    static int posY();

public:
    ASTEROIDE();
    void pintar();
    void mover();
    void choque(NAVE& nave);
    int X() { return x; }
    int Y() { return y; }
};

ASTEROIDE::ASTEROIDE()
{
    x = posX();
    y = posY();
}

int ASTEROIDE::posX()
{
    return (LIM_HOR_MIN + 1) + rand() % (LIM_HOR_MAX - LIM_HOR_MIN - 2);
}

int ASTEROIDE::posY()
{
    return (LIM_VER_MIN + 1) + rand() % 7;
}

void ASTEROIDE::pintar()
{
    posicionarCursor(x, y);
    printf("%c", 184);
}

void ASTEROIDE::mover()
{
    posicionarCursor(x, y);
    printf(" ");
    y++;
    if (y > LIM_VER_MAX - 1) {
        x = posX();
        y = LIM_VER_MIN + 1;
    }
    pintar();
}

void ASTEROIDE::choque(NAVE& nave)
{
    if (x >= nave.X() && x < nave.X() + NAVE_ANCHO &&
        y >= nave.Y() && y < nave.Y() + NAVE_ALTO) {
        nave.choque();
        nave.pintar();
        x = posX();
        y = LIM_VER_MIN + 1;
    }
}

class BALA
{
    int x;
    int y;

public:
    BALA(int _x, int _y) : x(_x), y(_y) {}
    void mover();
    bool fuera();
    int X() { return x; }
    int Y() { return y; }
};

void BALA::mover()
{
    posicionarCursor(x, y);
    printf(" ");
    y--;
    posicionarCursor(x, y);
    printf("%c", 248);
}

bool BALA::fuera()
{
    return y <= LIM_VER_MIN + 1;
}

void mostrarComoJugar()
{
    system("cls");
    printf("COMO JUGAR\n\n");
    printf("- Flechas: mover la nave\n");
    printf("- Tecla 'a': disparar\n");
    printf("- Tecla 'p': pausar / reanudar\n");
    printf("- Evita chocar con los asteroides\n");
    printf("- Destruye asteroides para sumar puntos\n");
    printf("- Cada %d puntos sube el nivel (mas dificultad)\n\n", PUNTOS_POR_NIVEL);
    system("pause");
}

int mostrarMenu()
{
    while (true) {
        system("cls");
        printf("=== JUEGO DE NAVES ===\n\n");
        printf("1. Jugar\n");
        printf("2. Como jugar\n");
        printf("3. Salir\n\n");
        printf("Elige una opcion: ");

        char opcion = getch();
        printf("%c\n", opcion);

        if (opcion == '1') return 1;
        if (opcion == '2') {
            mostrarComoJugar();
        } else if (opcion == '3') {
            return 3;
        }
    }
}

bool jugarPartida()
{
    system("cls");
    ocultarCursor();
    pintar_limites();

    NAVE nave(LIM_HOR_MAX / 3, LIM_VER_MAX - NAVE_ALTO, 3, 3);
    nave.pintar();

    list<ASTEROIDE*> asteroides;
    list<BALA*> balas;

    for (int i = 0; i < ASTEROIDES_BASE; i++) {
        asteroides.push_back(new ASTEROIDE());
    }

    bool gameOver = false;
    bool pausado = false;
    int puntos = 0;
    int nivel = 1;
    int asteroidesObjetivo = ASTEROIDES_BASE;

    nave.pintar_hud(puntos, nivel);

    while (!gameOver) {
        while (kbhit()) {
            int tecla = getch();
            if (tecla == 0 || tecla == 224) {
                if (!kbhit()) continue;
                tecla = getch();
                if (!pausado) {
                    if (tecla == IZQUIERDA) nave.mover(-1, 0);
                    else if (tecla == DERECHA) nave.mover(1, 0);
                    else if (tecla == ARRIBA) nave.mover(0, -1);
                    else if (tecla == ABAJO) nave.mover(0, 1);
                }
            } else if (tecla == 'p' || tecla == 'P') {
                pausado = !pausado;
                posicionarCursor(LIM_HOR_MIN + 35, LIM_VER_MIN + 12);
                if (pausado) {
                    printf(" PAUSA ");
                } else {
                    printf("       ");
                    nave.pintar();
                }
            } else if (!pausado && (tecla == 'a' || tecla == 'A')) {
                balas.push_back(new BALA(nave.X() + 2, nave.Y() - 1));
            }
        }

        if (pausado) {
            Sleep(30);
            continue;
        }

        for (list<BALA*>::iterator bala = balas.begin(); bala != balas.end();) {
            (*bala)->mover();
            if ((*bala)->fuera()) {
                posicionarCursor((*bala)->X(), (*bala)->Y());
                printf(" ");
                delete *bala;
                bala = balas.erase(bala);
            } else {
                ++bala;
            }
        }

        for (list<ASTEROIDE*>::iterator asteroide = asteroides.begin();
             asteroide != asteroides.end();
             ++asteroide) {
            (*asteroide)->mover();
            (*asteroide)->choque(nave);
        }

        for (list<ASTEROIDE*>::iterator asteroide = asteroides.begin();
             asteroide != asteroides.end();) {
            bool destruido = false;
            for (list<BALA*>::iterator bala = balas.begin(); bala != balas.end();) {
                if ((*asteroide)->X() == (*bala)->X() &&
                    ((*asteroide)->Y() + 1 == (*bala)->Y() ||
                     (*asteroide)->Y() == (*bala)->Y())) {
                    posicionarCursor((*bala)->X(), (*bala)->Y());
                    printf(" ");
                    delete *bala;
                    bala = balas.erase(bala);

                    posicionarCursor((*asteroide)->X(), (*asteroide)->Y());
                    printf(" ");
                    delete *asteroide;
                    asteroide = asteroides.erase(asteroide);

                    asteroides.push_back(new ASTEROIDE());
                    puntos += 5;
                    destruido = true;
                    break;
                } else {
                    ++bala;
                }
            }
            if (!destruido) {
                ++asteroide;
            }
        }

        int nuevoNivel = 1 + (puntos / PUNTOS_POR_NIVEL);
        if (nuevoNivel > nivel) {
            nivel = nuevoNivel;
            asteroidesObjetivo = ASTEROIDES_BASE + (nivel - 1);
            if (asteroidesObjetivo > ASTEROIDES_MAX) {
                asteroidesObjetivo = ASTEROIDES_MAX;
            }
        }

        while ((int)asteroides.size() < asteroidesObjetivo) {
            asteroides.push_back(new ASTEROIDE());
        }

        nave.morir();
        nave.pintar_hud(puntos, nivel);

        if (nave.VIDAS() == 0) {
            gameOver = true;
        }

        int delay = SLEEP_BASE - (nivel - 1) * 3;
        if (delay < SLEEP_MIN) delay = SLEEP_MIN;
        Sleep(delay);
    }

    limpiarLista(balas);
    limpiarLista(asteroides);

    system("cls");
    printf("GAME OVER\n");
    printf("Tu puntaje fue: %d\n", puntos);
    printf("Nivel alcanzado: %d\n\n", nivel);
    printf("1. Volver al menu\n");
    printf("2. Salir\n\n");
    printf("Elige una opcion: ");

    while (true) {
        char opcion = getch();
        if (opcion == '1') return true;
        if (opcion == '2') return false;
    }
}

int main()
{
    srand((unsigned)time(NULL));
    ocultarCursor();

    bool salir = false;
    while (!salir) {
        int opcion = mostrarMenu();
        if (opcion == 3) {
            salir = true;
        } else if (opcion == 1) {
            bool volverAlMenu = jugarPartida();
            if (!volverAlMenu) {
                salir = true;
            }
        }
    }

    system("cls");
    printf("Gracias por jugar\n");
    system("pause");
    return 0;
}
