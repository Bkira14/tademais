#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
// Estrutura das Pocoes
typedef struct {
    float x, y, xInicial, yInicial, largura, altura;
    ALLEGRO_BITMAP* imagem;
    bool visivel;
} Pocao;
typedef struct {
    float x1, y1, x2, y2;
} Retangulo;
typedef struct {
    float x1, y1, x2, y2;
    char texto[30];
} Botao;
typedef enum {
    MENU, JOGO, INSTRUCAO
} Tela;
// Desenhar botoes
void desenharBotao(ALLEGRO_FONT* fonte, Botao b) {
    al_draw_filled_rectangle(b.x1, b.y1, b.x2, b.y2,
        al_map_rgb(65, 35, 80));
    al_draw_rectangle(b.x1, b.y1, b.x2, b.y2,
        al_map_rgb(180, 140, 200), 3);
    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        (b.x1 + b.x2) / 2, (b.y1 + b.y2) / 2 - 12,
        ALLEGRO_ALIGN_CENTER, b.texto);
}
// Verificar clique no botao
bool clicouBotao(Botao b, float x, float y) {
    return x >= b.x1 && x <= b.x2 &&
        y >= b.y1 && y <= b.y2;
}
// Desenhar imagem de fundo
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
// Reiniciar posicao das pocoes
void reiniciarPocoes(Pocao pocoes[], int total) {
    for (int i = 0; i < total; i++) {
        pocoes[i].x = pocoes[i].xInicial;
        pocoes[i].y = pocoes[i].yInicial;
        pocoes[i].visivel = true;
    }
}
// Desenhar Grimorio
void desenharGrimorio(ALLEGRO_FONT* fonte, int pagina) {
    ALLEGRO_COLOR corTexto = al_map_rgb(65, 35, 20);
    ALLEGRO_COLOR corLinha = al_map_rgb(145, 112, 75);
    ALLEGRO_COLOR corTitulo = al_map_rgb(80, 42, 25);
    // Fundo escuro atras do grimorio
    al_draw_filled_rectangle(0, 0, 1920, 1000,
        al_map_rgba(0, 0, 0, 180));
    // Paginas do grimorio
    al_draw_filled_rectangle(460, 150, 1460, 850,
        al_map_rgb(218, 190, 135));
    al_draw_rectangle(460, 150, 1460, 850,
        al_map_rgb(75, 40, 20), 8);
    // Divisao entre as paginas
    al_draw_line(960, 165, 960, 835,
        al_map_rgb(110, 75, 40), 4);
    // Botao fechar
    al_draw_filled_rectangle(1390, 160, 1430, 200,
        al_map_rgb(130, 35, 35));
    al_draw_text(fonte, al_map_rgb(255, 255, 255),
        1410, 168, ALLEGRO_ALIGN_CENTER, "X");
    if (pagina == 1) {
        // Titulos das paginas
        al_draw_text(fonte, corTitulo,
            710, 205, ALLEGRO_ALIGN_CENTER,
            "GRIMORIO DE QUIMICA");
        al_draw_text(fonte, corTitulo,
            1210, 205, ALLEGRO_ALIGN_CENTER,
            "ANOTACOES DA BRUXA");
        // PAGINA ESQUERDA - TABELA COMPARATIVA
        al_draw_text(fonte, corTexto,
            485, 245, 0, "TABELA COMPARATIVA DOS COMPOSTOS");
        al_draw_text(fonte, corTexto, 485, 275, 0, "Composto");
        al_draw_text(fonte, corTexto, 575, 275, 0, "pH");
        al_draw_text(fonte, corTexto, 635, 275, 0, "OH-?");
        al_draw_text(fonte, corTexto, 730, 275, 0, "Conduz?");
        al_draw_line(480, 292, 940, 292, corLinha, 1);
        // HCl
        al_draw_text(fonte, corTexto, 485, 305, 0, "HCl");
        al_draw_text(fonte, corTexto, 575, 305, 0, "0 a 2");
        al_draw_text(fonte, corTexto, 635, 305, 0, "Nao");
        al_draw_text(fonte, corTexto, 730, 305, 0, "Sim, em agua");
        al_draw_line(480, 325, 940, 325, corLinha, 1);
        // NaOH (Correta)
        al_draw_text(fonte, corTexto, 485, 338, 0, "NaOH");
        al_draw_text(fonte, corTexto, 575, 338, 0, "13 a 14");
        al_draw_text(fonte, corTexto, 635, 338, 0, "Sim");
        al_draw_text(fonte, corTexto, 730, 338, 0, "Sim, em agua");
        al_draw_line(480, 358, 940, 358, corLinha, 1);
        // NaCl
        al_draw_text(fonte, corTexto, 485, 371, 0, "NaCl");
        al_draw_text(fonte, corTexto, 575, 371, 0, "7");
        al_draw_text(fonte, corTexto, 635, 371, 0, "Nao");
        al_draw_text(fonte, corTexto, 730, 371, 0, "Dissolvido/fundido");
        al_draw_line(480, 391, 940, 391, corLinha, 1);
        // CO2
        al_draw_text(fonte, corTexto, 485, 404, 0, "CO2");
        al_draw_text(fonte, corTexto, 575, 404, 0, "5,6*");
        al_draw_text(fonte, corTexto, 635, 404, 0, "Nao");
        al_draw_text(fonte, corTexto, 730, 404, 0, "Nao, puro");
        al_draw_line(480, 424, 940, 424, corLinha, 1);
        // H2SO4
        al_draw_text(fonte, corTexto, 485, 437, 0, "H2SO4");
        al_draw_text(fonte, corTexto, 575, 437, 0, "0 a 1");
        al_draw_text(fonte, corTexto, 635, 437, 0, "Nao");
        al_draw_text(fonte, corTexto, 730, 437, 0, "Sim, em agua");
        al_draw_line(480, 457, 940, 457, corLinha, 1);
        // PAGINA DIREITA - ANOTACOES DA BRUXA
        al_draw_text(fonte, corTitulo,
            1210, 245, ALLEGRO_ALIGN_CENTER,
            "CARACTERISTICAS DAS BASES");
        al_draw_text(fonte, corTexto, 990, 290, 0, "1. PRESENCA DE HIDROXILA");
        al_draw_text(fonte, corTexto, 990, 320, 0, "A principal marca de uma base e a");
        al_draw_text(fonte, corTexto, 990, 342, 0, "presenca do anion hidroxila (OH-)");
        al_draw_text(fonte, corTexto, 990, 430, 0, "2. PH");
        al_draw_text(fonte, corTexto, 990, 460, 0, "Apresentam pH maior que 7,");
        al_draw_text(fonte, corTexto, 990, 482, 0, "geralmente entre 7 e 14.");
        al_draw_text(fonte, corTexto, 990, 526, 0, "3. CONDUTIVIDADE");
        al_draw_text(fonte, corTexto, 990, 556, 0, "Conduzem corrente eletrica quando");
        al_draw_text(fonte, corTexto, 990, 578, 0, "dissolvidas em agua (eletrolitos).");
    }
    // Botoes para navegar entre as paginas
    if (pagina == 1) {
        al_draw_filled_rectangle(1370, 790, 1430, 830, al_map_rgb(75, 40, 20));
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 1400, 800, ALLEGRO_ALIGN_CENTER, ">");
    }
    else {
        al_draw_filled_rectangle(490, 790, 550, 830, al_map_rgb(75, 40, 20));
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 520, 800, ALLEGRO_ALIGN_CENTER, "<");
    }
}
int main() {
    setlocale(LC_ALL, "");
    if (!al_init()) {
        printf("Erro ao inicializar Allegro.\n");
        return 1;
    }
    if (!al_install_mouse()) {
        printf("Erro ao inicializar o mouse.\n");
        return 1;
    }
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    const int larguraTela = 1920, alturaTela = 1000;
    ALLEGRO_DISPLAY* display = al_create_display(larguraTela, alturaTela);
    if (!display) {
        printf("Erro ao criar a janela.\n");
        return 1;
    }
    // Carregar imagens
    ALLEGRO_BITMAP* fundoMenu = al_load_bitmap("menu.png");
    ALLEGRO_BITMAP* fundoJogo = al_load_bitmap("fundo.png");
    ALLEGRO_BITMAP* imgCaldeiraoVermelho = al_load_bitmap("caldeirao_vermelho.png");
    ALLEGRO_BITMAP* imgCaldeiraoAzul = al_load_bitmap("caldeirao_azul.png");
    ALLEGRO_BITMAP* imagemCoracao = al_load_bitmap("coracao.png");
    ALLEGRO_BITMAP* imagemAni = al_load_bitmap("ani.png");
    // Imagens individuais das 5 poções
    ALLEGRO_BITMAP* imgHCl = al_load_bitmap("pocao_HCl.png");
    ALLEGRO_BITMAP* imgNaOH = al_load_bitmap("pocao_NaOH.png");
    ALLEGRO_BITMAP* imgNaCl = al_load_bitmap("pocao_NaCl.png");
    ALLEGRO_BITMAP* imgCO2 = al_load_bitmap("pocao_CO2.png");
    ALLEGRO_BITMAP* imgH2SO4 = al_load_bitmap("pocao_H2SO4.png");
    // Avisar no console se algum arquivo de imagem nao foi encontrado.
    if (!fundoMenu) printf("Nao foi possivel carregar menu.png\n");
    if (!fundoJogo) printf("Nao foi possivel carregar fundo.png\n");
    if (!imgCaldeiraoVermelho) printf("Nao foi possivel carregar caldeirao_vermelho.png\n");
    if (!imgCaldeiraoAzul) printf("Nao foi possivel carregar caldeirao_azul.png\n");
    if (!imagemCoracao) printf("Nao foi possivel carregar coracao.png\n");
    if (!imagemAni) printf("Nao foi possivel carregar ani.png\n");
    if (!imgHCl) printf("Nao foi possivel carregar pocao_HCl.png\n");
    if (!imgNaOH) printf("Nao foi possivel carregar pocao_NaOH.png\n");
    if (!imgNaCl) printf("Nao foi possivel carregar pocao_NaCl.png\n");
    if (!imgCO2) printf("Nao foi possivel carregar pocao_CO2.png\n");
    if (!imgH2SO4) printf("Nao foi possivel carregar pocao_H2SO4.png\n");
    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));
    // Botoes do Menu
    Botao jogar = { 760, 400, 1160, 480, "JOGAR" };
    Botao instrucao = { 760, 510, 1160, 590, "INSTRUCOES" };
    Botao sair = { 760, 620, 1160, 700, "SAIR" };
    Botao botaoGrimorio = { 1650, 850, 1850, 950, "GRIMORIO" };
    Tela telaAtual = MENU;
    // Caldeirão ainda maior e mais alto (X: 450 a 1450, Y: 450 a 980)
    Retangulo ret = { 450, 450, 1450, 980 };
    bool caldeiraoAtivo = false;
    // Configuração das 5 poções com tamanho aumentado (largura: 120, altura: 150)
    // E reposicionadas para o lado direito superior de forma organizada
    Pocao pocoes[5] = {
        { 1350, 80,  1350, 80,  120, 150, imgHCl, true },
        { 1500, 80,  1500, 80,  120, 150, imgNaOH, true },
        { 1650, 80,  1650, 80,  120, 150, imgNaCl, true },
        { 1420, 260, 1420, 260, 120, 150, imgCO2, true },
        { 1580, 260, 1580, 260, 120, 150, imgH2SO4, true }
    };
    int totalPocoes = 5;
    int pocaoCorreta = 1;
    int vidas = 3;
    bool grimorioAberto = false;
    int paginaGrimorio = 1;
    bool arrastando = false;
    int pocaoSelecionada = -1;
    float offsetX = 0, offsetY = 0;
    // Controle do tempo de fala da Ani (15 segundos)
    double tempoInicioFala = 0;
    bool aniFalando = true;
    bool estaRodando = true;
    bool redesenhar = true;
    al_start_timer(timer);
    while (estaRodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            estaRodando = false;
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
            if (telaAtual == JOGO && aniFalando) {
                if (al_get_time() - tempoInicioFala >= 15.0) {
                    aniFalando = false;
                }
            }
        }
        // CLIQUES DO MOUSE
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            float mouseX = evento.mouse.x;
            float mouseY = evento.mouse.y;
            if (telaAtual == MENU) {
                if (clicouBotao(jogar, mouseX, mouseY)) {
                    telaAtual = JOGO;
                    vidas = 3;
                    caldeiraoAtivo = false;
                    grimorioAberto = false;
                    arrastando = false;
                    pocaoSelecionada = -1;
                    reiniciarPocoes(pocoes, totalPocoes);
                    tempoInicioFala = al_get_time();
                    aniFalando = true;
                }
                else if (clicouBotao(instrucao, mouseX, mouseY)) {
                    telaAtual = INSTRUCAO;
                }
                else if (clicouBotao(sair, mouseX, mouseY)) {
                    estaRodando = false;
                }
            }
            else if (telaAtual == INSTRUCAO) {
                if (mouseX >= 50 && mouseX <= 250 && mouseY >= 850 && mouseY <= 920) {
                    telaAtual = MENU;
                }
            }
            else if (telaAtual == JOGO) {
                if (grimorioAberto) {
                    if (mouseX >= 1390 && mouseX <= 1430 && mouseY >= 160 && mouseY <= 200) {
                        grimorioAberto = false;
                        paginaGrimorio = 1;
                    }
                    else if (paginaGrimorio == 1 && mouseX >= 1370 && mouseX <= 1430 && mouseY >= 790 && mouseY <= 830) {
                        paginaGrimorio = 2;
                    }
                    else if (paginaGrimorio == 2 && mouseX >= 490 && mouseX <= 550 && mouseY >= 790 && mouseY <= 830) {
                        paginaGrimorio = 1;
                    }
                    else if (mouseX < 460 || mouseX > 1460 || mouseY < 150 || mouseY > 850) {
                        grimorioAberto = false;
                        paginaGrimorio = 1;
                    }
                }
                else if (clicouBotao(botaoGrimorio, mouseX, mouseY)) {
                    grimorioAberto = true;
                    paginaGrimorio = 1;
                    arrastando = false;
                    pocaoSelecionada = -1;
                }
                else {
                    for (int i = totalPocoes - 1; i >= 0; i--) {
                        if (!pocoes[i].visivel) continue;
                        if (mouseX >= pocoes[i].x && mouseX <= pocoes[i].x + pocoes[i].largura &&
                            mouseY >= pocoes[i].y && mouseY <= pocoes[i].y + pocoes[i].altura) {
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
        // MOVIMENTACAO DAS POCOES
        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES && telaAtual == JOGO && !grimorioAberto && arrastando && pocaoSelecionada != -1) {
            pocoes[pocaoSelecionada].x = evento.mouse.x - offsetX;
            pocoes[pocaoSelecionada].y = evento.mouse.y - offsetY;
        }
        // SOLTAR POCAO NO CALDEIRAO
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && evento.mouse.button == 1) {
            if (telaAtual == JOGO && !grimorioAberto && arrastando && pocaoSelecionada != -1) {
                float cx = pocoes[pocaoSelecionada].x + pocoes[pocaoSelecionada].largura / 2;
                float cy = pocoes[pocaoSelecionada].y + pocoes[pocaoSelecionada].altura / 2;
                if (cx >= ret.x1 && cx <= ret.x2 && cy >= ret.y1 && cy <= ret.y2) {
                    if (pocaoSelecionada == pocaoCorreta) {
                        pocoes[pocaoSelecionada].visivel = false;
                        caldeiraoAtivo = true;
                    }
                    else {
                        vidas--;
                        pocoes[pocaoSelecionada].x = pocoes[pocaoSelecionada].xInicial;
                        pocoes[pocaoSelecionada].y = pocoes[pocaoSelecionada].yInicial;
                        if (vidas <= 0) {
                            telaAtual = MENU;
                            grimorioAberto = false;
                            caldeiraoAtivo = false;
                            reiniciarPocoes(pocoes, totalPocoes);
                        }
                    }
                }
            }
            arrastando = false;
            pocaoSelecionada = -1;
        }
        // DESENHAR TELAS
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;
            if (telaAtual == MENU) {
                desenharFundo(fundoMenu, larguraTela, alturaTela);
                desenharBotao(fonte, jogar);
                desenharBotao(fonte, instrucao);
                desenharBotao(fonte, sair);
            }
            else if (telaAtual == INSTRUCAO) {
                desenharFundo(fundoMenu, larguraTela, alturaTela);
                al_draw_filled_rectangle(400, 180, 1520, 800, al_map_rgba(20, 10, 30, 220));
                al_draw_rectangle(400, 180, 1520, 800, al_map_rgb(180, 140, 200), 4);
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 960, 230, ALLEGRO_ALIGN_CENTER, "INSTRUCOES DO JOGO");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 470, 320, 0, "1. Leia as anotacoes no Grimorio para descobrir a pocao correta.");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 470, 380, 0, "2. Clique em uma pocao e arraste ate o caldeirao.");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 470, 440, 0, "3. Soltar a pocao errada faz perder uma vida.");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 470, 500, 0, "4. Voce tem tres vidas para acertar.");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 470, 560, 0, "5. A pocao correta muda a cor do caldeirao.");
                Botao voltar = { 50, 850, 250, 920, "VOLTAR" };
                desenharBotao(fonte, voltar);
            }
            else if (telaAtual == JOGO) {
                desenharFundo(fundoJogo, larguraTela, alturaTela);
                // Desenhar o Caldeirão ampliado
                ALLEGRO_BITMAP* imgCaldeiraoAtual = caldeiraoAtivo ? imgCaldeiraoAzul : imgCaldeiraoVermelho;
                if (imgCaldeiraoAtual) {
                    al_draw_scaled_bitmap(
                        imgCaldeiraoAtual, 0, 0,
                        al_get_bitmap_width(imgCaldeiraoAtual), al_get_bitmap_height(imgCaldeiraoAtual),
                        ret.x1, ret.y1, ret.x2 - ret.x1, ret.y2 - ret.y1, 0
                    );
                }
                else {
                    al_draw_filled_rectangle(ret.x1, ret.y1, ret.x2, ret.y2, caldeiraoAtivo ? al_map_rgb(0, 0, 255) : al_map_rgb(255, 0, 0));
                }
                // Desenhar as Poções maiores
                for (int i = 0; i < totalPocoes; i++) {
                    if (pocoes[i].visivel) {
                        if (pocoes[i].imagem) {
                            al_draw_scaled_bitmap(
                                pocoes[i].imagem, 0, 0,
                                al_get_bitmap_width(pocoes[i].imagem), al_get_bitmap_height(pocoes[i].imagem),
                                pocoes[i].x, pocoes[i].y, pocoes[i].largura, pocoes[i].altura, 0
                            );
                        }
                        else {
                            al_draw_filled_rectangle(
                                pocoes[i].x, pocoes[i].y,
                                pocoes[i].x + pocoes[i].largura,
                                pocoes[i].y + pocoes[i].altura,
                                al_map_rgb(120, 80, 180));
                        }
                    }
                }
                // Sistema de vidas: tres coracoes no canto superior esquerdo.
                for (int v = 0; v < vidas; v++) {
                    float xCoracao = 30 + (v * 60);
                    float yCoracao = 25;
                    ALLEGRO_COLOR vermelho = al_map_rgb(255, 0, 0);

                    // Cada coracao e formado por duas partes arredondadas e uma ponta.
                    al_draw_filled_circle(xCoracao + 15, yCoracao + 15, 13, vermelho);
                    al_draw_filled_circle(xCoracao + 33, yCoracao + 15, 13, vermelho);
                    al_draw_filled_triangle(
                        xCoracao + 3, yCoracao + 19,
                        xCoracao + 45, yCoracao + 19,
                        xCoracao + 24, yCoracao + 47,
                        vermelho
                    );
                }
                // Personagem Ani com tamanho aumentado (Largura: 220, Altura: 270)
                if (imagemAni) {
                    al_draw_scaled_bitmap(
                        imagemAni, 0, 0,
                        al_get_bitmap_width(imagemAni), al_get_bitmap_height(imagemAni),
                        20, 680, 220, 270, 0
                    );
                }
                // Balão de texto da Ani
                if (aniFalando) {
                    al_draw_filled_rectangle(250, 750, 800, 830, al_map_rgb(255, 255, 240));
                    al_draw_rectangle(250, 750, 800, 830, al_map_rgb(80, 40, 20), 3);
                    al_draw_text(fonte, al_map_rgb(40, 20, 10), 265, 778, 0, "Salve lek utilize o grimorio para neutralizar a pocao \xf0\x9f\xa4\x99");
                }
                desenharBotao(fonte, botaoGrimorio);
                if (grimorioAberto) {
                    desenharGrimorio(fonte, paginaGrimorio);
                }
            }
            al_flip_display();
        }
    }
    // LIBERAR MEMORIA
    if (fundoMenu) al_destroy_bitmap(fundoMenu);
    if (fundoJogo) al_destroy_bitmap(fundoJogo);
    if (imgCaldeiraoVermelho) al_destroy_bitmap(imgCaldeiraoVermelho);
    if (imgCaldeiraoAzul) al_destroy_bitmap(imgCaldeiraoAzul);
    if (imagemCoracao) al_destroy_bitmap(imagemCoracao);
    if (imagemAni) al_destroy_bitmap(imagemAni);
    if (imgHCl) al_destroy_bitmap(imgHCl);
    if (imgNaOH) al_destroy_bitmap(imgNaOH);
    if (imgNaCl) al_destroy_bitmap(imgNaCl);
    if (imgCO2) al_destroy_bitmap(imgCO2);
    if (imgH2SO4) al_destroy_bitmap(imgH2SO4);
    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(display);
    return 0;
}
