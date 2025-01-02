/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "GLCD.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int pausa;
extern int countdown;
extern volatile int lives;
extern volatile int schermataGioco[19][23];
volatile int contaPP=0;


int * generaVettore(){
	static int vett[6];
	int i;
	for(i = 0; i<6; i++){
		vett[i] = 25 + rand() % (55 - 25 + 1);
	}
	return vett;
}

int * bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
		return arr;
}

void TIMER0_IRQHandler (void)
{
	  int * vett = bubbleSort(generaVettore(),6); //contiene 6 tempi in cui vengono generate le pillole
    static char buffer[10]; //Buffer per contenere il testo da stampare
    
    if (LPC_TIM0->IR & 1) // MR0
    { 
        if (pausa == 0)
        {
            countdown--;
            if (countdown < 0) 
							countdown = 0;
						
            sprintf(buffer, "%d s", countdown);
            GUI_Text(200, 6, (uint8_t *)buffer, Red, Black);
						
						
						//Generazione Power Pills
						if (vett[contaPP] >= countdown && contaPP < 6) {
								int c, x, y;
								do {
										c = rand() % (19 * 23);
										x = c / 23;             
										y = c % 23;              
								}while(schermataGioco[x][y] != 2);
						LCD_DrawDot((x * 12) + 12, (y * 12) + 28, 3, Red);
						schermataGioco[x][y] = 6;
						contaPP++;
						}

            if (countdown == 0 && lives > 0)
            {
                countdown = 60;
                lives--;
								char livesStr[20];
								sprintf(livesStr, "Remaining Lives: %d", lives);
								GUI_Text(11, 300, (uint8_t *)livesStr, Red, Black);							
            }
						else if (lives == 0){
							GUI_Text(85, 140, (uint8_t *)"GAME OVER", Yellow, Black);
							pausa = 1;
						}

        }
        LPC_TIM0->IR = 1; // Pulisci il flag di interruzione
    }

    return;
}



/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM1->IR & 2){ // MR1
		LPC_TIM1->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		// your code	
		LPC_TIM1->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM1->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0
	{ 
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 4){ // MR2
		// your code	
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 8){ // MR3
		// your code	
		LPC_TIM2->IR = 8;			// clear interrupt flag 
	} 

	return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1) // MR0
	{ 
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 4){ // MR2
		// your code	
		LPC_TIM3->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 8){ // MR3
		// your code	
		LPC_TIM3->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
