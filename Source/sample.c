/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
** 
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
** 
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "../RIT.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "../joystick.h"
#include "../button.h"



#include <math.h>
#include <stdio.h>


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

//0--> spazio vuoto
//1--> muro
//2--> pills
//5--> pacman
//6--> power pills

#define Nr 19
#define Nc 23

 volatile int schermataGioco[Nr][Nc] = {
    {1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,1,0,0,1,0,1,0,0,1,2,2,2,1,2,2,2,1},
    {1,2,1,2,1,2,1,0,0,1,0,1,0,0,1,2,1,2,2,2,1,2,1},
    {1,2,1,2,1,2,1,1,1,1,0,1,1,1,1,2,1,1,1,2,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
    {1,2,1,2,1,1,1,1,1,1,2,1,1,1,1,2,1,2,1,1,1,2,1},
    {1,2,1,2,2,2,1,2,2,2,2,2,2,2,2,2,1,2,2,2,1,2,1},
    {1,2,1,2,1,2,1,2,2,1,1,1,2,2,1,2,1,2,1,2,1,2,1},
    {1,2,2,2,1,2,2,2,2,1,0,1,2,2,1,2,2,2,1,2,2,2,1},
    {1,1,1,2,1,1,1,2,2,0,5,1,2,2,1,1,1,2,1,1,1,2,1},
    {1,2,2,2,1,2,2,2,2,1,0,1,2,2,1,2,2,2,1,2,2,2,1},
    {1,2,1,2,1,2,1,2,2,1,1,1,2,2,1,2,1,2,1,2,1,2,1},
    {1,2,1,2,2,2,1,2,2,2,2,2,2,2,2,2,1,2,2,2,1,2,1},
    {1,2,1,2,1,1,1,1,1,1,2,1,1,1,1,2,1,2,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
    {1,2,1,2,1,2,1,1,1,1,0,1,1,1,1,2,1,1,1,2,1,2,1},
    {1,2,1,2,1,2,1,0,0,1,0,1,0,0,1,2,1,2,2,2,1,2,1},
    {1,2,2,2,2,2,1,0,0,1,0,1,0,0,1,2,2,2,1,2,2,2,1},
    {1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1}
};
 
 
		void riempiSchermata(int r, int c){
			int i ,j; 
			int mh = 22; //margine d'altezza per inserire score e countdown
			int ml = 6; //margine in larghezza
			int n = 12; //n. pixel per lato
			for(i=r; i<Nr ; i++){
				for(j=c ; j<Nc; j++){
					if(schermataGioco[i][j] == 1)
						LCD_DrawSquare((i*n)+ml,(j*n)+mh,Blue);
					else if(schermataGioco[i][j] == 2)
						LCD_DrawCircle((i*n)+ml+6,(j*n)+mh+6,1,Red);
				  else if(schermataGioco[i][j]==5)
					LCD_DrawDot((i*n)+ml+6,(j*n)+mh+6,4,Yellow); //disenga Pac
				}
			}
}
		

int main(void)
{
   SystemInit();  												/* System Initialization (i.e., PLL)  */
	 joystick_init();
   LCD_Initialization();
	 BUTTON_init();
	
   //TP_Init();
	 //TouchPanel_Calibrate();
	
	LCD_Clear(Black);

	GUI_Text(11, 6, (uint8_t *)"Score: 0", Red, Black);
	GUI_Text(11, 300, (uint8_t *)"Remaining Lives: 1", Red, Black);
	init_RIT(0x000F4240); //10ms
	enable_RIT();
	
	riempiSchermata(0,0);
	GUI_Text(100, 160, (uint8_t *)"PAUSE", Red, Black);
	
	
	
	
	//LCD_DrawLine(0, 0, 200, 200, White);
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
//	init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	//enable_timer(0);
	
	init_timer(0, 999, 0, 3, 1000);  // 1000ms = 1 secondo

	enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
