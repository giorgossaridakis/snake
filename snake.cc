/* ascii snake  game */
#include <unistd.h>
#include <iostream>
#include <conio.h>
#include <termios.h>

/* numeric assignments */
#define UP 'q'
#define LEFT 'n'
#define RIGHT 'm'
#define DOWN 'a'
#define ASTERISK 42
#define MAXLENGTH 999
#define MAXFRUIT 20
#define INITIALSIZE 5
#define INITIALLIVES 5

/* character definitions */
#define BORDER 'X'
#define HEADX '~'
#define HEADY '|'
#define BODY '@'
#define FRUIT 'o'
#define BIGFRUIT 'O'
#define POISON '%'
#define EXPANDER '$'
#define SUPERGIFT '@'

/* structures of snake and fruit */
struct points {
  int x;
  int y; } ;
  
struct snake_prototype {
  struct points pt; 
  struct points direction;
  char symbol[2]; } ; /* symbol 0 for x movement, 1 for y movement */
  
struct fruit_prototype {
   char symbol;
   int color;
   int timer;
   int life;
   struct points pt;
   unsigned int appearance : 1;
   unsigned int flag : 1;
   int rarity; 
   int score_add; 
   int size_add;
   int life_add;   } ;   
  
WINDOW *win1=newwin(80, 25, 0, 0);

/* local routines */  
void initialize_variables(); 
void draw_screen();
void expand_snake(int quantity);
void set_fruit_random_coords(int number);
int check_coord_limits(int x, int y);
int check_rarity(int rarity);
void message(int flag, const char message[]);
void Sleep(int sleepMs);
void cleanstdin();

/* world variables */
int snake_size;
struct snake_prototype snake[MAXLENGTH];
struct fruit_prototype fruit[MAXFRUIT];
  
