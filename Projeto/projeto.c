#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

typedef struct {
    float x, y, xInicial, yInicial, largura, altura;
    ALLEGRO_BITMAP* imagem;
    bool visivel;
} Pocao;

typedef struct { float x1, y1, x2, y2; } Retangulo;
typedef struct {
    float x1, y1, x2, y2;
    char texto[30];
} Botao;

typedef enum { MENU, JOGO, INSTRUCAO } Tela;

void desenharBotao(ALLEGRO_FONT* fonte, Botao b) {
    al_draw_filled_rectangle(b.x1, b.y1, b.x2, b.y2, al_map_rgb(65, 35, 80));
    al_draw_rectangle(b.x1, b.y1, b.x2, b.y2, al_map_rgb(180, 140, 200), 3);
    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        (b.x1 + b.x2) / 2, (b.y1 + b.y2) / 2 - 12,
        ALLEGRO_ALIGN_CENTER, b.texto);
}

bool clicouBotao(Botao b, float x, float y) {
    return x >= b.x1 && x <= b.x2 && y >= b.y1 && y <= b.y2;
}

void desenharFundo(ALLEGRO_BITMAP* img, int largura, int altura) {
    if (img) {
        al_draw_scaled_bitmap(img, 0, 0,
            al_get_bitmap_width(img), al_get_bitmap_height(img),
            0, 0, largura, altura, 0);
    }
    else {
        al_clear_to_color(al_map_rgb(20, 15, 25));
    }
}

void reiniciarPocoes(Pocao pocoes[], int total) {
    for (int i = 0; i < total; i++) {
        pocoes[i].x = pocoes[i].xInicial;
        pocoes[i].y = pocoes[i].yInicial;
        pocoes[i].visivel = true;
    }
}

void desenharGrimorio(ALLEGRO_FONT* fonte) {
    al_draw_filled_rectangle(0, 0, 1920, 1000, al_map_rgba(0, 0, 0, 180));
    al_draw_filled_rectangle(460, 150, 1460, 850, al_map_rgb(218, 190, 135));
    al_draw_rectangle(460, 150, 1460, 850, al_map_rgb(75, 40, 20), 8);
    al_draw_line(960, 165, 960, 835, al_map_rgb(110, 75, 40), 4);

    al_draw_filled_rectangle(1390, 160, 1430, 200, al_map_rgb(130, 35, 35));
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 1410, 168,
        ALLEGRO_ALIGN_CENTER, "X");

    al_draw_text(fonte, al_map_rgb(65, 35, 20), 710, 210,
        ALLEGRO_ALIGN_CENTER, "GRIMORIO DE QUIMICA");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 500, 280, 0, "TABELA PERIODICA");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 500, 330, 0, "H  - Hidrogenio");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 500, 370, 0, "O  - Oxigenio");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 500, 410, 0, "Na - Sodio");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 500, 450, 0, "Cl - Cloro");

    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1210, 210,
        ALLEGRO_ALIGN_CENTER, "ANOTACOES DA BRUXA");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1010, 300, 0, "A pocao correta esta");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1010, 340, 0, "relacionada ao elemento");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1010, 380, 0, "indicado no desafio.");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1010, 470, 0, "Observe as pistas antes");
    al_draw_text(fonte, al_map_rgb(65, 35, 20), 1010, 510, 0, "de escolher uma pocao.");
}

