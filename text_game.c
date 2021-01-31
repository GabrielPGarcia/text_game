
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
int Health;
int Damage;
int level = 1;



void Wepon_Selection()
{
  ppu_off();
  vram_adr(NAMETABLE_A);
  vram_fill(0,1024);
  // I AM THE GREATEST
  vram_adr(NTADR_A(5,10));
  vram_write("weapons", 7);

  ppu_on_all(); 

}
void Monster_Maker()
{
  M_Name = (rand()%11) + 1;
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
  
  vram_adr(NTADR_A(10,2));
  vram_write(MonsterL, 1);
  
  vram_adr(NTADR_A(4,6));
  vram_write("Health:", 7);

  vram_adr(NTADR_A(11,6));
  vram_write(MonsterH, 3);
  
  vram_adr(NTADR_A(4,8));
  vram_write("Damage:", 7);

  vram_adr(NTADR_A(11,8));
  vram_write(MonsterD, 3);

  //ppu_on_all();
}
void Player()
{
  char MonsterH[3];
  char MonsterL[1];  
  char MonsterD[3];
  int num = (rand()%11) + 1;
  int num1 = (rand()%3+1);
  M_level = (rand()%10);
  M_Damage = (num1+1) * (M_level + 1) * 10;
  if (M_level < 9)
    M_Health = 100 * (M_level + 1);
  else
    M_Health = 999;
  sprintf(MonsterH,"%d",M_Health);
  sprintf(MonsterL,"%d",M_level);
  sprintf(MonsterD,"%d",M_Damage);


  vram_adr(NTADR_A(15,16));
  vram_write("Player", 6);
  
  vram_adr(NTADR_A(15,18));
  vram_write("Level:", 6);
  
  vram_adr(NTADR_A(21,18));
  vram_write(MonsterL, 1);
  
  vram_adr(NTADR_A(4,20));
  vram_write("Health:", 7);

  vram_adr(NTADR_A(12,20));
  vram_write(MonsterH, 3);
  
  vram_adr(NTADR_A(4,22));
  vram_write("Damage:", 7);

  vram_adr(NTADR_A(12,22));
  vram_write(MonsterD, 3);
    
  vram_adr(NTADR_A(22,20));
  vram_write("Attack", 6);
  
  vram_adr(NTADR_A(22,22));
  vram_write("Run", 3);
  
  vram_adr(NTADR_A(26,22));
  vram_write(MonsterD, 1);

      
}
void Battal()
{
   int M_Roll = (rand()%6);
   int P_Roll = (rand()%6);
   if(M_Roll > P_Roll)
   {
     vram_adr(NTADR_A(9,12)); 
     vram_write("You Lost the Roll!", 18);
   }
   else if(M_Roll < P_Roll)
   {
     vram_adr(NTADR_A(9,12)); 
     vram_write("You Won the Roll!", 17);
   }
   else
   {
     vram_adr(NTADR_A(12,12)); 
     vram_write("Tie!", 3);
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

void gamestate()
{
    char pad1;
  
    int state = 0;
    while(1) 
    {
      pad1 = pad_trigger(0);
    if(pad1 & PAD_START)
    {
      Monster_Maker();
      state =  state + 1;
    }
    else if(pad1 & state == 1)
    {
        ppu_off();
  	Monster_Output();
  	Player();
        Battal();
  	ppu_on_all();
      state = state - 1;
    }
  }
}
void main(void)
{
   
  int state = 0;
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white

 

  ppu_off();
  Monster_Maker();
  Monster_Output();
  Player();
  Battal();
  ppu_on_all();
  // infinite loop
  while(1) 
  {
    gamestate();
  }
}

