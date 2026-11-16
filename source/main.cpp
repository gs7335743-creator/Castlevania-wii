#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

// Inclusao automatica das imagens da pasta "data"
#include "resource_05_0666e3d66131_jpg.h" // Logo Castlevania
#include "resource_06_c6944ff8027d_png.h" // Press Start (Branco)
#include "resource_07_d31bc0b8bc20_png.h" // Press Start (Vermelho)
#include "resource_18_1488dc77a410_png.h" // Castelo
#include "resource_19_e2949d0f3897_png.h" // Lua
#include "resource_21_90850a0a363b_jpg.h" // Fundo noite
#include "resource_22_f0411275ac18_png.h" // Arvore grande
#include "resource_32_3989a379246b_png.h" // Chao de pedra

int main(int argc, char **argv) {
    // Inicia video e controles do Wii
    GRRLIB_Init();
    WPAD_Init();

    // Carrega as imagens na memoria do Wii
    GRRLIB_texImg *tex_fundo   = GRRLIB_LoadTexture(resource_21_90850a0a363b_jpg);
    GRRLIB_texImg *tex_lua     = GRRLIB_LoadTexture(resource_19_e2949d0f3897_png);
    GRRLIB_texImg *tex_castelo = GRRLIB_LoadTexture(resource_18_1488dc77a410_png);
    GRRLIB_texImg *tex_logo    = GRRLIB_LoadTexture(resource_05_0666e3d66131_jpg);
    GRRLIB_texImg *tex_start1  = GRRLIB_LoadTexture(resource_06_c6944ff8027d_png);
    GRRLIB_texImg *tex_start2  = GRRLIB_LoadTexture(resource_07_d31bc0b8bc20_png);
    GRRLIB_texImg *tex_arvore  = GRRLIB_LoadTexture(resource_22_f0411275ac18_png);
    GRRLIB_texImg *tex_chao    = GRRLIB_LoadTexture(resource_32_3989a379246b_png);

    int estado = 0; // 0 = Tela Inicial, 1 = Jogando
    int timer = 0;

    // Variaveis do jogador na fase
    float playerX = 120.0f;
    float playerY = 330.0f;
    float velY = 0.0f;
    bool noChao = true;
    int direcao = 1; // 1 = direita, -1 = esquerda
    int atacando = 0;

    while (1) {
        WPAD_ScanPads();
        u32 down = WPAD_ButtonsDown(0);
        u32 held = WPAD_ButtonsHeld(0);

        if (down & WPAD_BUTTON_HOME) break;

        timer++;

        if (estado == 0) {
            // Desenha a Tela Inicial
            GRRLIB_DrawImg(0, 0, tex_fundo, 0, 1.0f, 1.05f, 0xFFFFFFFF);
            GRRLIB_DrawImg(420, 30, tex_lua, 0, 0.45f, 0.45f, 0xFFFFFFFF);
            GRRLIB_DrawImg(340, 120, tex_castelo, 0, 0.65f, 0.65f, 0xFFFFFFFF);
            GRRLIB_DrawImg(60, 40, tex_logo, 0, 0.8f, 0.8f, 0xFFFFFFFF);

            // Efeito piscando no botao Press Start
            if ((timer / 25) % 2 == 0) {
                GRRLIB_DrawImg(130, 360, tex_start1, 0, 1.0f, 1.0f, 0xFFFFFFFF);
            } else {
                GRRLIB_DrawImg(130, 360, tex_start2, 0, 1.0f, 1.0f, 0xFFFFFFFF);
            }

            // Aperte A ou + para iniciar o jogo
            if ((down & WPAD_BUTTON_A) || (down & WPAD_BUTTON_PLUS)) {
                estado = 1;
            }

        } else if (estado == 1) {
            // Controles (Wii Remote na horizontal ou vertical)
            if ((held & WPAD_BUTTON_RIGHT) || (held & WPAD_BUTTON_DOWN)) {
                playerX += 3.5f;
                direcao = 1;
            }
            if ((held & WPAD_BUTTON_LEFT) || (held & WPAD_BUTTON_UP)) {
                playerX -= 3.5f;
                direcao = -1;
            }

            // Pulo (Botao 2 ou A)
            if (((down & WPAD_BUTTON_2) || (down & WPAD_BUTTON_A)) && noChao) {
                velY = -10.5f;
                noChao = false;
            }

            // Ataque (Botao 1 ou B)
            if ((down & WPAD_BUTTON_1) || (down & WPAD_BUTTON_B)) {
                atacando = 12;
            }

            // Fisica da gravidade e chao
            playerY += velY;
            if (!noChao) velY += 0.55f;
            if (playerY >= 330.0f) {
                playerY = 330.0f;
                velY = 0.0f;
                noChao = true;
            }

            // Limites da tela
            if (playerX < 10) playerX = 10;
            if (playerX > 590) playerX = 590;

            // Desenha o cenario da fase
            GRRLIB_DrawImg(0, 0, tex_fundo, 0, 1.0f, 1.05f, 0xFFFFFFFF);
            GRRLIB_DrawImg(460, 25, tex_lua, 0, 0.35f, 0.35f, 0xFFFFFFFF);
            GRRLIB_DrawImg(40, 175, tex_arvore, 0, 0.9f, 0.9f, 0xFFFFFFFF);
            GRRLIB_DrawImg(430, 185, tex_arvore, 0, 0.85f, 0.85f, 0xFFFFFFFF);

            // Desenha o chao de pedra repetido
            for (int x = 0; x < 640; x += 80) {
                GRRLIB_DrawImg(x, 395, tex_chao, 0, 1.0f, 2.2f, 0xFFFFFFFF);
            }

            // Desenha o personagem Alucard (capa escura + brilho)
            GRRLIB_Rectangle(playerX, playerY, 28, 65, 0x1A1A2EFF, 1);
            GRRLIB_Rectangle(playerX + 4, playerY + 4, 20, 16, 0xF5D0A9FF, 1);
            GRRLIB_Rectangle(playerX + 2, playerY + 2, 24, 6, 0xE6E6FAFF, 1); // Cabelo claro

            // Desenha o golpe de espada se estiver atacando
            if (atacando > 0) {
                atacando--;
                if (direcao == 1) {
                    GRRLIB_Rectangle(playerX + 28, playerY + 25, 48, 6, 0x00E5FFFF, 1);
                } else {
                    GRRLIB_Rectangle(playerX - 48, playerY + 25, 48, 6, 0x00E5FFFF, 1);
                }
            }
        }

        GRRLIB_Render();
    }

    // Limpa memoria ao sair
    GRRLIB_FreeTexture(tex_fundo);
    GRRLIB_FreeTexture(tex_lua);
    GRRLIB_FreeTexture(tex_castelo);
    GRRLIB_FreeTexture(tex_logo);
    GRRLIB_FreeTexture(tex_start1);
    GRRLIB_FreeTexture(tex_start2);
    GRRLIB_FreeTexture(tex_arvore);
    GRRLIB_FreeTexture(tex_chao);
    GRRLIB_Exit();
    return 0;
}
