#include <stdio.h>
#include <psl1ght/lv2.h>
#include <tiny3d.h>
#include <io/pad.h>
#include <math.h>

typedef struct { float x, y; int ativo; } Tiro;
typedef struct { float x, y; int vida; int ativo; } Inimigo;

int main() {
    tiny3d_Init(1024*1024);
    padInfo padinfo;
    padData paddata;
    ioPadInit(7);

    // Jogador - Caneca
    float px = 200, py = 400;
    int vida = 3;
    int fase = 1;
    int pontos = 0;
    int bossVida = 50;

    Tiro tiros[20];
    for(int i=0;i<20;i++) tiros[i].ativo = 0;

    Inimigo inimigos[5];
    for(int i=0;i<5;i++){ inimigos[i].x = 800 + i*150; inimigos[i].y = 100 + i*80; inimigos[i].vida = 2; inimigos[i].ativo = 1; }

    int tiroTimer = 0;

    while(1) {
        ioPadGetInfo(&padinfo);
        if(padinfo.status[0]) {
            ioPadGetData(0, &paddata);
            // ANALOGICO ESQUERDO - Move a caneca
            float ax = (paddata.ANA_L_H - 128) / 128.0f;
            float ay = (paddata.ANA_L_V - 128) / 128.0f;
            if(ax > 0.2 || ax < -0.2) px += ax * 6;
            if(ay > 0.2 || ay < -0.2) py += ay * 6;
            // ANALOGICO DIREITO - Mira / Dash
            float rx = (paddata.ANA_R_H - 128) / 128.0f;
            if(rx > 0.5) px += 12; // Dash direita
            if(rx < -0.5) px -= 12; // Dash esquerda

            // R1 / Quadrado = Atirar
            if((paddata.BTN_R1 || paddata.BTN_SQUARE) && tiroTimer <= 0) {
                for(int i=0;i<20;i++) if(!tiros[i].ativo){
                    tiros[i].x = px + 30; tiros[i].y = py + 10; tiros[i].ativo = 1; break;
                }
                tiroTimer = 10;
            }
            // X = Pular
            if(paddata.BTN_CROSS) py -= 8;
            // Bolinha = Descer
            if(paddata.BTN_CIRCLE) py += 8;
        }
        if(tiroTimer > 0) tiroTimer--;

        // Gravidade simples
        if(py < 400) py += 3;
        if(px < 20) px = 20; if(px > 1100) px = 1100;
        if(py < 20) py = 20; if(py > 500) py = 500;

        // Move tiros
        for(int i=0;i<20;i++) if(tiros[i].ativo){
            tiros[i].x += 10;
            if(tiros[i].x > 1280) tiros[i].ativo = 0;
            // Colisao com inimigo
            for(int j=0;j<5;j++) if(inimigos[j].ativo){
                if(tiros[i].x > inimigos[j].x && tiros[i].x < inimigos[j].x+50 &&
                   tiros[i].y > inimigos[j].y && tiros[i].y < inimigos[j].y+50){
                    tiros[i].ativo = 0; inimigos[j].vida--;
                    if(inimigos[j].vida <= 0){ inimigos[j].ativo = 0; pontos += 100; }
                }
            }
            // Colisao com Boss na Fase 3
            if(fase == 3 && tiros[i].x > 900 && tiros[i].y > 200 && tiros[i].y < 400){
                bossVida--; tiros[i].ativo = 0; pontos += 10;
                if(bossVida <= 0){ pontos += 1000; fase = 1; bossVida = 50; }
            }
        }

        // Move inimigos - Fase 1, 2, 3
        for(int j=0;j<5;j++) if(inimigos[j].ativo){
            inimigos[j].x -= (1 + fase);
            if(inimigos[j].x < -50){ inimigos[j].x = 1300; inimigos[j].y = 100 + (j*80); }
            // Se encostar, perde vida
            if(px > inimigos[j].x-30 && px < inimigos[j].x+50 && py > inimigos[j].y-30 && py < inimigos[j].y+50){
                vida--; inimigos[j].x = 1300; py = 400;
                if(vida <= 0){ vida = 3; pontos = 0; fase = 1; }
            }
        }

        // Troca de fase por pontos
        if(pontos >= 300 && fase == 1) fase = 2;
        if(pontos >= 800 && fase == 2) fase = 3;

        // DESENHA - Fundo muda por fase
        u32 corFundo = 0xFF87CEEB; // Fase 1 - Ceu
        if(fase == 2) corFundo = 0xFF2F004F; // Fase 2 - Roxo inferno
        if(fase == 3) corFundo = 0xFF8B0000; // Fase 3 - Vermelho Boss
        tiny3d_Clear(corFundo, TINY3D_CLEAR_ALL);
        tiny3d_Flip();
    }
    return 0;
}