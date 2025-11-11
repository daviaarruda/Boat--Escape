#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef struct {
    int x, y;
} Barco;

void iniciarJogo(Barco *barco);
void desenharBarco(Barco *barco);
void limparBarco(Barco *barco);
void desenharObstaculos(int linhaY, int espacoX);
void limparLinha(int linhaY);
void verificarColisao(Barco *barco, int linhaY, int espacoX, int *jogoEncerrado);
void imprimirPontuacao(int pontuacao);
void telaInicial();
void telaFinal(int pontuacao);

int main() {
    Barco barco;
    int jogoEncerrado = 0;
    int pontuacao = 0;
    int tecla = 0;
    long temporizador = 0;
    int contadorAtualizacao = 0;
    int velocidade = 2;

    screenInit(1);
    keyboardInit();
    timerInit(100);
    srand(time(NULL));

    telaInicial();

    while (1) {
        if (keyhit()) {
            tecla = readch();
            if (tecla == 10) break;
        }
    }

    while (keyhit()) readch();
    tecla = 0;

    iniciarJogo(&barco);
    screenUpdate();

    int linhaY = 1;
    int espacoX = rand() % (MAXX - 8) + 4;
    int ultimoEspaco = espacoX;

    while (!jogoEncerrado && tecla != 10) {
        if (keyhit()) {
            tecla = readch();
            limparBarco(&barco);
            if ((tecla == 'e' || tecla == 'E') && barco.x < MAXX - 2) barco.x++;
            if ((tecla == 'q' || tecla == 'Q') && barco.x > 1) barco.x--;
            desenharBarco(&barco);
            screenUpdate();
        }

        if (timerTimeOver() == 1) {
            contadorAtualizacao++;
            if (contadorAtualizacao % velocidade == 0) {
                limparLinha(linhaY - 1);
                linhaY++;
                if (linhaY >= MAXY) {
                    linhaY = 1;
                    do {
                        espacoX = rand() % (MAXX - 8) + 4;
                    } while (espacoX == ultimoEspaco);
                    ultimoEspaco = espacoX;
                    pontuacao++;
                    if (pontuacao % 10 == 0 && velocidade > 1)
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

    telaFinal(pontuacao);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

void iniciarJogo(Barco *barco) {
    barco->x = MAXX / 2;
    barco->y = MAXY - 2;
    screenClear();
    desenharBarco(barco);
}

void desenharBarco(Barco *barco) {
    screenSetColor(CYAN, BLACK);
    screenGotoxy(barco->x, barco->y);
    printf("⛵");
}

void limparBarco(Barco *barco) {
    screenGotoxy(barco->x, barco->y);
    printf("  ");
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
    if (barco->y == linhaY && (barco->x < espacoX || barco->x > espacoX + 3)) *jogoEncerrado = 1;
}

void imprimirPontuacao(int pontuacao) {
    screenSetColor(WHITE, BLACK);
    screenGotoxy(2, 2);
    printf("🌊 Obstáculos desviados: %d", pontuacao);
}

void telaInicial() {
    screenClear();
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MAXX / 2 - 8, MAXY / 2 - 2);
    printf("🚤  B O A T   E S C A P E  🌊");
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MAXX / 2 - 13, MAXY / 2);
    printf("Pressione ENTER para comecar o jogo!");
    screenSetColor(BLUE, BLACK);
    screenGotoxy(MAXX / 2 - 16, MAXY / 2 + 3);
    printf("Use Q e E para mover o barco e desviar dos obstaculos!");
    screenUpdate();
}

void telaFinal(int pontuacao) {
    screenClear();
    screenSetColor(RED, BLACK);
    screenGotoxy(MAXX / 2 - 6, MAXY / 2 - 2);
    printf("💥  V O C Ê   P E R D E U ! 💀");
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MAXX / 2 - 10, MAXY / 2);
    printf("Pontuacao final: %d", pontuacao);
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MAXX / 2 - 14, MAXY / 2 + 2);
    printf("Pressione ENTER para sair...");
    screenUpdate();
    int tecla = 0;
    while (1) {
        if (keyhit()) {
            tecla = readch();
            if (tecla == 10) break;
        }
    }
}
