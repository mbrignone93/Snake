#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>

#define ARRIBA     72
#define IZQUIERDA  75
#define DERECHA    77
#define ABAJO      80
#define ESC        27
#define ENTER      13

// colores PARA PINTAR...
#define AZUL     0x009 // MENU
#define AMARILLO 0x00E // COMIDA
#define ROJO     0x00C // LIMITES
#define VERDE    0x002 // SNAKE
#define BLANCO   0x00F // PUNTOS Y ACERCA DE...

int cuerpo[200][2];
int n = 1;
int tam = 4;
int x = 10, y = 12;
int dir = 3;
int xc = 30, yc = 15;
int score = 0;
int pos = 1;

char tecla;

void gotoxy(int x, int y){ // funcion que posiciona
    HANDLE hCon;
    COORD dwPos;
    
    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

char inicio[50][100] = {
"                                                                      ",
"       A A          AAAAA  A    A  AAAAA  A   A  AAAAA                ",
"        A           A      AA   A  A   A  A  A   A                    ",
"        A           AAAAA  A A  A  AAAAA  AAA    AAAAA                ",
"      AAAAA             A  A  A A  A   A  A  A   A                    ",
"     A AAA A        AAAAA  A   AA  A   A  A   A  AAAAA                ",
"    AAAAAAAAA                                                         ",
"    AAAAAAAAA                                                         ",
"     AAAAAAA           AAAAAAAAAAA               AAAAAAAAAAAAAAAA     ",
"     AAAAAAA          AAAAAAAAAAAAAA            AAAAAAAAAAAAAAAAAA    ",
"      AAAAAAA        AAAAAAAAAAAAAAAA          AAAAAAAAAAAAAAAAAA     ",
"       AAAAAAA      AAAAAA     AAAAAAA        AAAAAAA                 ",
"        AAAAAAA    AAAAAA       AAAAAAA      AAAAAA                   ",
"         AAAAAAAAAAAAAAA         AAAAAAAAAAAAAAAAA                    ",
"          AAAAAAAAAAAAA           AAAAAAAAAAAAAAA                     ",
"           AAAAAAAAAAA             AAAAAAAAAAAAA                      ",
"                                                                      ",
};

void menu_inicial(){
    for(int i = 0 ; i< 17 ; i++){
        for(int j = 0 ; j < 71 ; j++){
            if(inicio[i][j] == 'A'){
                gotoxy(j+5,i+5); printf("X");
            }
        }
    }
 
    tecla = getch();
 
    for(int i = 0 ; i< 17 ; i++){
        for(int j = 0 ; j < 71 ; j++){
            if(inicio[i][j] == 'A'){
                gotoxy(j+5,i+5); printf(" ");
            }
        }
    }
}

void pintar(){
    //lineas horizontales
    for (int i = 2; i < 78; i++)
    {
        gotoxy(i,  3); printf("%c", 205);
        gotoxy(i, 23); printf("%c", 205);
    }
    
    //lineas verticales
    for (int i = 4; i < 23; i++)
    {
        gotoxy(2,  i); printf("%c", 186);
        gotoxy(77, i); printf("%c", 186);
    }
    
    // esquinas
    gotoxy(2,3); printf("%c", 201);
    gotoxy(2,23); printf("%c", 200);
    gotoxy(77,3); printf("%c", 187);
    gotoxy(77,23); printf("%c", 188);
}

void OcultaCursor() {
    CONSOLE_CURSOR_INFO cci = {100, FALSE}; // El segundo parametro de la estructura indica si se muestra el cursor o no.

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void Color(int color){
	static HANDLE hConsole;
    hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hConsole, color | (0 * 0x10 + 0x100) );
}

void guardar_posicion(){
    cuerpo[n][0] = x;
    cuerpo[n][1] = y;
    n++;
    if (n == tam) n = 1;
}

void pintar_cuerpo(){
    for (int i = 1; i < tam; i++)
    {   
        gotoxy(cuerpo[i][0], cuerpo[i][1]);
        printf("*");
    }
}

void borrar_cuerpo(){
    gotoxy(cuerpo[n][0], cuerpo[n][1]);
    printf(" ");
}

void teclear(){    
    if (kbhit()){
            tecla = getch();
            switch(tecla){
                case ARRIBA :
                     if (dir != 2) // para hacer movientos cuadrados tipicos de SNAKE
                        dir = 1;
                        break;
                case ABAJO :
                     if (dir != 1)
                        dir = 2;
                        break;
                case DERECHA :
                     if (dir != 4)
                        dir = 3;
                        break;
                case IZQUIERDA :
                     if (dir != 3)
                        dir = 4;
                        break;
            }
    }
}

void comida(){
    if (x == xc && y == yc){  
        xc = (rand()%73) + 4;
        yc = (rand()%18) + 4;
        
        tam++;
        score += 10;
        Color(AMARILLO); gotoxy(xc, yc); printf("%c", 4);
    }
}

bool game_over(){
    if (y == 3 || y == 23 || x == 2 || x == 77) return false;
    for (int j = tam -1; j > 0; j--){
        if (cuerpo[j][0] == x && cuerpo[j][1] == y)
           return false;
    }
    /*
      Este for sirve para checkear cuando Snake choque con si misma,
      entonces devolvera false (se acaba el juego)
    */
    
    return true;
}

void puntos(){
    gotoxy(3, 1);
    Color(BLANCO); printf("PUNTOS: %d", score);
}

int menu_final(void){
 
    if(kbhit()){
        tecla = getch();
        if(tecla == ARRIBA){
            gotoxy(30,12); printf("%c   JUGAR DE NUEVO",16);
            gotoxy(30,15); printf("    SALIR DEL JUEGO");
            pos = 1;
        }
 
        if(tecla == ABAJO){
            gotoxy(30,12); printf("    JUGAR DE NUEVO");
            gotoxy(30,15); printf("%c   SALIR DEL JUEGO",16);
            pos = 0;
        }
 
        if(tecla == ENTER)
           return 1;
    }
    return 0;
}

void interacion_menu_final(){
    borrar_cuerpo();
    gotoxy(xc,yc); printf(" ");
 
    gotoxy(35,1); printf(" PRESS ENTER");
    gotoxy(30,12); printf("%c   JUGAR DE NUEVO",16);
    gotoxy(30,15); printf("    SALIR DEL JUEGO");
    do{
 
      menu_final();
    }while (!menu_final());
    gotoxy(35,1); printf("            ");
    gotoxy(30,12); printf("                    ");
    gotoxy(30,15); printf("                    ");
}

void iniciar_datos(){ // REINICAR JUEGO
     for(int j = tam ; j > 0  ; j--){   // Borramos el cuerpo de la partida anterior
         gotoxy(cuerpo[j][0],cuerpo[j][1]); printf(" ");
     }
     for(int j = 0 ; j <= tam  ; j++){   // reseteamos las espacios de la matriz cuerpo
         for(int i = 0 ; i < 2 ; i++){
               cuerpo[j][i] = '\0';
         }
 
     }
     score = 0;
     x = 10 ; 
     y = 12 ; 
     tam = 3; 
     dir = 3; 
     n = 0;
 
}

int main()
{
    system("cls"); // limpiamos la consola
    
    OcultaCursor();
    srand(time(NULL));
    Color(ROJO); pintar();
    Color(AZUL); menu_inicial();
    do{
    //system("color 8F");
    Color(ROJO); pintar();
    iniciar_datos();
    Color(AMARILLO); gotoxy(xc, yc); printf("%c", 4); // pintamos la primer comida
    gotoxy(44, 1); Color(BLANCO); printf("POR MAXIMILIANO BRIGNONE C++ 2015");
    
    do
    {
        borrar_cuerpo();
        guardar_posicion();
        Color(VERDE); pintar_cuerpo();
        comida();
        puntos();
        teclear();
        teclear();
        
        if (dir == 1) y--;
        if (dir == 2) y++;
        if (dir == 3) x++;
        if (dir == 4) x--;
        
        Sleep(100);
    } while(tecla != ESC && game_over()); // FIN DEL WHILE
    interacion_menu_final();
    
    }while(pos);
    return 0;
}
