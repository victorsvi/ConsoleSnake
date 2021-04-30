/**
Made by Victor Henrique Salvi victorsvi@gmail.com
Version 2016.08.29

Snake game played in TUI implemented with the conio2 library

Controls: arrow keys

can be initialized with custom size and speed:
    start Snake.c WIDHT HEIGHT SPEED INITLEN
*/


#include <conio2.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

//getch returns 2 byte char for arrow keys
#define UP 'H'
#define DOWN 'P'
#define RIGHT 'M'
#define LEFT 'K'

//#define INITLEN 10
//#define SPEED 50

#define WALL ' '
#define SNAKE ' '
#define FOOD '@'

//#define WIDTH 54 //45
//#define HEIGHT 38 //25
#define WINHEIGHT HEIGHT+4
#define MAXLEN (WIDTH-2)*(HEIGHT-2)

#define BKGFRAME textbackground(0)  //black
#define BKGFIELD textbackground(15) //white
#define BKGSNAKE textbackground(2)  //green
#define BKGWIN   textbackground(12) //red
#define BKGLOSE  textbackground(9)  //blue
#define TXTINFO  textcolor(7)       //lightgray
#define TXTFOOD  textcolor(12)      //red

/** */
typedef struct pos {
int x;
int y;
struct pos *next;
} coord;

void buildFrame (void);
void delay(int milliseconds);
int moveSnake (coord **snake, int *x, int *y, int direction, int *hasFood, int *snkln);
void enqueue (coord **queue, coord *last);
coord *dequeue (coord **queue);
void initSnake (coord **snake);
int hasCoordinate (coord *queue, int x, int y);
//int snklen (coord *snake); //deprecated
int hasFreeSpace (coord *snake); //deprecated
void clearqueue(coord *queue);

static int WIDTH, HEIGHT, SPEED, INITLEN;

int main(int argc, char **argv) {

    if (argc != 5){
        WIDTH = 26; //26
        HEIGHT = 18; //18
        SPEED = 80; //80
        INITLEN = 13;
    }
    else {
        sscanf(argv[1],"%d",&WIDTH);
        sscanf(argv[2],"%d",&HEIGHT);
        sscanf(argv[3],"%d",&SPEED);
        sscanf(argv[4],"%d",&INITLEN);
    }

    char *winsize = malloc(50*sizeof(char));
	/* OPERATING SYSTEM DEPENDENT CODE. WORKS ONLY ON WINDOWS */
    sprintf(winsize,"mode con: cols=%d lines=%d",WIDTH,WINHEIGHT);
    system (winsize); //sets the console size
    free(winsize);

    coord *snake = NULL;
    int x, y;
    char input;
    char direction;
    int hasFood;
    int isAlive;
    int snkln;

    do {
// *** Beginning of preparation to start ***
        BKGFIELD;
        TXTINFO;
        clrscr(); //apply the background color
        cputsxy(1,HEIGHT+4,"Made by Victor Salvi");
        BKGFRAME;
        buildFrame();

        clearqueue(snake);
        snake = NULL;
        initSnake(&snake);

        snkln = INITLEN;
        hasFood = 0;
        x = -1;
        y = -1;

        direction = RIGHT;

        int currtime = 0;
        int lasttime = 0;
// *** End of preparation to start ***

        do {

            currtime = clock();
            delay(80 - (currtime - lasttime));
            lasttime = currtime;

            if(hasFood == 0){
                srand(time(NULL));
                if(rand()%2 == 1){
                    srand(time(NULL));
                    if(hasFreeSpace(snake)){
                        do{
                            x = (rand()%(WIDTH -2))+2;
                            y = (rand()%(HEIGHT -2)+2);
                        }while(hasCoordinate(snake,x,y));
                        TXTFOOD;
                        putchxy(x,y,FOOD);
                        TXTINFO;
                        hasFood = 1;
                    }
                }
            }

            if(kbhit()){
                if(getch() == 224){
                    input = getch();

                    switch (input){
                    case UP:
                        if (direction != DOWN)
                            direction = input;
                        break;
                    case DOWN:
                        if (direction != UP)
                            direction = input;
                        break;
                    case RIGHT:
                        if (direction != LEFT)
                            direction = input;
                        break;
                    case LEFT:
                        if (direction != RIGHT)
                            direction = input;
                        break;
                    }
                }
            }

        isAlive = moveSnake(&snake,&x, &y, direction, &hasFood, &snkln);
        } while (isAlive == 0);

        TXTINFO;
        if (isAlive == 1){
            BKGLOSE;
            cputsxy(WIDTH/2 -5,HEIGHT/2 +1,"You won C:");
            cputsxy(1,HEIGHT+3,"Press Enter to play again!");
        }
        else{
            BKGWIN;
            cputsxy(WIDTH/2 -5,HEIGHT/2 +1,"You lost :C");
            cputsxy(1,HEIGHT+3,"Press Enter to try again!");
        }

        while(kbhit()) getch();
        while(getch() != '\r');
    } while (1);

    return EXIT_SUCCESS;
}

