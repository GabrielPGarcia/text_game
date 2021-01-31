
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// BCD arithmetic support
#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

//---------Global Monster--------------
const char Monster[11][10]=
{
  "Rat","Lizardfolk","Orc","Plant","Slime",
  "Undead","Bat","Boar","Wolf","Imp","Dragon"
};
const char MonsterType[3][10]=
{
  "          ","Giant    ","Elemental"
 };
//-----------Global V------------
char gPad0;
char gPad1;
char gPad2;
char gPad3;
char gPad4;
int points;
//---------Global Monster Stats--------------
int M_Name;
int M_Type;
int M_Health;
int M_Damage;
int M_level;
char MonsterH[3];
char MonsterL[1];  
char MonsterD[3];
//---------Global Player Stats--------------
int P_Health;
int P_Damage;
int P_level;
int P_Run;
char PlayerH[3];
char PlayerL[1];
char PlayerD[3];
char PlayerR[1];
int attack;

void Monster_Maker()
{
  M_Name = (rand()%11);
  M_Type = (rand()%3);
  
  M_level = (rand()%10);
  
  M_Damage = 10;
  if (M_level < 9)
    M_Health = 100 * (M_level + 1);
  else
    M_Health = 999;
  sprintf(MonsterH,"%d",M_Health);
  sprintf(MonsterL,"%d",M_level);
  sprintf(MonsterD,"%d",M_Damage);  
}
void Player_Maker()
{  
  P_Damage = 50;
  P_Health = 100;
  P_level = 1;
  P_Run = 3;
  points = 0;
  sprintf(PlayerH,"%d",P_Health);
  sprintf(PlayerL,"%d",P_level);
  sprintf(PlayerD,"%d",P_Damage); 
  sprintf(PlayerR,"%d",P_Run);  
}
void Monster_Output()
{
  //ppu_off();
  
  vram_adr(NAMETABLE_A);
  vram_fill(0,1024);
  // I AM THE GREATEST
  
  vram_adr(NTADR_A(4,4));
  vram_write(*(MonsterType + M_Type), 10);
  
  vram_adr(NTADR_A(14,4));
  vram_write(*(Monster + M_Name), 10);
  
  vram_adr(NTADR_A(4,2));
  vram_write("Level:", 6);
  
  vram_adr(NTADR_A(4,6));
  vram_write("Health:", 7);
  
  vram_adr(NTADR_A(4,8));
  vram_write("Damage:", 7);
  

  vram_adr(NTADR_A(11,8));
  vram_write(MonsterD, 3);
  
  vram_adr(NTADR_A(10,2));
  vram_write(MonsterL, 1);
  
  vram_adr(NTADR_A(11,6));
  vram_write(MonsterH, 3);

  //ppu_on_all();
}
void Player()
{

  vram_adr(NTADR_A(15,16));
  vram_write("Player", 6);
  
  vram_adr(NTADR_A(15,18));
  vram_write("Level:", 6);
  
  vram_adr(NTADR_A(21,18));
  vram_write(PlayerL, 1);
  
  vram_adr(NTADR_A(4,20));
  vram_write("Health:", 7);

  vram_adr(NTADR_A(12,20));
  vram_write(PlayerH, 3);
  
  vram_adr(NTADR_A(4,22));
  vram_write("Damage:", 7);

  vram_adr(NTADR_A(12,22));
  vram_write(PlayerD, 3);
    
  vram_adr(NTADR_A(22,20));
  vram_write("Attack", 6);
  
  vram_adr(NTADR_A(22,22));
  vram_write("Run", 3);
  
  vram_adr(NTADR_A(26,22));
  vram_write(PlayerR, 1);

      
}
void Battal()
{
   int M_Roll = (rand()%6) + (M_level - 3);
   int P_Roll = (rand()%6) + P_level;
   if(M_Roll > P_Roll)
   {
     attack = -1;
   }
   else if(M_Roll < P_Roll)
   {
     attack = 1;
   }
   else
   {
     attack = 0;
   }
}
void Game_World()
{
  ppu_off();
  vram_adr(NAMETABLE_A);
  vram_fill(0,1024);
  // I AM THE GREATEST
  vram_adr(NTADR_A(5,10));
  vram_write("weapons", 7);

  ppu_on_all(); 
}



