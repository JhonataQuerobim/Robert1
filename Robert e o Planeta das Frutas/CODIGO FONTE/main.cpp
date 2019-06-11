#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "Pilha.cpp"

using namespace std;

sf::RenderWindow window(sf::VideoMode(880, 600), "Robert e o Planeta das Frutas");
sf::Image icon;
string endereco[50];
sf::Texture texturas[50];
sf::Sprite sprites[50];
sf::Music reloginho, sino, perdeu, musica;
int mapa[20][15];
int personagem[2];
int pontos = 0;
int historia = 0;
int inicio = 0;
int vidas = 3;
int fase = 1;
int win = 0;
int start = 0;
int coletou = 0;
sf::Clock c;
sf::Clock teclaPress;
double vel = 0.08;

typedef struct
{
    int x;
    int y;
    int codFruta;
} fruta;

Pilha<fruta> pilhaFruta;
Pilha<fruta> pilhaColetadas;

void setEnderecos()
{
    for(int i = 0; i < 50; i++)
        endereco[i] = "./IMAGENS/Chao.png";
    endereco[0] = "./IMAGENS/Chao.png";
    endereco[1] = "./IMAGENS/Parede.png";
    endereco[2] = "./IMAGENS/Ovni1.png";
    endereco[3] = "./IMAGENS/Ovni2.png";
    endereco[5] = "./IMAGENS/Spike.png";
    endereco[6] = "./IMAGENS/Buraco.png";
    endereco[10] = "./IMAGENS/Placar.png";
    endereco[11] = "./IMAGENS/CoracaoPequeno.png";
    endereco[20] = "./IMAGENS/Laranja.png";
    endereco[21] = "./IMAGENS/Uva.png";
    endereco[22] = "./IMAGENS/Cereja.png";
    endereco[23] = "./IMAGENS/Melancia.png";
    endereco[24] = "./IMAGENS/Kiwi.png";
    endereco[25] = "./IMAGENS/Morango.png";
    endereco[26] = "./IMAGENS/Ameixa.png";
    endereco[27] = "./IMAGENS/LimaoInteiro.png";
    endereco[28] = "./IMAGENS/Goiaba.png";
    endereco[29] = "./IMAGENS/Maca.png";
}

void setTexturas()
{
    for(int i =0; i < 50; i++)
        texturas[i].loadFromFile(endereco[i]);
    cout << "TEXTURAS CARREGADAS\n";
}

void setSprites()
{
    for(int i =0; i < 50; i++)
        sprites[i].setTexture(texturas[i]);
    cout << "SPRITES CRIADOS\n";
}

void desenhaMapa()
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            if(((mapa[i][j] >= 20) && (mapa[i][j] <= 30)) || mapa[i][j] == 5)
            {
                sprites[0].setPosition(sf::Vector2f(i*40,j*40));
                window.draw(sprites[0]);
            }
            if(inicio == 1 || ((mapa[i][j] < 20) || (mapa[i][j] > 30)))
            {
                sprites[mapa[i][j]].setPosition(sf::Vector2f(i*40,j*40));
                window.draw(sprites[mapa[i][j]]);
            }
        }
    }
    sprites[10].setPosition(sf::Vector2f(800,0));
    window.draw(sprites[10]);
}

void desenhaPersonagem()
{
    int tempo;
    tempo = c.getElapsedTime().asMilliseconds()/500;
    if( tempo%2 == 0)
    {
        sprites[2].setPosition(sf::Vector2f(40*personagem[0],40*personagem[1]));
        window.draw(sprites[2]);
    }
    else
    {
        sprites[3].setPosition(sf::Vector2f(40*personagem[0],40*personagem[1]));
        window.draw(sprites[3]);
    }

}

void adicionaFrutas(int x, int y, int cod)
{
    fruta aux;
    aux.x = x;
    aux.y = y;
    aux.codFruta = cod;
    pilhaFruta.Insere(aux);
}