int main()
{ 
  /* variables setup */
  int i;
  char c;

  int score=0;
  int crash=0;
  double level=1.0;
  int lives=INITIALLIVES;
  int kbrd_flag;

  /*    COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7 */
  
  win1=initscr();
  if (has_colors() == FALSE) {
   endwin();
   printf("Your terminal does not support color\n");
  exit(1); }
  
  curs_set(0);
  start_color();			/* Start color 			*/
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_RED, COLOR_YELLOW);
  init_pair(8, COLOR_BLUE, COLOR_YELLOW);
  init_pair(9, COLOR_BLACK, COLOR_BLACK);
  attron(COLOR_PAIR(0));
  srand(time(NULL));
  
  struct points direction;
  struct points temp_points[MAXLENGTH];
  int game_timer=0;
  int sym;
  
  initialize_variables(); 
  
  while (lives>0) {
	  	  
	/* initialize parameters */	  
    snake_size=INITIALSIZE;
    direction.x=-1;
    direction.y=0;
    snake[0].direction=direction;
    snake[0].pt.x=40;
    snake[0].pt.y=12;
  
     for (i=1;i<snake_size;i++) {
      snake[i].pt.x=snake[i-1].pt.x+1;
	  snake[i].pt.y=snake[i-1].pt.y;
     snake[i].direction=snake[i-1].direction; }
	  
    /* reset counter for size expansion */  
    game_timer=0; 

    draw_screen();
	 
	/* main routine */
	 while (c!=27 && !crash) {
	  
      gotoxy(13,1); printw("  "); 
      gotoxy(13,1);
  	  attron(COLOR_PAIR(2)); printw("%d", score); 
      gotoxy(29,1); printw("   ");
      gotoxy(29,1);
      attron(COLOR_PAIR(4)); printw("%0.1f", level);
	  gotoxy(43,1);
	  attron(COLOR_PAIR(1)); printw("%d", lives);
      gotoxy(57,1);
      attron(COLOR_PAIR(3)); printw("%d",  snake_size);
      
      refresh();
	
  	 /* add to timer of fruit and extinguish if limit has been reached */
	 i=0;
	 while (fruit[i].appearance) {
	  if (fruit[i].flag) 
	   ++fruit[i].life;
      if (fruit[i].life==fruit[i].timer) {
	   fruit[i].life=0;
       fruit[i].flag=0;
      gotoxy (fruit[i].pt.x, fruit[i].pt.y); addch(' '); }	 
      ++i; }	   
	 
	 /* arrange for fruit coordinates */
	 i=0;
	 while (fruit[i].appearance) {		 
	  if (!fruit[i].flag && check_rarity(fruit[i].rarity)) 
	   set_fruit_random_coords(i);  
      ++i; } 
 
	/* keyboard controls */
    if(kbhit()) {
	 c=getch();
    kbrd_flag=1; }
    // transform arrow key input
     if (c==254 && kbrd_flag)
      c=0;
     if (kbrd_flag && c==0) {
      switch (getch()) {
       case 53:
        c=UP;
       break;
       case 52:
        c=DOWN;
       break;
       case 55:
        c=RIGHT;
       break;
       case 54:
        c=LEFT;
      break; } }
      kbrd_flag=0;
      switch (c) {
	   case UP:
	    if (direction.y!=1) {
	     direction.x=0;
	 	 direction.y=-1; }
	   break;
	   case DOWN:
	    if (direction.y!=-1) {
	     direction.x=0;
		 direction.y=1; }
	   break;
	   case LEFT:
	    if (direction.x!=1) {
	     direction.x=-1;
		 direction.y=0; }
	   break;
	   case RIGHT:
	    if (direction.x!=-1) {
	     direction.x=1;
		 direction.y=0; }
	   break; 
	    case '+':
		 c=0;
		 expand_snake(1);	
		break; 
		case ASTERISK:
		 level+=0.1;
		break ;
        case '/':
		 level-=0.1;
		break;
		case 'p':
		 message(1, "paused");
		 c=getch();
		 message(0, "paused");
		 c=0;
		break;
		case 27:
		 lives=0;
        break; } 
        
	 cleanstdin();  
 	 snake[0].direction=direction;
	 if (!direction.y)
	  sym=0;
     else
	  sym=1;
	   
     /* parts&fruit management */	
	 
	 /* erase previous parts */	
      for (i=0;i<snake_size;i++) { 
	   if (!(check_coord_limits(snake[i].pt.x, snake[i].pt.y))) {
        gotoxy(snake[i].pt.x, snake[i].pt.y);
        attron(COLOR_PAIR(9));
      addch(32);  }	}    
 
	 /* show snake parts */  
	  for (i=0;i<snake_size;i++) {
	   snake[i].pt.x+=snake[i].direction.x;
	   snake[i].pt.y+=snake[i].direction.y;
	   if (!(check_coord_limits(snake[i].pt.x, snake[i].pt.y))) {
        gotoxy(snake[i].pt.x, snake[i].pt.y);
        attron(COLOR_PAIR(5));
      addch(snake[i].symbol[sym]); } }
	   
	  /* show fruit section */
	  i=0;
	  while (fruit[i].appearance) {
	   if (fruit[i].flag) {
	    gotoxy (fruit[i].pt.x, fruit[i].pt.y);
       attron(COLOR_PAIR(fruit[i].color)); addch(fruit[i].symbol);   }
	   ++i; }
	   
     refresh();
	  
      /* inherit direction properties for next part snake[i] */	  
	  for (i=0;i<snake_size;i++) 
       temp_points[i]=snake[i].direction;	
      for (i=1;i<snake_size;i++) 
       snake[i].direction=temp_points[i-1];	
   
     /* check for collisions */
	 if (snake[0].pt.x<2 && snake[0].direction.x==-1) 
      ++crash;		 
	 if (snake[0].pt.x>78 && snake[0].direction.x==1)
      ++crash;
	 if (snake[0].pt.y<3 && snake[0].direction.y==-1)
      ++crash;
     if (snake[0].pt.y>22 && snake[0].direction.y==1)
	  ++crash;
     for (i=1;i<snake_size;i++) {
	  if (snake[0].pt.x==snake[i].pt.x && snake[0].pt.y==snake[i].pt.y)
	   ++crash; }  
   
     /* check for fruit contact */
	 i=0;
	 while (fruit[i].appearance) {
	  if (fruit[i].flag && snake[0].pt.x==fruit[i].pt.x && snake[0].pt.y==fruit[i].pt.y) {
	   score+=fruit[i].score_add;
	   expand_snake(fruit[i].size_add);
	   lives+=fruit[i].life_add;
	   fruit[i].life=0;
	   fruit[i].flag=0; }
	  ++i; }
     
	 /* success calculator */ 
	  if (game_timer>100/level) {
	   game_timer=0;
	   c='+'; }	  
	  if (level>9.9) {
	   lives=0;
	   break; } 
      if (score>10) {
	   score-=10;
	   level+=0.1; }
	   
	  /* prepare for loop */
      Sleep(1000-(level*100));
	  ++game_timer;
	  fflush(stdin);
	   
	  } 
	 
	 /* check for collision incidence */
	 if (crash)	{
	   crash=0;
	   c=0;
	   --lives;
	   gotoxy(35,12);
	   attron(COLOR_PAIR(8)); printw("Crash!!!");
	   refresh();
	  Sleep(1000); 
	  }  
	 }	    
	  if (level>=9.9)
	   message(1, "Snake is retired with high pension!");
      else
       message(1, "Snake no more :-(");
	Sleep(2000); 
    delwin(win1);
    endwin();
    curs_set(1);
    refresh();
    
  return 0;
}

