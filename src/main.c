#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef struct {
    int x, y;
} Barco;

void iniciarJogo(Barco *barco);
void desenharBarco(Barco *barco);
void desenharObstaculos(int linhaY, int espacoX);
void limparLinha(int linhaY);
void verificarColisao(Barco *barco, int linhaY, int espacoX, int *jogoEncerrado);
void imprimirPontuacao(int pontuacao);

int main() {
    Barco barco;
    int jogoEncerrado = 0;
    int pontuacao = 0;
    int tecla = 0;
    long temporizador = 0;
    int contadorAtualizacao = 0;

    screenInit(1);
    keyboardInit();
    timerInit(100);
    srand(time(NULL));

    iniciarJogo(&barco);
    screenUpdate();

    int linhaY = 1;
    int espacoX = rand() % (MAXX - 8) + 4;

    while (!jogoEncerrado && tecla != 10) {
        if (keyhit()) {
            tecla = readch();
            if ((tecla == 'e' || tecla == 'E') && barco.x < MAXX - 2) barco.x++;
            if ((tecla == 'q' || tecla == 'Q') && barco.x > 1) barco.x--;
        }

        if (timerTimeOver() == 1) {
            contadorAtualizacao++;
            if (contadorAtualizacao % 2 == 0) {
                limparLinha(linhaY - 1);
                linhaY++;

                if (linhaY >= MAXY) {
                    linhaY = 1;
                    espacoX = rand() % (MAXX - 8) + 4;
                    pontuacao++;
                }

                desenharObstaculos(linhaY, espacoX);
                desenharBarco(&barco);
                verificarColisao(&barco, linhaY, espacoX, &jogoEncerrado);
                imprimirPontuacao(pontuacao);
                screenUpdate();
            }
            temporizador++;
        }
    }

    screenSetColor(RED, BLACK);
    screenGotoxy(MAXX / 2 - 5, MAXY / 2);
    printf("VOCÊ AFUNDOU!");

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

void iniciarJogo(Barco *barco) {
    barco->x = MAXX / 2;
    barco->y = MAXY - 2;
    screenClear();
}

void desenharBarco(Barco *barco) {
    screenSetColor(CYAN, BLACK);
    screenGotoxy(barco->x, barco->y);
    printf("⛵");
}

void desenharObstaculos(int linhaY, int espacoX) {
    screenSetColor(RED, BLACK);
    for (int x = 1; x <= MAXX; x++) {
        if (x < espacoX || x > espacoX + 3) {
            screenGotoxy(x, linhaY);
            printf("▓");
        }
    }
}

void limparLinha(int linhaY) {
    if (linhaY > 0 && linhaY < MAXY) {
        for (int x = 1; x <= MAXX; x++) {
            screenGotoxy(x, linhaY);
            printf(" ");
        }
    }
}

void verificarColisao(Barco *barco, int linhaY, int espacoX, int *jogoEncerrado) {
    if (barco->y == linhaY && (barco->x < espacoX || barco->x > espacoX + 3)) {
        *jogoEncerrado = 1;
    }
}

void imprimirPontuacao(int pontuacao) {
    screenSetColor(WHITE, BLACK);
    screenGotoxy(2, 2);
    printf("Obstáculos desviados: %d", pontuacao);
}