void leMapa()
{
    string linha;
    ifstream arquivo;

    fruta f;
    while(!pilhaFruta.EstaVazia())
        pilhaFruta.Retira(f);
    while(!pilhaColetadas.EstaVazia())
        pilhaColetadas.Retira(f);
    int l = 0;
    int x, y, cod;
    if(fase == 1)
        arquivo.open("./MAPAS/mapa1.txt");
    if(fase == 2)
        arquivo.open("./MAPAS/mapa2.txt");
    if(fase == 3)
        arquivo.open("./MAPAS/mapa3.txt");
    if(fase == 4)
        arquivo.open("./MAPAS/mapa4.txt");
    if(fase == 5)
        arquivo.open("./MAPAS/mapa5.txt");
    if(fase == 6)
        arquivo.open("./MAPAS/mapa6.txt");
    if(fase == 7)
        arquivo.open("./MAPAS/mapa7.txt");
    if(arquivo.is_open())
    {
        cout << "ARQUIVO ABERTO COM SUCESSO\n";
        while(getline(arquivo, linha))
        {
            if(l<15)
            {
                for(int j = 0; j < 20; j++)
                {
                    mapa[j][l] = linha[j]-48;
                }
                l++;
            }
            else
            {
                //cout << "V";
                x = linha[1]-65;
                y = linha[2]-65;
                cod = linha[0]-77;
                mapa[x][y] = cod;
                adicionaFrutas(x,y,cod);
                l++;
            }
        }
        arquivo.close();
    }
    else
        cout << "ERRO AO ABRIR O ARQUIVO DE MAPAS\n";
}

void mudarFase()
{
    if(fase == 7)
    {
        win = 1;
        sf::Texture t;
        t.loadFromFile("./IMAGENS/Vitoria.png");
        sf::Sprite sprT;
        sprT.setTexture(t);
        sprT.setPosition(0,0);
        window.draw(sprT);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            pontos = 0;
            historia = 0;
            inicio = 0;
            vidas = 3;
            fase = 1;
            win = 0;
            start = 0;
            coletou = 0;
            leMapa();
            personagem[0] = 1;
            personagem[1] = 1;
            sf::sleep(sf::milliseconds(200));
        }
    }
    else
    {
        sf::RectangleShape ret(sf::Vector2f(800,600));
        ret.setFillColor(sf::Color(255,128,0,128));
        ret.setPosition(sf::Vector2f(0,0));
        window.draw(ret);
        win = 1;
        sf::Font fonte;
        fonte.loadFromFile("./FONTES/AllerDisplay.ttf");
        sf::Text texto;
        texto.setFont(fonte);
        texto.setFillColor(sf::Color::Black);
        texto.setPosition(190,400);
        texto.setCharacterSize(40);
        texto.setString("Clique para continuar!");
        window.draw(texto);
    }
}