/* set up how many fruit will play and how and assign snake body parts */  
void initialize_variables()
{
   int i;
	
	/* first set up default flags for all fruit */
    for (i=0;i<MAXFRUIT;i++) {
    fruit[i].appearance=0;
    fruit[i].flag=0; 
    fruit[i].life=0; }
   
     /* each .appearance is a flag, if it's set to 0, fruit will never appear */
	fruit[0].appearance=1;
	fruit[0].symbol=FRUIT; /* symbol is obvious */
	fruit[0].timer=100; /* how many rounds it will stay until it is reevaluated */
	fruit[0].color=1; /* obvious 1-7 */
	fruit[0].rarity=0; /* rarity set to 0 means it will appear immediately after extinction */
	fruit[0].score_add=1; /* how much it adds to the score */
	fruit[0].size_add=1; /* to size */
	fruit[0].life_add=0; /* to lives */
	fruit[1].appearance=1;
	fruit[1].symbol=BIGFRUIT;
	fruit[1].timer=50;
	fruit[1].color=2;
	fruit[1].rarity=100;
	fruit[1].score_add=10;
	fruit[1].size_add=5;
	fruit[1].life_add=0;
    fruit[2].appearance=1;
	fruit[2].symbol=POISON;
	fruit[2].timer=300;
	fruit[2].color=4;
	fruit[2].rarity=300;
	fruit[2].score_add=0;
	fruit[2].size_add=0;
	fruit[2].life_add=-1;
	fruit[3].appearance=1;
	fruit[3].symbol=EXPANDER;
	fruit[3].timer=25;
	fruit[3].color=5;
	fruit[3].rarity=3000;
	fruit[3].score_add=1;
	fruit[3].size_add=50;
	fruit[3].life_add=5;
	fruit[4].appearance=1;
	fruit[4].symbol=SUPERGIFT;
	fruit[4].timer=15;
	fruit[4].color=6;
	fruit[4].rarity=10000;
	fruit[4].score_add=0;
	fruit[4].size_add=0;
	fruit[4].life_add=50;
	
	/* lets arrange the snake parts */
     for (i=1;i<MAXLENGTH;i++) {
 	  snake[i].symbol[0]=BODY; 
	  snake[i].symbol[1]=BODY; }
	 /* set up snake head with different character */
	 snake[0].symbol[0]=HEADX;
     snake[0].symbol[1]=HEADY;
}

/* draw the screen */
void draw_screen()
{
   int i;	
 
    clear();
    gotoxy(7,1);
    attron(COLOR_PAIR(2)); printw("score:  "); 
    gotoxy(22,1); 
    attron(COLOR_PAIR(4)); printw("level:   ");
    gotoxy(37,1);
    attron(COLOR_PAIR(1)); printw("lives:");
    gotoxy(52,1);
    attron(COLOR_PAIR(3)); printw("size:");
    gotoxy(65,1);
    attron(COLOR_PAIR(8)); printw("C++ ASCII Snake");
    gotoxy(1,24);
    attron(COLOR_PAIR(2)); printw("  | arrow keys to move | + to add size | * level up | / level down | p pause |");
 
    for (i=1;i<80;i++) {
     gotoxy(i,2);
     attron(COLOR_PAIR(6)); addch(BORDER); 
 	 gotoxy(i,23);
    attron(COLOR_PAIR(6)); addch(BORDER);;  }
    for (i=2;i<23;i++) {
     gotoxy(1, i);  
	 attron(COLOR_PAIR(6)); addch(BORDER); 
 	 gotoxy(79,i);
	attron(COLOR_PAIR(6)); addch(BORDER);;  }
}

/* set up fruit in board and reset flag to appearance */
void set_fruit_random_coords(int number)
{
  fruit[number].pt.x=rand() % 75 + 2 ;
  fruit[number].pt.y=rand() % 20 + 3 ;
  fruit[number].flag=1;
}

/* grow by integer */
void expand_snake(int quantity)
{
  int i;
  
   for (i=0;i<quantity;i++) {
	snake_size++;
	snake[snake_size-1]=snake[snake_size-2];
	snake[snake_size-1].pt.x-=snake[snake_size-2].direction.x;
	snake[snake_size-1].pt.y-=snake[snake_size-2].direction.y;	}
}	

/* check if coordinates of object are out of range for added snake parts*/
int check_coord_limits(int x, int y)
{
  if (x<2 || x>78)
   return -1;
  if (y<3 || y>22) 
   return -1;
 
 return 0;
}

/* return 1 if object is lucky enough to appear */
int check_rarity(int rarity)
{
  int i,n,l;
  int success[3];
  
   if (!rarity)
	return 1;

   else
	i=rand()% rarity;
    n=int(rarity/3);
    
    for (l=1;l<4;l++) 
     success[l-1]=n*l;
	for (l=0;l<3;l++) {
	 if (i==success[l])
	  return 1; }
 
  return 0;
}

/* show message on top left of border window, flag 1 to show, 0 to recover border */
void message(int flag, const char message[])
{
   if (flag) {
    gotoxy(35,12);	
    attron(COLOR_PAIR(7)); printw("%s", message); }
   else { 
    gotoxy(35,12);	
    attron(COLOR_PAIR(9)); printw("%s", message); }
   refresh();
}

// Sleep for miliseconds
void Sleep(int sleepMs)
{
  usleep(sleepMs*1000);
}

/* remove garbage from stdin */
void cleanstdin()
{
 int stdin_copy = dup(STDIN_FILENO);

  tcdrain(stdin_copy);
  tcflush(stdin_copy, TCIFLUSH);
 close(stdin_copy);
}
