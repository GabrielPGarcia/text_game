
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

//---------Global Monster Name/Type--------------
const char Monster[11][10]=
{
  "Rat","Lizardfolk","Orc","Plant","Slime",
  "Undead","Bat","Boar","Wolf","Imp","Dragon"
};
const char MonsterType[3][10]=
{
  "          ","Giant    ","Elemental"
};
//-----------Global Variables------------
byte joy;
int inum;
int iDnum;
int iCount;
int attack;
char pad;
//---------Global Monster Stats--------------
int M_Name;
int M_Type;
int M_Health;
int M_Damage;
int M_level;
char MonsterL[1]; 
char MonsterH[3]; 
char MonsterD[3];
//---------Global Player Stats--------------
int P_level;
int P_Health;
int P_Damage;
int P_Run;
int P_Points;
char PlayerL[1];
char PlayerH[3];
char PlayerD[3];
char PlayerR[1];
char PlayerP[4];
//-------------------Set Monster----------
void Monster_Maker()
{
  int iRand = (rand()%10);	//pick random level 
  M_Type = (rand()%3);		//pick random type   
  M_Name = (rand()%11);		//pick random name
  
  if(iRand > 5 & P_level <= 5)
    M_level = (iRand - 3);
  else
    M_level = iRand ;
  
  M_Health = 100 * (iRand + M_Type);
  M_Damage = 12 + (M_level + iRand * 10 );
  //--------------------set int to string----
  sprintf(MonsterL,"%d",M_level);
  sprintf(MonsterH,"%d",M_Health); 
  sprintf(MonsterD,"%d",M_Damage); 
}
//-----------------Set Player--------------
void Player_Maker()
{  
  P_level = 1;			//pick random level
  P_Damage = 50;		//pick random Damage
  P_Health = 100;		//pick random Health
  P_Run = 3;			//pick random Run
  P_Points = 0;			//pick random Points
  //--------------------set int to string----
  sprintf(PlayerL,"%d",P_level);
  sprintf(PlayerH,"%d",P_Health); 
  sprintf(PlayerR,"%d",P_Run); 
  sprintf(PlayerD,"%d",P_Damage); 
}
//-----------------Set Player--------------
void Player_LevelUp()
{  
  P_level = P_level;			//pick random level
  P_Damage = 50 * P_level;		//pick random Damage
  P_Health = 100 * P_level + P_Points;	//pick random Health
  P_Run = P_Run;			//pick random Run
  P_Points = P_Points;			//pick random Points
  //--------------------set int to string----
  sprintf(PlayerL,"%d",P_level);
  sprintf(PlayerH,"%d",P_Health);
  sprintf(PlayerD,"%d",P_Damage); 
  sprintf(PlayerR,"%d",P_Run);  
}
//-----------------Prints out Monster--------------
void Monster_Output()
{  
  vram_adr(NAMETABLE_A);
  vram_fill(0,1024);
  
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
}
//-----------------Prints out Player--------------
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
//-----------------clearScreen--------------
void clearScreen()
{  
  vram_adr(NAMETABLE_A);
  vram_fill(0,1024);
}