void desenhaPlacar()
{
    Pilha<fruta> aux;
    fruta a;
    int tam = 0, s = 0;
    sf::Font fonte;
    fonte.loadFromFile("./FONTES/cour.ttf");
    sf::Text texto;
    string pontuacao, fasestr;
    sf::Time t;
    while(!pilhaColetadas.EstaVazia())
    {
        pilhaColetadas.Retira(a);
        aux.Insere(a);
        sprites[a.codFruta].setPosition(sf::Vector2f(820, 200+tam*40));
        window.draw(sprites[a.codFruta]);
        tam++;
    }
    while(!aux.EstaVazia())
    {
        aux.Retira(a);
        pilhaColetadas.Insere(a);
    }

    for(int j = 0; j < vidas; j++)
    {
        sprites[11].setPosition(770-40*j, 570);
        window.draw(sprites[11]);
    }
    texto.setFont(fonte);
    texto.setCharacterSize(20);
    texto.setFillColor(sf::Color::Black);
    if(pontos > 9)
            texto.setPosition(sf::Vector2f(844,12));
    else
        texto.setPosition(sf::Vector2f(850,15));
    ostringstream ss, tt, ff;
    ss << pontos;
    ff << fase;
    pontuacao = ss.str();
    fasestr = ff.str();
    texto.setString(pontuacao);
    window.draw(texto);
    if(vidas > 0)
    {
        t = c.getElapsedTime();
        s = t.asSeconds();
        s -= 10;
        if(s < 0)
            tt << (s*(-1));
        else
            if(!win)
                tt << 120 - s;
        pontuacao = tt.str();
        texto.setString(pontuacao);
        if(s == -1 && sino.getStatus() != 2)
        {
            sino.openFromFile("./AUDIOS/Bell.ogg");
            sino.play();
        }
        if(s > 109)
        {
            texto.setFillColor(sf::Color::Red);
            if(s == 110 && reloginho.getStatus() != 2)
            {
                reloginho.openFromFile("./AUDIOS/Clock.ogg");
                reloginho.play();
            }
        }
        if(s < 0)
            texto.setFillColor(sf::Color::Blue);
        if(s > 120 && !win)
        {
            vidas=0;
            perdeu.openFromFile("./AUDIOS/Perdeu.ogg");
            perdeu.play();
        }
        if(s > 120 || win)
            reloginho.stop();
    }
    else
    {
        texto.setString("");
    }
    texto.setCharacterSize(20);
    if((120 - s) > 99 && s >= 0)
        texto.setPosition(sf::Vector2f(838,75));
    else
    {
        if(((120 - s) > 9 && (s > 0)) || (120 - s) == -10 || s < -9)
            texto.setPosition(sf::Vector2f(844,75));
        else
            texto.setPosition(sf::Vector2f(850,75));
    }
    if(s <= 120)
        window.draw(texto);
    texto.setString(fasestr);
    texto.setFillColor(sf::Color::Black);
    texto.setPosition(sf::Vector2f(850,138));
    window.draw(texto);
    if(vidas <= 0 && win == 0)
    {
        vidas = 0;
        sf::Texture t;
        t.loadFromFile("./IMAGENS/Derrota.png");
        sf::Sprite sprT;
        sprT.setTexture(t);
        sprT.setPosition(0,0);
        window.draw(sprT);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            pontos = 0;
            historia = 0;
            inicio = 0;
            vidas = 3;
            fase = 1;
            win = 0;
            start = 0;
            coletou = 0;
            leMapa();
            personagem[0] = 1;
            personagem[1] = 1;
            sf::sleep(sf::milliseconds(200));
        }
    }
    if(pilhaFruta.EstaVazia())
    {
        mudarFase();
    }
}

void inicia()
{
    Pilha<fruta> aux;
    fruta a;
    sf::Time t;
    int j = 0;
    float s;
    t = c.getElapsedTime();
    s = t.asSeconds();
    if(s < 7 && inicio==0)
    {
        for(int sp = 20; sp < 30; sp++)
            sprites[sp].setColor(sf::Color(255,255,255,255-36*s));
    }
    if(s > 10 && !win)
    {
        for(int sp = 20; sp < 30; sp++)
        sprites[sp].setColor(sf::Color(255,255,255,255));
        inicio=1;
    }
    while(!pilhaFruta.EstaVazia() && inicio == 0)
    {
        pilhaFruta.Retira(a);
        aux.Insere(a);
        sprites[a.codFruta].setPosition(sf::Vector2f(820, 185+j*40));
        window.draw(sprites[a.codFruta]);
        j++;
    }
    while(!aux.EstaVazia())
    {
        aux.Retira(a);
        pilhaFruta.Insere(a);
    }
    sf::Font fonte;
    fonte.loadFromFile("./FONTES/AllerDisplay.ttf");
    sf::Text texto;
    texto.setFont(fonte);
    texto.setFillColor(sf::Color::Black);
    if(s < 9 && s > 8  && inicio == 0)
    {
        texto.setPosition(sf::Vector2f(808,210));
        texto.setCharacterSize(13);
        texto.setString("Preparar!");
        window.draw(texto);
    }
    if(s < 10 && s > 9  && inicio == 0)
    {
        texto.setPosition(sf::Vector2f(819,200));
        texto.setCharacterSize(50);
        texto.setString("Já");
        window.draw(texto);
    }
}

