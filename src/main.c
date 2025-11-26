#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int mapaObstaculos[MAXY][MAXX]; 

typedef struct {
    int x, y;
} Barco;

typedef struct TopScore {
    char iniciais[4];
    int pontos;
    struct TopScore *prox;
} TopScore;

TopScore* carregarRanking();
void salvarRanking(TopScore *lista);
void adicionarRanking(TopScore **lista, char nome[4], int pontos);
void mostrarRanking(TopScore *lista);
void liberarRanking(TopScore *lista);
void iniciarJogo(Barco *barco);
void desenharBarco(Barco *barco);
void limparBarco(Barco *barco);
void desenharObstaculos(int linhaY, int espacoX);
void limparLinha(int linhaY);
void verificarColisao(Barco *barco, int linhaY, int espacoX, int *jogoEncerrado);
void imprimirPontuacao(int pontuacao);
void telaInicial();
void telaFinal(int pontuacao, TopScore *ranking);

int main() {

    for (int y = 0; y < MAXY; y++)
        for (int x = 0; x < MAXX; x++)
            mapaObstaculos[y][x] = 0;

    Barco barco;
    int jogoEncerrado = 0;
    int pontuacao = 0;
    int tecla = 0;
    long temporizador = 0;
    int contadorAtualizacao = 0;
    int velocidade = 2;

    TopScore *ranking = carregarRanking();

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
                }

                for (int x = 0; x < MAXX; x++) {
                    if (x < espacoX || x > espacoX + 3)
                        mapaObstaculos[linhaY][x] = 1; 
                    else
                        mapaObstaculos[linhaY][x] = 0; 
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

    char nome[4];
    screenClear();
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MAXX/2 - 12, MAXY/2 - 1);
    printf("Digite suas iniciais (3 letras): ");
    screenUpdate();

    scanf("%3s", nome);

    adicionarRanking(&ranking, nome, pontuacao);
    salvarRanking(ranking);

    telaFinal(pontuacao, ranking);

    liberarRanking(ranking);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}


TopScore* carregarRanking() {
    FILE *f = fopen("ranking.txt", "r");
    if (!f) return NULL;

    TopScore *lista = NULL;
    while (!feof(f)) {
        TopScore *novo = malloc(sizeof(TopScore));
        if (fscanf(f, "%s %d", novo->iniciais, &novo->pontos) != 2) {
            free(novo);
            break;
        }
        novo->prox = lista;
        lista = novo;
    }
    fclose(f);
    return lista;
}

void salvarRanking(TopScore *lista) {
    FILE *f = fopen("ranking.txt", "w");
    if (!f) return;

    TopScore *atual = lista;
    while (atual) {
        fprintf(f, "%s %d\n", atual->iniciais, atual->pontos);
        atual = atual->prox;
    }
    fclose(f);
}

void adicionarRanking(TopScore **lista, char nome[4], int pontos) {
    TopScore *novo = malloc(sizeof(TopScore));
    strcpy(novo->iniciais, nome);
    novo->pontos = pontos;
    novo->prox = NULL;

    if (*lista == NULL || pontos > (*lista)->pontos) {
        novo->prox = *lista;
        *lista = novo;
        return;
    }

    TopScore *atual = *lista;
    while (atual->prox && atual->prox->pontos >= pontos)
        atual = atual->prox;

    novo->prox = atual->prox;
    atual->prox = novo;
}

void mostrarRanking(TopScore *lista) {
    screenClear();
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MAXX/2 - 5, 2);
    printf("🏆 RANKING");

    TopScore *atual = lista;
    int linha = 4;
    int pos = 1;

    while (atual && linha < MAXY-2) {
        screenGotoxy(MAXX/2 - 10, linha);
        printf("%dº - %s : %d pontos", pos, atual->iniciais, atual->pontos);
        atual = atual->prox;
        linha++;
        pos++;
    }

    screenGotoxy(MAXX/2 - 10, MAXY - 3);
    printf("Pressione ENTER para voltar...");
    screenUpdate();

    int tecla = 0;
    while (1) {
        if (keyhit()) {
            tecla = readch();
            if (tecla == 10) break;
        }
    }
}

void liberarRanking(TopScore *lista) {
    TopScore *tmp;
    while (lista) {
        tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
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
    if (barco->y == linhaY && (barco->x < espacoX || barco->x > espacoX + 3)) 
        *jogoEncerrado = 1;
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

void telaFinal(int pontuacao, TopScore *ranking) {
    int tecla = 0;

    while (1) {
        screenClear();
        screenSetColor(RED, BLACK);
        screenGotoxy(MAXX / 2 - 6, MAXY / 2 - 2);
        printf("💥  VOCE PERDEU! 💀");

        screenSetColor(WHITE, BLACK);
        screenGotoxy(MAXX / 2 - 10, MAXY / 2);
        printf("Pontuacao final: %d", pontuacao);

        screenSetColor(CYAN, BLACK);
        screenGotoxy(MAXX / 2 - 16, MAXY / 2 + 2);
        printf("R - Ver Ranking | ENTER - Sair");
        screenUpdate();

        if (keyhit()) {
            tecla = readch();
            if (tecla == 10) break;
            if (tecla == 'r' || tecla == 'R')
                mostrarRanking(ranking);
        }
    }
}
