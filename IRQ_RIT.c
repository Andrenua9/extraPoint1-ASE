/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "GLCD.h"
#include "stdio.h"
//#include "../led/led.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern volatile int schermataGioco[19][23];
volatile static int position = 0;
volatile int down_0=0,down_1,down_2;

volatile static int up=0,down=0,sx=0,dx=0;

volatile int countdown = 60;

 int score = 0;
 int lives = 1;
 int pausa = 1;
 
 volatile int nPillole;


int* trovaPosPacman(){
	static int res[2];
	int i,j;
	for(i = 0; i<19; i++){
		for(j = 0; j<23; j++){
			if(schermataGioco[i][j] == 5){ //trovato Pacman
				res[0] = i;
				res[1] = j;
			}
		}
	}
	return res;
}

int incontraMuro( int posx, int posy){
		if(pausa == 1)
			return 0;
	if (posx<0)
		return 1;
	if(schermataGioco[posx][posy] == 1 )
		return 0;
	return 1;
}

int contaPillole(){
	int c=0;
	int i,j;
	for(i = 0; i<19; i++){
		for(j = 0 ; j<23; j++){
			if(schermataGioco[i][j]== 2 || schermataGioco[i][j] == 6)
				c++;
		}
	}
	return c;
}

void aggiornaScore(int posx, int posy){
	 if(schermataGioco[posx][posy] == 2){
				 score += 10;
	 }
	 if(schermataGioco[posx][posy] == 6){
				 score += 50;
	 }
	  char scoreStr[20];
		sprintf(scoreStr, "Score: %d", score);
		GUI_Text(11, 6, (uint8_t *)scoreStr, Red, Black);
	  reset_RIT();
	 
	 if(score%1000 == 0 && score != 0){
		 lives ++;
		 char livesStr[20];
		 sprintf(livesStr, "Remaining Lives: %d", lives);
		 GUI_Text(11, 300, (uint8_t *)livesStr, Red, Black);
	 }
	 if(nPillole == 0){
			GUI_Text(85, 140, (uint8_t *)"HAI VINTO", Yellow, Black);
			pausa = 1;
		}
	 
}

void mangiaPillola(int posx, int posy){
	if(schermataGioco[posx][posy] == 2 || schermataGioco[posx][posy] == 6){
		nPillole--;
	}
}
void RIT_IRQHandler (void)
{
	nPillole = contaPillole();
	
	if(pausa == 0){
		int* res = trovaPosPacman();
		int posx = res[0];
		int posy = res[1];
		
		/* UP */
		if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0){ 
			/* Joystick UP pressed */
			up = 1; down = 0; sx = 0; dx = 0;
			while(incontraMuro(posx, posy - 1) && pausa==0){
				mangiaPillola(posx,posy-1);
				aggiornaScore(posx, posy - 1);
				schermataGioco[posx][posy] = 0;
				schermataGioco[posx][posy - 1] = 5;
				LCD_DrawDot((posx * 12) + 12, ((posy - 1) * 12) + 28, 4, Yellow);
				LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
				posy = posy - 1;
				if((LPC_GPIO1 -> FIOPIN & (1<<26)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<27)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<28)) == 0) break;
			}
		}

		/* DOWN */
		if((LPC_GPIO1 -> FIOPIN & (1<<26)) == 0){ 
			/* Joystick DOWN pressed */
			down = 1; up = 0; sx = 0; dx = 0;
			while(incontraMuro(posx, posy + 1)){
				mangiaPillola(posx,posy+1);
				aggiornaScore(posx, posy + 1);
				schermataGioco[posx][posy] = 0;
				schermataGioco[posx][posy + 1] = 5;
				LCD_DrawDot((posx * 12) + 12, ((posy + 1) * 12) + 28, 4, Yellow);
				LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
				posy = posy + 1;
				if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<27)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<28)) == 0) break;
			}
		}

		/* SX */
		if((LPC_GPIO1 -> FIOPIN & (1<<27)) == 0){ 
			/* Joystick SX pressed */
			sx = 1; up = 0; down = 0; dx = 0; 
			while(incontraMuro(posx - 1, posy)){
				if(posx == 0){ // gestione teletrasporto
					schermataGioco[posx][posy] = 0;
					schermataGioco[18][posy] = 5;
					LCD_DrawDot((18 * 12) + 12, (posy * 12) + 28, 4, Yellow);
					LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
					posx = 18;
				}
				mangiaPillola(posx-1,posy);
				aggiornaScore(posx - 1, posy);
				schermataGioco[posx][posy] = 0;
				schermataGioco[posx - 1][posy] = 5;
				LCD_DrawDot(((posx - 1) * 12) + 12, (posy * 12) + 28, 4, Yellow);
				LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
				posx = posx - 1;
				if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<26)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<28)) == 0) break;
			}
		}

		/* DX */
		if((LPC_GPIO1 -> FIOPIN & (1<<28)) == 0){ 
			/* Joystick DX pressed */
			dx = 1; up = 0; down = 0; sx = 0;
			while(incontraMuro(posx + 1, posy)){
				if(posx == 18){ // gestione teletrasporto
					schermataGioco[posx][posy] = 0;
					schermataGioco[0][posy] = 5;
					LCD_DrawDot(12, (posy * 12) + 28, 4, Yellow);
					LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
					posx = 0;
				}
				mangiaPillola(posx+1,posy);
				aggiornaScore(posx + 1, posy);
				schermataGioco[posx][posy] = 0;
				schermataGioco[posx + 1][posy] = 5;
				LCD_DrawDot(((posx + 1) * 12) + 12, (posy * 12) + 28, 4, Yellow);
				LCD_DrawDot((posx * 12) + 12, (posy * 12) + 28, 4, Black);
				posx = posx + 1;
				if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<26)) == 0 || (LPC_GPIO1 -> FIOPIN & (1<<27)) == 0) break;
			}
		}
	}

	/* button management */
	if(down_0 != 0){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			down_0++;
			reset_RIT();
			switch(down_0){
				case 2:
					if(pausa == 0){
						pausa = 1;
						GUI_Text(100, 160, (uint8_t *)"PAUSE", Red, Black);
					}else{
						pausa = 0;
						GUI_Text(100, 160, (uint8_t *)"PAUSE", Black, Black);
						riempiSchermata(7, 11);
					}
					break;
				default:
					break;
			}
		}else{
			down_0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 20);
		}
	}

        LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
        return; 
    
}


/******************************************************************************
**                            End Of File
******************************************************************************/