void gamestate1()
{
  int k;
  byte joy = pad_poll(0);
  // move left/right?
  if(M_Health <= 0)
  {   
    ppu_off();
    points = points + 1;
    P_Health = 100;
    if(points <= 2)
      P_level = P_level;
    else if (points <= 4)
      P_level = 2;
    else if (points <= 8)
      P_level = 3;
    else if (points <= 16)
      P_level = 4;
    else if (points <= 32)
      P_level = 5;
    else if (points <= 64)
      P_level = 6;
    else if (points <= 128)
      P_level = 7;
    else if (points <= 256)
      P_level = 8;
    else if (points <= 512)
      P_level = 9;
    P_Damage = P_Damage;
    if(P_Run <3)
      P_Run = P_Run + 1;
    
    sprintf(PlayerH,"%d",P_Health);
    sprintf(PlayerL,"%d",P_level);
    sprintf(PlayerD,"%d",P_Damage);
    sprintf(PlayerR,"%d",P_Run);      
    Monster_Maker();
    Monster_Output();
    Player();    
    ppu_on_all();
  }
  else if(k == 5 & attack == 1) 
  {
    ppu_off();
    
    M_Health = (M_Health - P_Damage);
    
    M_level = M_level;
    M_Damage = M_Damage;
    sprintf(MonsterH,"%d",M_Health);
    sprintf(MonsterL,"%d",M_level);
    sprintf(MonsterD,"%d",M_Damage);
    Monster_Output();
    Player();    
    ppu_on_all(); 
    k = 0;
  }
  else if(k == 5 & attack == -1) 
  {
    ppu_off();
    
    P_Health = (P_Health - M_Damage);
    
    P_level = P_level;
    P_Damage = P_Damage;
    sprintf(PlayerH,"%d",P_Health);
    sprintf(PlayerL,"%d",P_level);
    sprintf(PlayerD,"%d",P_Damage);
    Monster_Output();
    Player();
    
    ppu_on_all(); 
    k = 0;
  }
  if (joy & PAD_LEFT) {k=5;attack = 1;}
        	
  if (joy & PAD_RIGHT) {};  
  if (joy & PAD_UP) ;
  if (joy & PAD_DOWN) ;
  // shoot missile?
  if (joy & PAD_A)
    if( joy & PAD_START)
      if (P_Run > 0)
      {
        ppu_off();
        Monster_Maker();
        Monster_Output();
        P_Health = 100;
        P_Damage = P_Damage;
        P_level = P_level;
        P_Run = P_Run - 1;
        sprintf(PlayerH,"%d",P_Health);
        sprintf(PlayerL,"%d",P_level);
        sprintf(PlayerD,"%d",P_Damage);
        sprintf(PlayerR,"%d",P_Run);
        Player();
        ppu_on_all();
      }

}


void Start()
{
  ppu_off();
  vram_adr(NTADR_A(8,10));
  vram_write("Survive Luck Island", 19);
  
  vram_adr(NTADR_A(8,18));
  vram_write("[Space] to start!", 17);
  ppu_on_all();
}

void Screen()
{
  ppu_off();
  Monster_Maker();
  Monster_Output();
  Player_Maker();
  Player();
  ppu_on_all();
}
void main(void)
{
  char pad; 
  int inum = 0;
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white

 
  while(1) 
  {
    pad = pad_trigger(0);
    if(inum == 0)
    {Start();inum=1;}
    if(pad & inum == 1)
    {Screen();inum = 2;}
    // infinite loop
    else if(inum == 2)
      gamestate1();
  }
}

