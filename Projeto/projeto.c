

#include <stdlib.h> 

#include <stdbool.h> 

#include<locale.h> 

#include <allegro5/allegro.h> 

#include <allegro5/allegro_primitives.h> 

#include <allegro5/allegro_image.h> 

#include <allegro5/allegro_font.h> 



// Estrutura dos circulos 

typedef struct {

    float x;

    float y;

    int raio;

    ALLEGRO_COLOR cor;

    bool visivel;

} Circulo;



// Estrutura do retangulo 

typedef struct {

    float x1, y1;

    float x2, y2;

} Retangulo;



// Estrutura dos botoes 

typedef struct {

    float x1, y1;

    float x2, y2;

    char texto[30];

} Botao;



// Controle das telas 

typedef enum {

    MENU,

    JOGO,

    INSTRUCAO

} Tela;



// Funcao para desenhar os botoes 

void desenharBotao(ALLEGRO_FONT* fonte, Botao botao)

{

    ALLEGRO_COLOR corBotao = al_map_rgb(65, 35, 80);

    ALLEGRO_COLOR corBorda = al_map_rgb(180, 140, 200);

    ALLEGRO_COLOR corTexto = al_map_rgb(255, 255, 255);



    al_draw_filled_rectangle(

        botao.x1, botao.y1,

        botao.x2, botao.y2,

        corBotao

    );



    al_draw_rectangle(

        botao.x1, botao.y1,

        botao.x2, botao.y2,

        corBorda, 3

    );



    al_draw_text(

        fonte,

        corTexto,

        (botao.x1 + botao.x2) / 2,

        (botao.y1 + botao.y2) / 2 - 12,

        ALLEGRO_ALIGN_CENTER,

        botao.texto

    );

}



// Funcao para verificar clique no botao 

bool clicouBotao(Botao botao, float mouseX, float mouseY)

{

    return mouseX >= botao.x1 &&

        mouseX <= botao.x2 &&

        mouseY >= botao.y1 &&

        mouseY <= botao.y2;

}



// Funcao para desenhar uma imagem de fundo 

void desenharFundo(ALLEGRO_BITMAP* imagem,

    int larguraTela, int alturaTela)

{

    if (imagem)

    {

        al_draw_scaled_bitmap(

            imagem,

            0, 0,

            al_get_bitmap_width(imagem),

            al_get_bitmap_height(imagem),

            0, 0,

            larguraTela,

            alturaTela,

            0

        );

    }

    else

    {

        al_clear_to_color(al_map_rgb(20, 15, 25));

    }

}



int main()

