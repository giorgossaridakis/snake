/* ascii snake  game */
#include <windows.h>
#include <time.h>
#include <constrea.h>

/* numeric assignments */
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
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
  
constream win1;

/* local routines */  
void expand_snake(int quantity);
void set_fruit_random_coords(int number);
int check_coord_limits(int x, int y);
int check_rarity(int rarity);
void message(int flag, char message[]);
void randomizer(long int depth);
int mod (double a, double b);
void showCursor(bool showFlag);  

/* world variables */
int snake_size;
struct snake_prototype snake[MAXLENGTH];
struct fruit_prototype fruit[MAXFRUIT];
  
main()
{
  /* variables setup */
  int i;
  char c;

  int score=0;
  int crash=0;
  double level=1.0;
  int lives=INITIALLIVES;
  randomizer(9);
  
  struct points direction;
  struct points temp_points[MAXLENGTH];
  int game_timer=0;
  int sym;
  
  /* set up how many fruit will play and how*/  
  for (i=0;i<MAXFRUIT;i++) {
   fruit[i].appearance=0;
   fruit[i].flag=0; 
   fruit[i].life=0; }
   
	fruit[0].appearance=1;
	fruit[0].symbol=FRUIT;
	fruit[0].timer=100;
	fruit[0].color=4;
	fruit[0].rarity=0;
	fruit[0].score_add=1;
	fruit[0].size_add=1;
	fruit[0].life_add=0;
	fruit[1].appearance=1;
	fruit[1].symbol=BIGFRUIT;
	fruit[1].timer=50;
	fruit[1].color=4;
	fruit[1].rarity=100;
	fruit[1].score_add=10;
	fruit[1].size_add=5;
	fruit[1].life_add=0;
    fruit[2].appearance=1;
	fruit[2].symbol=POISON;
	fruit[2].timer=300;
	fruit[2].color=2;
	fruit[2].rarity=300;
	fruit[2].score_add=0;
	fruit[2].size_add=0;
	fruit[2].life_add=-1;
	fruit[3].appearance=1;
	fruit[3].symbol=EXPANDER;
	fruit[3].timer=25;
	fruit[3].color=7;
	fruit[3].rarity=3000;
	fruit[3].score_add=1;
	fruit[3].size_add=50;
	fruit[3].life_add=5;
	fruit[4].appearance=1;
	fruit[4].symbol=SUPERGIFT;
	fruit[4].timer=15;
	fruit[4].color=11;
	fruit[4].rarity=10000;
	fruit[4].score_add=0;
	fruit[4].size_add=0;
	fruit[4].life_add=50;
  
  while (lives>0) {
	  	  
	/* initialize parameters */	  
    snake_size=INITIALSIZE;
    direction.x=-1;
    direction.y=0;
    snake[0].pt.x=40;
    snake[0].pt.y=12;
    snake[0].symbol[0]=HEADX;
    snake[0].symbol[1]=HEADY;
    snake[0].direction=direction;
     for (i=1;i<MAXLENGTH;i++) {
      snake[i]=snake[i-1];
 	 snake[i].symbol[0]=BODY; 
	 snake[i].symbol[1]=BODY; }
     for (i=1;i<snake_size;i++) {
      snake[i].pt.x=snake[i-1].pt.x+1;
	  snake[i].direction=snake[i-1].direction; }
	  
    /* reset counter for size expansion */  
    game_timer=0; 

    /* draw the screen */
    win1.window(1, 1, 80, 25); 
    win1.clrscr();
	showCursor(false);
	win1 << setxy(7,1);
    win1 << setattr(GREEN)	<< "score:  "; 
	win1 << setxy(22,1); 
	win1 << setattr(BLUE) << "level:   ";
    win1 << setxy(37,1);
	win1 <<setattr(RED)<< "lives:";
    win1 << setxy(52,1);
    win1 <<setattr(YELLOW | YELLOW)	<< "size:";
	win1 << setxy(65,1);
    win1 <<setattr(BLUE<<4 | YELLOW)	<< "C++ ASCII Snake";
	win1 << setxy(1,24);
	win1 << setattr(GREEN)	<< "  | arrow keys to move | + to add size | * level up | / level down | p pause |";
 
    for (i=1;i<80;i++) {
     win1 << setxy(i,2);
      win1 << setattr(CYAN | CYAN) << BORDER; 
 	win1 << setxy(i,23);
      win1 << setattr(CYAN | CYAN) << BORDER;  }
    for (i=2;i<23;i++) {
     win1 << setxy(1, i);  
	 win1 << setattr(CYAN | CYAN) << BORDER; 
 	win1 << setxy(79,i);
	 win1 << setattr(CYAN | CYAN) << BORDER;  }
	 
	/* main routine */
	 while (c!=27 && !crash) {
	  
	  win1 << setxy(13,1) << "  "; 
	  win1 << setxy(13,1);
  	  win1 <<setattr(GREEN) << score; 
	  win1 << setxy(29,1)<< "   ";
	  win1 << setxy(29,1);
	  win1 <<setattr(BLUE) << level;
	  win1 << setxy(43,1);
	  win1 <<setattr(RED) << lives;
      win1 << setxy(57,1);
      win1 <<setattr(YELLOW | YELLOW) << snake_size;
	
  	 /* add to timer of fruit and extinguish if limit has been reached */
	 i=0;
	 while (fruit[i].appearance) {
	  if (fruit[i].flag) 
	   ++fruit[i].life;
      if (fruit[i].life==fruit[i].timer) {
	   fruit[i].life=0;
       fruit[i].flag=0;
       win1 << setxy (fruit[i].pt.x, fruit[i].pt.y) << ' '; }	 
      ++i; }	   
	 
	 /* arrange for fruit coordinates */
	 i=0;
	 while (fruit[i].appearance) {		 
	  if (!fruit[i].flag && check_rarity(fruit[i].rarity)) 
	   set_fruit_random_coords(i);  
      ++i; } 
 
	/* keyboard controls */
	  if(kbhit()) 
	   c=getch();
   
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
		case '*':
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
	   
 	 snake[0].direction=direction;
	 if (!direction.y)
	  sym=0;
     else
	  sym=1;
	   
     /* parts&fruit management */	
	 
	 /* erase previous parts */	
      for (i=0;i<snake_size;i++) { 
	   if (!(check_coord_limits(snake[i].pt.x, snake[i].pt.y))) {
        win1 << setxy(snake[i].pt.x, snake[i].pt.y);
	    win1 << setattr(BLACK) << ' ';  }	}    
 
	 /* show snake parts */  
	  for (i=0;i<snake_size;i++) {
	   snake[i].pt.x+=snake[i].direction.x;
	   snake[i].pt.y+=snake[i].direction.y;
	   if (!(check_coord_limits(snake[i].pt.x, snake[i].pt.y))) {
        win1 << setxy(snake[i].pt.x, snake[i].pt.y);
	    win1 << setattr(MAGENTA) << snake[i].symbol[sym]; } }
	   
	  /* show fruit section */
	  i=0;
	  while (fruit[i].appearance) {
	   if (fruit[i].flag) {
	    win1 << setxy (fruit[i].pt.x, fruit[i].pt.y);
	    win1 << setattr(fruit[i].color) << fruit[i].symbol; }
	   ++i; }
	  
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
	  if (level>9.9)
	   break; 
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
	  win1 << setxy(35,12);
	  win1 << setattr (YELLOW<<4 | RED) << "Crash!!!";
	  win1 << setattr (BLACK | BLACK);
	  Sleep(1000); 
	  }  
	 }	    
	  win1 << setxy(35,12);
	  if (level>=9.9)
	   message(1, "Snake is retired with high pension!");
      else
       message(1, "Snake no more :-(");
	 
  showCursor(true);	 

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
void message(int flag, char message[])
{
  int i;	
	
   if (flag) {
    win1 << setxy(35,12);	
    win1 <<setattr(YELLOW<<4 | RED) << message; }
   else { 
    win1 << setxy(35,12);	
    for (i=0;i<strlen(message);i++)
     win1 << setattr(BLACK | BLACK) << ' '; }
}
   

/* randomize numbers better */
void randomizer(long int depth)
{
  time_t timer;
  int seed_number;
  int i,n,l;

  randomize();

  time(&timer);
  seed_number=mod(timer,10000);
 
   for (n=0;n<seed_number;n++) {

    for (i=0;i<depth;i++)
     l=(rand () % 1111);

  }
}

/* remaining from division */
int mod (double a, double b)
{
int tmp =a/b;

return a-(b*tmp);
}

/* show / hide cursor*/
void showCursor(bool showFlag)
{   //enable/disable cursor visibility
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