void movimenta(char dir)
{
    if(inicio)
    {
        coletou = 0;
        fruta aux;
        int movex, movey;
        if(dir == 'w')
        {
            movex = 0;
            movey = -1;
        }
        if(dir == 's')
        {
            movex = 0;
            movey = +1;
        }
        if(dir == 'a')
        {
            movey = 0;
            movex = -1;
        }
        if(dir == 'd')
        {
            movey = 0;
            movex = +1;
        }

        if(mapa[personagem[0]+movex][personagem[1]+movey] != 1)
        {
            personagem[0] += movex;
            personagem[1] += movey;
            if(mapa[personagem[0]][personagem[1]] >= 20 && mapa[personagem[0]][personagem[1]] <= 30)
            {
                pilhaFruta.Retira(aux);
                if(mapa[personagem[0]][personagem[1]] == aux.codFruta)
                {
                    pilhaColetadas.Insere(aux);
                    pontos++;
                    mapa[aux.x][aux.y] = 0;
                    coletou=1;
                }
                else
                {
                    if(!pilhaColetadas.EstaVazia())
                    {
                        coletou--;
                        if(pontos > 0)
                        {
                            if(pontos == 1)
                                pontos = 0;
                            else
                                pontos -= 2;
                        }
                        pilhaFruta.Insere(aux);
                        pilhaColetadas.Retira(aux);
                        pilhaFruta.Insere(aux);
                        mapa[aux.x][aux.y] = aux.codFruta;
                    }
                    else
                    {
                        if(pontos > 0)
                        {
                            if(pontos == 1)
                                pontos = 0;
                            else
                                pontos -= 2;
                        }
                        coletou = -3;
                        vidas--;
                        pilhaFruta.Insere(aux);
                    }
                }
            }
            if(mapa[personagem[0]][personagem[1]] == 6)
            {
                coletou = -2;
                personagem[0] = 1;
                personagem[1] = 1;
            }
            if(mapa[personagem[0]][personagem[1]] == 5)
            {
                coletou = -3;
                vidas--;
            }
        }
    }
}

void mainMenu()
{
        sf::RectangleShape ret(sf::Vector2f(880,600));
        ret.setFillColor(sf::Color(250,250,250));
        ret.setPosition(sf::Vector2f(0,0));
        window.draw(ret);
        sf::Texture t;
        t.loadFromFile("./IMAGENS/Titulo.png");
        sf::Sprite sprT;
        sprT.setTexture(t);
        sprT.setPosition(0,0);
        window.draw(sprT);
        if(musica.getStatus() != 2)
        {
            musica.openFromFile("./AUDIOS/Abertura.ogg");
            musica.play();
        }
        sf::Font fonte;
        fonte.loadFromFile("./FONTES/cour.ttf");
}