{

    // Inicializacao da Allegro 

    al_init();

    al_install_mouse();



    al_init_primitives_addon();

    al_init_image_addon();

    al_init_font_addon();



    int larguraTela = 1920;

    int alturaTela = 1000;



    // Criacao da janela 

    ALLEGRO_DISPLAY* display =

        al_create_display(larguraTela, alturaTela);



    // Carregamento das imagens de fundo 

    ALLEGRO_BITMAP* fundoMenu =

        al_load_bitmap("menu.png");



    ALLEGRO_BITMAP* fundoJogo =

        al_load_bitmap("fundo.png");



    // Fonte 

    ALLEGRO_FONT* fonte = al_create_builtin_font();



    // Timer 60 FPS 

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);



    // Fila de eventos 

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();



    al_register_event_source(

        fila,

        al_get_display_event_source(display)

    );



    al_register_event_source(

        fila,

        al_get_mouse_event_source()

    );



    al_register_event_source(

        fila,

        al_get_timer_event_source(timer)

    );



    // Botoes do menu 

    Botao jogar = { 760, 400, 1160, 480, "JOGAR" };



    Botao instrucao = { 760, 510, 1160, 590, "INSTRUÇÃO" };



    Botao sair = { 760, 620, 1160, 700, "SAIR" };



    Botao voltar = { 50, 850, 250, 920, "VOLTAR" };



    // Tela inicial 

    Tela telaAtual = MENU;



    // Cores dos circulos 

    ALLEGRO_COLOR corVermelho = al_map_rgb(255, 0, 0);

    ALLEGRO_COLOR corAzul = al_map_rgb(0, 0, 255);

    ALLEGRO_COLOR corVerde = al_map_rgb(0, 255, 0);



    // Cor inicial do retangulo 

    ALLEGRO_COLOR corRetanguloAtual =

        al_map_rgb(0, 0, 0);



    // Retangulo 

    Retangulo ret = { 700, 800, 1200, 1000 };



    // Circulos 

    Circulo circulos[3] = {

        { 1650, 130, 50, corVermelho, true },

        { 1800, 320, 50, corAzul, true },

        { 1580, 480, 50, corVerde, true }

    };



    int totalCirculos = 3;



    // Controle de arrastar 

    bool arrastando = false;

    int circuloSelecionado = -1;



    float offsetX = 0;

    float offsetY = 0;



    bool estaRodando = true;

    bool redesenhar = true;



    al_start_timer(timer);



    // Loop principal 

    while (estaRodando)

    {

        ALLEGRO_EVENT evento;



        al_wait_for_event(fila, &evento);



        // Fechar janela 

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)

        {

            estaRodando = false;

        }



        // Timer 

        else if (evento.type == ALLEGRO_EVENT_TIMER)

        {

            redesenhar = true;

        }



        // Clique do mouse 

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)

        {

            if (evento.mouse.button == 1)

            {

                float mouseX = evento.mouse.x;

                float mouseY = evento.mouse.y;



                // MENU PRINCIPAL 

                if (telaAtual == MENU)

                {

                    if (clicouBotao(jogar, mouseX, mouseY))

                    {

                        telaAtual = JOGO;

                    }



                    else if (clicouBotao(instrucao, mouseX, mouseY))

                    {

                        telaAtual = INSTRUCAO;

                    }



                    else if (clicouBotao(sair, mouseX, mouseY))

                    {

                        estaRodando = false;

                    }

                }



                // TELA DE INSTRUÇÃO 

                else if (telaAtual == INSTRUCAO)

                {

                    if (clicouBotao(voltar, mouseX, mouseY))

                    {

                        telaAtual = MENU;

                    }

                }



                // TELA DO JOGO 

                else if (telaAtual == JOGO)

                {

                    for (int i = totalCirculos - 1; i >= 0; i--)

                    {

                        if (!circulos[i].visivel)

                            continue;



                        float distanciaX =

                            mouseX - circulos[i].x;



                        float distanciaY =

                            mouseY - circulos[i].y;



                        if ((distanciaX * distanciaX +

                            distanciaY * distanciaY) <=

                            (circulos[i].raio *

                                circulos[i].raio))

                        {

                            arrastando = true;

                            circuloSelecionado = i;



                            offsetX = mouseX - circulos[i].x;

                            offsetY = mouseY - circulos[i].y;



                            break;

                        }

                    }

                }

            }

        }



        // Movimentacao do mouse 

        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)

        {

            if (telaAtual == JOGO &&

                arrastando &&

                circuloSelecionado != -1)

            {

                circulos[circuloSelecionado].x =

                    evento.mouse.x - offsetX;



                circulos[circuloSelecionado].y =

                    evento.mouse.y - offsetY;

            }

        }



        // Soltar o botao do mouse 

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)

        {

            if (evento.mouse.button == 1)

            {

                if (telaAtual == JOGO &&

                    arrastando &&

                    circuloSelecionado != -1)

                {

                    float cx =

                        circulos[circuloSelecionado].x;



                    float cy =

                        circulos[circuloSelecionado].y;



                    // Verifica se o circulo esta dentro do retangulo 

                    if (cx >= ret.x1 &&

                        cx <= ret.x2 &&

                        cy >= ret.y1 &&

                        cy <= ret.y2)

                    {

                        corRetanguloAtual =

                            circulos[circuloSelecionado].cor;



                        circulos[circuloSelecionado].visivel =

                            false;

                    }

                }



                arrastando = false;

                circuloSelecionado = -1;

            }

        }



        // DESENHO DAS TELAS 

        if (redesenhar && al_is_event_queue_empty(fila))

        {

            redesenhar = false;



            // TELA MENU 

            if (telaAtual == MENU)

            {

                // Fundo exclusivo do menu 

                desenharFundo(

                    fundoMenu,

                    larguraTela,

                    alturaTela

                );



                al_draw_text(

                    fonte,

                    al_map_rgb(230, 200, 150),

                    larguraTela / 2,

                    220,

                    ALLEGRO_ALIGN_CENTER,

                    "O LABORATORIO DA BRUXA"

                );



                desenharBotao(fonte, jogar);

                desenharBotao(fonte, instrucao);

                desenharBotao(fonte, sair);

            }



            // TELA INSTRUÇÃO 

            else if (telaAtual == INSTRUCAO)

            {

                // Utiliza o mesmo fundo do menu 

                desenharFundo(

                    fundoMenu,

                    larguraTela,

                    alturaTela

                );



                al_draw_text(

                    fonte,

                    al_map_rgb(230, 200, 150),

                    larguraTela / 2,

                    200,

                    ALLEGRO_ALIGN_CENTER,

                    "INSTRUÇÃO"

                );



                al_draw_text(

                    fonte,

                    al_map_rgb(255, 255, 255),

                    larguraTela / 2,

                    350,

                    ALLEGRO_ALIGN_CENTER,

                    "Bem-vindo ao laboratorio da bruxa!"

                );



                al_draw_text(

                    fonte,

                    al_map_rgb(255, 255, 255),

                    larguraTela / 2,

                    410,

                    ALLEGRO_ALIGN_CENTER,

                    "Clique em JOGAR para iniciar a atividade."

                );



                desenharBotao(fonte, voltar);

            }



            // TELA JOGO - PRIMEIRA FASE 

            else if (telaAtual == JOGO)

            {

                // Fundo exclusivo da primeira fase 

                desenharFundo(

                    fundoJogo,

                    larguraTela,

                    alturaTela

                );



                // Desenha o retangulo 

                al_draw_filled_rectangle(

                    ret.x1,

                    ret.y1,

                    ret.x2,

                    ret.y2,

                    corRetanguloAtual

                );



                // Desenha os circulos visiveis 

                for (int i = 0; i < totalCirculos; i++)

                {

                    if (circulos[i].visivel)

                    {

                        al_draw_filled_circle(

                            circulos[i].x,

                            circulos[i].y,

                            circulos[i].raio,

                            circulos[i].cor

                        );

                    }

                }

            }



            al_flip_display();

        }

    }



    // Liberacao da memoria 

    if (fundoMenu)

        al_destroy_bitmap(fundoMenu);



    if (fundoJogo)

        al_destroy_bitmap(fundoJogo);



    al_destroy_font(fonte);

    al_destroy_timer(timer);

    al_destroy_event_queue(fila);

    al_destroy_display(display);



    return 0;

}



