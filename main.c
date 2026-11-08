#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Configurações do tamanho da tela do jogo
#define LARGURA 40
#define ALTURA 15
#define MAX_TIROS 10

// Estrutura do Personagem (Cuphead)
typedef struct {
    float x, y;
    float velocidadeY;
    int noChao;
    int vida;
    int direcao; // 1 = Direita, -1 = Esquerda
} Personagem;

// Estrutura dos Tiros
typedef struct {
    float x, y;
    int ativo;
    int direcao;
} Tiro;

// Inicializa as entidades
Personagem cuphead = {5, 11, 0, 1, 3, 1};
Tiro tiros[MAX_TIROS];

// Função para disparar um tiro do Cuphead
void atirar() {
    for (int i = 0; i < MAX_TIROS; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = cuphead.x + cuphead.direcao;
            tiros[i].y = cuphead.y;
            tiros[i].direcao = cuphead.direcao;
            tiros[i].ativo = 1;
            break;
        }
    }
}

// Atualiza a física do jogo (Engine)
void atualizar_jogo() {
    // ---- FÍSICA DO CUPHEAD ----
    const float GRAVIDADE = 0.6f;
    const int CHAO_Y = 11;

    // Aplica gravidade se não estiver no chão
    if (!cuphead.noChao) {
        cuphead.velocidadeY += GRAVIDADE;
        cuphead.y += cuphead.velocidadeY;
    }

    // Colisão com o chão
    if (cuphead.y >= CHAO_Y) {
        cuphead.y = CHAO_Y;
        cuphead.velocidadeY = 0;
        cuphead.noChao = 1;
    }

    // ---- FÍSICA DOS TIROS ----
    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            tiros[i].x += tiros[i].direcao * 1.5f; // Velocidade do tiro
            // Desativa o tiro se ele sair da tela
            if (tiros[i].x < 0 || tiros[i].x >= LARGURA) {
                tiros[i].ativo = 0;
            }
        }
    }
}

// Desenha a tela do jogo no terminal (Renderizador)
void renderizar_tela() {
    // Limpa a tela para o próximo frame
    printf("\033[H"); 

    printf("=== CUPHEAD MEIO VELHO DE PS3 (ENGINE VERDE) ===\n");
    printf(" Vida: [");
    for(int i=0; i<3; i++) printf(i < cuphead.vida ? "❤️" : "🖤");
    printf("]  |  Controles simulados ativos\n\n");

    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            // Desenha o Cuphead (Representado pelo 'C')
            if (x == (int)cuphead.x && y == (int)cuphead.y) {
                printf("C");
            } 
            // Desenha os Tiros (Representados por '*')
            else {
                int desenhouTiro = 0;
                for (int i = 0; i < MAX_TIROS; i++) {
                    if (tiros[i].ativo && x == (int)tiros[i].x && y == (int)tiros[i].y) {
                        printf("*");
                        desenhouTiro = 1;
                        break;
                    }
                }
                
                if (!desenhouTiro) {
                    // Desenha o Chão da fase
                    if (y == 12) {
                        printf("=");
                    } 
                    // Desenha o fundo/espaço vazio
                    else {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    printf("================================================\n");
}

int main() {
    // Simulação do loop de gameplay rodando no servidor do GitHub
    printf("Iniciando simulação de frames do Cuphead...\n");
    
    // Frame 1: Estado inicial
    renderizar_tela();
    usleep(100000);

    // Frame 2: Cuphead anda para a direita
    cuphead.x += 2;
    cuphead.direcao = 1;
    atualizar_jogo();
    renderizar_tela();
    usleep(100000);

    // Frame 3: Cuphead pula (Velocidade vertical negativa)
    cuphead.velocidadeY = -4.0f;
    cuphead.noChao = 0;
    atualizar_jogo();
    renderizar_tela();
    usleep(100000);

    // Frame 4: Cuphead atira no ar
    atirar();
    atualizar_jogo();
    renderizar_tela();
    usleep(100000);

    // Frame 5 a 15: O jogo roda simulando a gravidade puxando ele e o tiro andando
    for(int frame = 0; frame < 10; frame++) {
        atualizar_jogo();
        renderizar_tela();
        usleep(100000);
    }

    printf("\n[Sucesso] Todos os frames de movimentação, pulo e tiro renderizaram perfeitamente!\n");
    return 0;
}