int main()
{
    sf::Time tempoTecla;
    int flag = 1;
    sf::Texture t;
    sf::Sprite sprT;
    sprT.setPosition(0,0);
    window.setFramerateLimit(30);
    icon.loadFromFile("./IMAGENS/Ovni1.png");
    window.setIcon(40, 40, icon.getPixelsPtr());
    setEnderecos();
    setTexturas();
    setSprites();
    leMapa();
    personagem[0] = 1;
    personagem[1] = 1;
    sf::SoundBuffer buff[4];
    buff[0].loadFromFile("./AUDIOS/Pega_fruta.ogg");
    buff[1].loadFromFile("./AUDIOS/Dropa_fruta.ogg");
    buff[2].loadFromFile("./AUDIOS/Buraco.ogg");
    buff[3].loadFromFile("./AUDIOS/Dano.ogg");
    sf::Sound som[4];
    for(int i = 0; i < 4; i++)
        som[i].setBuffer(buff[i]);
    sf::SoundSource::Status estado[4];
    int somTocando = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if(start == 1)
        {
            if(vidas == 0)
                for(int i=0;i<4;i++)
                    som[i].stop();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && vidas>0)
            {
                if(teclaPress.getElapsedTime().asSeconds() > vel)
                {
                    movimenta('w');
                    teclaPress.restart();
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && vidas>0)
            {
                if(teclaPress.getElapsedTime().asSeconds() > vel)
                {
                    movimenta('s');
                    teclaPress.restart();
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  && vidas>0)
            {
                if(teclaPress.getElapsedTime().asSeconds() > vel)
                {
                    movimenta('a');
                    teclaPress.restart();
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && vidas>0)
            {
                if(teclaPress.getElapsedTime().asSeconds() > vel)
                {
                    movimenta('d');
                    teclaPress.restart();
                }
            }
            if(coletou!=0)
            {
                for(int i=0;i<4;i++)
                        som[i].pause();
                for(int i = 0; i < 4; i++)
                {
                    estado[i] = som[i].getStatus();
                    if(estado[i] == 2)
                        somTocando = 1;
                }
                if(somTocando != 1)
                    somTocando = 0;
                if(somTocando == 0)
                {
                    if(coletou == 1)
                        som[0].play();
                    if(coletou == -1)
                        som[1].play();
                    if(coletou == -2)
                        som[2].play();
                    if(coletou == -3)
                        som[3].play();
                }
                coletou=0;
                if(vidas==0)
                {
                    musica.openFromFile("./AUDIOS/Perdeu.ogg");
                    musica.play();
                }
            }
            if(win==1 && flag)
            {
                inicio = 0;
                if(fase == 7)
                {
                    musica.openFromFile("./AUDIOS/Ganhou_o_jogo.ogg");
                    musica.play();
                }
                else
                {
                    musica.openFromFile("./AUDIOS/Passou_de_fase.ogg");
                    musica.play();
                }
                flag--;
            }
            if((sf::Mouse::isButtonPressed(sf::Mouse::Left) && win==1))
            {
                if(fase < 7)
                {
                    fase++;
                    if(vidas<6)
                        vidas++;
                    c.restart();
                    leMapa();
                    inicio = 0;
                    start = 1;
                    win = 0;
                    flag = 1;
                    fruta auxiliar;
                    while(!pilhaColetadas.EstaVazia())
                        pilhaColetadas.Retira(auxiliar);
                    personagem[0] = 1;
                    personagem[1] = 1;
                }
            }
            desenhaMapa();
            desenhaPersonagem();
            desenhaPlacar();
            inicia();
        }
        else
        {
            if(historia == 0)
                mainMenu();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                historia++;
                sf::sleep(sf::milliseconds(200));
            }
            if(historia == 1)
            {
                t.loadFromFile("./IMAGENS/Pagina1.png");
                sprT.setTexture(t);
                window.draw(sprT);
            }
            if(historia == 2)
            {
                t.loadFromFile("./IMAGENS/Pagina2.png");
                sprT.setTexture(t);
                window.draw(sprT);
            }
            if(historia == 3)
            {
                t.loadFromFile("./IMAGENS/Pagina3.png");
                sprT.setTexture(t);
                window.draw(sprT);
            }
            if(historia == 4)
            {
                t.loadFromFile("./IMAGENS/Pagina4.png");
                sprT.setTexture(t);
                window.draw(sprT);
            }
            if(historia == 5)
            {
                historia = 0;
                start = 1;
                inicio = 0;
                c.restart();
                musica.pause();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                start = 1;
                inicio = 0;
                c.restart();
                musica.pause();
            }
        }
        window.display();
    }
    return 0;
}