int main() {
    setlocale(LC_ALL, "");

    if (!al_init()) { printf("Erro ao inicializar Allegro.\\n"); return 1; }
    if (!al_install_mouse()) { printf("Erro ao inicializar o mouse.\\n"); return 1; }

    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();

    const int larguraTela = 1920, alturaTela = 1000;
    ALLEGRO_DISPLAY* display = al_create_display(larguraTela, alturaTela);
    if (!display) { printf("Erro ao criar a janela.\\n"); return 1; }

    ALLEGRO_BITMAP* fundoMenu = al_load_bitmap("menu.png");
    ALLEGRO_BITMAP* fundoJogo = al_load_bitmap("fundo.png");
    ALLEGRO_BITMAP* imagemPocoes = al_load_bitmap("pocoes.png");
    if (!imagemPocoes) printf("Aviso: nao foi possivel carregar pocoes.png.\\n");

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();

    if (!fonte || !timer || !fila) {
        printf("Erro ao criar fonte, timer ou fila de eventos.\\n");
        if (fundoMenu) al_destroy_bitmap(fundoMenu);
        if (fundoJogo) al_destroy_bitmap(fundoJogo);
        if (imagemPocoes) al_destroy_bitmap(imagemPocoes);
        if (fonte) al_destroy_font(fonte);
        if (timer) al_destroy_timer(timer);
        if (fila) al_destroy_event_queue(fila);
        al_destroy_display(display);
        return 1;
    }

    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));

    Botao jogar = { 760, 400, 1160, 480, "JOGAR" };
    Botao instrucao = { 760, 510, 1160, 590, "INSTRUCAO" };
    Botao sair = { 760, 620, 1160, 700, "SAIR" };
    Botao voltar = { 50, 850, 250, 920, "VOLTAR" };
    Botao botaoGrimorio = { 1650, 850, 1850, 950, "GRIMORIO" };

    Tela telaAtual = MENU;
    ALLEGRO_COLOR corVermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR corAzul = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR corCaldeirao = corVermelho;
    Retangulo ret = { 700, 800, 1200, 1000 };

    Pocao pocoes[8] = {
        { 1400,100,1400,100,80,100,NULL,true },
        { 1500,100,1500,100,80,100,NULL,true },
        { 1600,100,1600,100,80,100,NULL,true },
        { 1700,100,1700,100,80,100,NULL,true },
        { 1400,250,1400,250,80,100,NULL,true },
        { 1500,250,1500,250,80,100,NULL,true },
        { 1600,250,1600,250,80,100,NULL,true },
        { 1700,250,1700,250,80,100,NULL,true }
    };

    int totalPocoes = 8;
    int pocaoCorreta = 4; /* Indice 4 = quinta pocao. */
    int vidas = 3;
    bool grimorioAberto = false;
    bool arrastando = false;
    int pocaoSelecionada = -1;
    float offsetX = 0, offsetY = 0;
    bool estaRodando = true, redesenhar = true;

    al_start_timer(timer);

    while (estaRodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            estaRodando = false;
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN &&
            evento.mouse.button == 1) {
            float mouseX = evento.mouse.x;
            float mouseY = evento.mouse.y;

            if (telaAtual == MENU) {
                if (clicouBotao(jogar, mouseX, mouseY)) {
                    telaAtual = JOGO;
                    vidas = 3;
                    corCaldeirao = corVermelho;
                    grimorioAberto = false;
                    arrastando = false;
                    pocaoSelecionada = -1;
                    reiniciarPocoes(pocoes, totalPocoes);
                }
                else if (clicouBotao(instrucao, mouseX, mouseY)) {
                    telaAtual = INSTRUCAO;
                }
                else if (clicouBotao(sair, mouseX, mouseY)) {
                    estaRodando = false;
                }
            }
            else if (telaAtual == INSTRUCAO) {
                if (clicouBotao(voltar, mouseX, mouseY)) telaAtual = MENU;
            }
            else if (telaAtual == JOGO) {
                if (grimorioAberto) {
                    if ((mouseX >= 1390 && mouseX <= 1430 &&
                        mouseY >= 160 && mouseY <= 200) ||
                        mouseX < 460 || mouseX > 1460 ||
                        mouseY < 150 || mouseY > 850) {
                        grimorioAberto = false;
                    }
                }
                else if (clicouBotao(botaoGrimorio, mouseX, mouseY)) {
                    grimorioAberto = true;
                    arrastando = false;
                    pocaoSelecionada = -1;
                }
                else {
                    for (int i = totalPocoes - 1; i >= 0; i--) {
                        if (!pocoes[i].visivel) continue;
                        if (mouseX >= pocoes[i].x &&
                            mouseX <= pocoes[i].x + pocoes[i].largura &&
                            mouseY >= pocoes[i].y &&
                            mouseY <= pocoes[i].y + pocoes[i].altura) {
                            arrastando = true;
                            pocaoSelecionada = i;
                            offsetX = mouseX - pocoes[i].x;
                            offsetY = mouseY - pocoes[i].y;
                            break;
                        }
                    }
                }
            }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES &&
            telaAtual == JOGO && !grimorioAberto &&
            arrastando && pocaoSelecionada != -1) {
            pocoes[pocaoSelecionada].x = evento.mouse.x - offsetX;
            pocoes[pocaoSelecionada].y = evento.mouse.y - offsetY;
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP &&
            evento.mouse.button == 1) {
            if (telaAtual == JOGO && !grimorioAberto &&
                arrastando && pocaoSelecionada != -1) {
                float cx = pocoes[pocaoSelecionada].x +
                    pocoes[pocaoSelecionada].largura / 2;
                float cy = pocoes[pocaoSelecionada].y +
                    pocoes[pocaoSelecionada].altura / 2;

                if (cx >= ret.x1 && cx <= ret.x2 &&
                    cy >= ret.y1 && cy <= ret.y2) {
                    if (pocaoSelecionada == pocaoCorreta) {
                        pocoes[pocaoSelecionada].visivel = false;
                        corCaldeirao = corAzul;
                    }
                    else {
                        vidas--;
                        pocoes[pocaoSelecionada].x = pocoes[pocaoSelecionada].xInicial;
                        pocoes[pocaoSelecionada].y = pocoes[pocaoSelecionada].yInicial;

                        if (vidas <= 0) {
                            telaAtual = MENU;
                            grimorioAberto = false;
                            corCaldeirao = corVermelho;
                            reiniciarPocoes(pocoes, totalPocoes);
                        }
                    }
                }
                /* Soltou fora do caldeirao: a pocao permanece no local. */
            }
            arrastando = false;
            pocaoSelecionada = -1;
        }

        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;

            if (telaAtual == MENU) {
                desenharFundo(fundoMenu, larguraTela, alturaTela);
                al_draw_text(fonte, al_map_rgb(230, 200, 150),
                    larguraTela / 2, 220, ALLEGRO_ALIGN_CENTER,
                    "O LABORATORIO DA BRUXA");
                desenharBotao(fonte, jogar);
                desenharBotao(fonte, instrucao);
                desenharBotao(fonte, sair);
            }
            else if (telaAtual == INSTRUCAO) {
                desenharFundo(fundoMenu, larguraTela, alturaTela);
                al_draw_text(fonte, al_map_rgb(230, 200, 150),
                    larguraTela / 2, 200, ALLEGRO_ALIGN_CENTER, "INSTRUCAO");
                al_draw_text(fonte, al_map_rgb(255, 255, 255),
                    larguraTela / 2, 350, ALLEGRO_ALIGN_CENTER,
                    "Bem-vindo ao laboratorio da bruxa!");
                al_draw_text(fonte, al_map_rgb(255, 255, 255),
                    larguraTela / 2, 410, ALLEGRO_ALIGN_CENTER,
                    "Clique em JOGAR para iniciar a atividade.");
                desenharBotao(fonte, voltar);
            }
            else if (telaAtual == JOGO) {
                desenharFundo(fundoJogo, larguraTela, alturaTela);

                al_draw_filled_rectangle(ret.x1, ret.y1, ret.x2, ret.y2,
                    corCaldeirao);

                /* Quadrados provisórios das poções. */
                for (int i = 0; i < totalPocoes; i++) {
                    if (pocoes[i].visivel) {
                        al_draw_filled_rectangle(
                            pocoes[i].x, pocoes[i].y,
                            pocoes[i].x + pocoes[i].largura,
                            pocoes[i].y + pocoes[i].altura,
                            al_map_rgb(120, 80, 180));
                    }
                }

                al_draw_textf(fonte, al_map_rgb(255, 255, 255),
                    50, 920, 0, "Vidas: %d", vidas);

                desenharBotao(fonte, botaoGrimorio);
                if (grimorioAberto) desenharGrimorio(fonte);
            }

            al_flip_display();
        }
    }

    if (fundoMenu) al_destroy_bitmap(fundoMenu);
    if (fundoJogo) al_destroy_bitmap(fundoJogo);
    if (imagemPocoes) al_destroy_bitmap(imagemPocoes);
    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(display);

    return 0;
}