//-----------------Title Screen--------------
void vTitleScreen()
{
  ppu_off();
  vram_adr(NTADR_A(8,10));
  vram_write("Survive Luck Island", 19);
  
  vram_adr(NTADR_A(8,18));
  vram_write("[Space] to start!", 17);
  ppu_on_all();
}
//-----------------End Screen--------------
void vEndScreen()
{
  sprintf(PlayerP,"%d",P_Points);
  vram_adr(NTADR_A(8,12));
  vram_write("Your Points:", 12);
  
  vram_adr(NTADR_A(20,12));
  vram_write(PlayerP, 4);
}
//-----------------Roll Dice--------------
void vBattle()
{
  int M_Roll;
  int P_Roll;
  if(P_level > 5)
  {
    M_Roll = (rand()%6) + M_level;	//random roll for Monster
    P_Roll = (rand()%6) + P_level;	//random roll for Player
  }  
  else
  {
    M_Roll = (rand()%6);		//random roll for Monster
    P_Roll = (rand()%6) + P_level;	//random roll for Player
  }
  
  if(M_Roll > P_Roll)	//if Monster Wins
  {
    attack = -1;
  }
  else if(M_Roll < P_Roll)//if Player Wins
  {
    attack = 1;    
  }
  else			//if Tie
  {
    attack = 3;
  }
}
//-----------------Build--------------
void vBuild()
{
  ppu_off();
  Player_Maker();
  Monster_Maker();
  Monster_Output();
  Player();
  ppu_on_all();
}
//-----------------Game State--------------
void vGameState()
{
  joy = pad_poll(0);
  //------------------check if Monster is dead & add point if so---
  if(M_Health <= 0)
  {   
    P_Points = P_Points + 1; //add points
    if(P_Run != 9)
      P_Run = P_Run + 1; // add points to run

    if(P_Points <= 1)	     //lvl up requirements   
      P_level = P_level;
    else if (P_Points <= 2)
      P_level = 2;
    else if (P_Points <= 4)
      P_level = 3;
    else if (P_Points <= 8)
      P_level = 4;
    else if (P_Points <= 16)
      P_level = 5;
    else if (P_Points <= 32)
      P_level = 6;
    else if (P_Points <= 64)
      P_level = 7;
    else if (P_Points <= 128)
      P_level = 8;
    else
      P_level = 9;    
    
    //--------------------update all----
    ppu_off();
    Player_LevelUp();
    Monster_Maker();
    Monster_Output();
    Player();    
    ppu_on_all();
  }
  //----------------------End Game---
  else if(P_Health <= 0)
  {
    ppu_off();
    clearScreen();
    vEndScreen();
    ppu_on_all();
    inum = 1;
  }

  if(iDnum == 0 & attack == 1) 
  {
    M_Health = M_Health - P_Damage;
    M_level = M_level; 
    M_Damage = M_Damage;
    
    sprintf(MonsterL,"%d",M_level);
    sprintf(MonsterH,"%d",M_Health);
    sprintf(MonsterD,"%d",M_Damage);
    
    ppu_off(); 
    
    Monster_Output(); 
    Player();  
    vram_adr(NTADR_A(8,12));
    vram_write("                ", 16);
    vram_adr(NTADR_A(8,12));
    vram_write("You land a hit!", 16);
    ppu_on_all();
    
    attack = 0;
    iDnum = 0;
  }
  else if(iDnum == 0 & attack == -1) 
  {
    P_Health = (P_Health - M_Damage);
    P_level = P_level; 
    P_Damage = P_Damage;
    
    sprintf(PlayerL,"%d",P_level);
    sprintf(PlayerH,"%d",P_Health);
    sprintf(PlayerD,"%d",P_Damage); 
    sprintf(PlayerR,"%d",P_Run);  
    
    ppu_off();
    Monster_Output();
    Player();    
    
    vram_adr(NTADR_A(8,12));
    vram_write("                ", 16);
    vram_adr(NTADR_A(8,12));
    vram_write("You got hit!", 13);
    ppu_on_all();
    
    attack = 0;
    iDnum = 0;
  }
  else if(iDnum == 0 & attack == 3)
  {
    ppu_off();
    vram_adr(NTADR_A(8,12));
    vram_write("                ", 16);
    vram_adr(NTADR_A(15,12));
    vram_write("Tie!", 4);
    ppu_on_all();
    attack = 0;
  }
  if(iDnum == 0 & iCount == 0)
  {  
    ppu_off();
    vram_adr(NTADR_A(17,21));
    vram_write(" >", 2);
    vram_adr(NTADR_A(19,20));
    vram_write("  ", 2);
    vram_adr(NTADR_A(19,22));
    vram_write("  ", 2);
    ppu_on_all();
    iCount=1;
  }
  if (joy & PAD_UP)
  {
    if(iDnum != 1)
    {
      iDnum = 1;
      ppu_off();  
      vram_adr(NTADR_A(8,12));
      vram_write("                ", 16);    
      vram_adr(NTADR_A(17,21));
      vram_write("  ", 2);
      vram_adr(NTADR_A(19,20));
      vram_write("->", 2);
      vram_adr(NTADR_A(19,22));
      vram_write("  ", 2);
      ppu_on_all();
    }
  }
  if (joy & PAD_DOWN)  
  { 
    if(iDnum != 2)
    {
      iDnum = 2;
      ppu_off();
      vram_adr(NTADR_A(8,12));
      vram_write("                ", 16);      
      vram_adr(NTADR_A(17,21));
      vram_write("  ", 2);
      vram_adr(NTADR_A(19,20));
      vram_write("  ", 2);
      vram_adr(NTADR_A(19,22));
      vram_write("->", 2);
      ppu_on_all();
    }
  }
  if(joy & PAD_START)
  {
    if(iDnum == 1)
    {   
      vBattle();
      iDnum = 0;
      iCount = 0;
    }
    else if(iDnum == 2 & P_Run != 0)
    {  
      P_Run = P_Run - 1;
      ppu_off();
      Monster_Maker();
      Monster_Output();
      Player_LevelUp();
      Player();
      ppu_on_all();
      iDnum = 0;
      iCount = 0;
    }
  }


}
//-----------------main--------------
void main(void)
{ 
  inum = 0;  
  iDnum = 0;
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white
  
  while(1) 
  {
    pad = pad_trigger(0);
    
    if(inum == 0)
    {
      vTitleScreen();
      inum=1;
    }
    else if(pad & inum == 1)
    {
      vBuild();
      inum = 2;
    }
    // infinite loop
    else if(inum == 2)
      vGameState();
    else if (pad & inum == 3)
    {
      vEndScreen();
      if(pad)
        inum=0;
    }
  }
}