void initSnake (coord **snake){
    int i;
    BKGSNAKE;
    for(i=2; i < INITLEN+2; i++ ){ //starts at 2 in order to not corrupt the wall
        coord *sp = malloc(sizeof(coord));
        sp->y = 2;
        sp->x = i;
        enqueue(snake,sp);
        putchxy(i,2,SNAKE);
    }
    BKGFIELD;
}

int snklen (coord *snake){
    register int i = 0;
    while (snake != NULL){
        snake = snake->next;
        i++;
    }
    return i;
}

int moveSnake (coord **snake, int *fx, int *fy, int direction, int *hasFood, int *snkln){
    static char info[30];
    sprintf(info,"x = %02d, y = %02d\nlen = %4d, ", (*snake)->x, (*snake)->y, *snkln);

    int sx = (*snake)->x, sy = (*snake)->y;

    switch(direction){
    case UP:
        strcat(info,"up   ");
        sy--;
        break;
    case DOWN:
        sy++;
        strcat(info,"down ");
        break;
    case RIGHT:
        sx++;
        strcat(info,"right");
        break;
    case LEFT:
        sx--;
        strcat(info,"left ");
        break;
    }
    cputsxy(1,HEIGHT+1,info);

    if(sx == 1 || sx == WIDTH || sy == 1 || sy == HEIGHT || hasCoordinate(*snake,sx,sy)){
        if(*snkln < MAXLEN){
            return -1;
        }
        else{
            return 1;
        }
    }// *** Beginning of preparation to start ***
    else if(sx == *fx && sy == *fy){
        *fx = -1;
        *fy = -1;
        *hasFood = 0;// *** Beginning of preparation to start ***
        (*snkln)++;

        coord *sp = malloc(sizeof(coord));
        sp->x = sx;
        sp->y = sy;
        enqueue(snake,sp);
    }
    else {
        coord *sp = dequeue(snake);
        BKGFIELD;
        putchxy(sp->x,sp->y,' ');
        sp->x = sx;
        sp->y = sy;
        enqueue(snake,sp);
    }

    BKGSNAKE;
    putchxy(sx,sy,SNAKE);
    BKGFIELD;

    return 0;
}

int hasCoordinate (coord *queue, int x, int y){
    while (queue != NULL){
        if (queue->x == x && queue->y == y){
            return 1;
        }
        queue = queue->next;
    }
    return 0;
}

void buildFrame (void){
    register int i;
    for(i=1; i<=WIDTH; i++){
        putchxy(i,1,WALL);
        putchxy(i,HEIGHT,WALL);
    }
    for(i=1; i<=HEIGHT; i++){
        putchxy(1,i,WALL);
        putchxy(WIDTH,i,WALL);
    }
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

void enqueue (coord **queue, coord *last){
    last->next = *queue;
    *queue=last;
}

coord *dequeue (coord **queue){
    coord *aux1 = *queue;
    coord *aux2 = aux1;
    if(aux1 != NULL){
        while(aux1->next != NULL){
            aux2 = aux1;
            aux1 = aux1->next;
        }
        aux2->next = NULL;
        if(*queue == aux1){
        *queue = NULL;
        }
        aux1->next = NULL;
    }
    return aux1;
}

void clearqueue(coord *queue){
    coord *c;
    while (queue != NULL){
        c = queue;
        queue = queue->next;
        free(c);
    }
}

/*cria uma matriz que representa o mapa e coloca 1 nas posições em que a cobra ocupa, se alguma posição for 0, o jogo não foi ganho*/
/*
int hasFreeSpace (coord *snake){
    int map[HEIGHT][WIDTH];

    register int i;
    register int j;

    for (i=2;i<HEIGHT;i++){
        for(j=2; j<WIDTH;j++)
            map[i][j] = 0;
    }

    while (snake != NULL){
        map[snake->y][snake->x] = 1;
        snake = snake->next;
    }

    for (i=2;i<HEIGHT;i++){
        for(j=2; j<WIDTH;j++){
            if(map[i][j] == 0)
                return 1;
        }
    }
    return 0;
} */

int hasFreeSpace(coord *snake){
    if(snklen(snake) < MAXLEN)
        return 1;
    else
        return 0;
}

