// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// #include "main.h"
#include "project_interrupts.h"


static volatile uint16_t PS2_X_DATA = 0;
static volatile uint16_t PS2_Y_DATA = 0;
static volatile PS2_DIR_t PS2_DIR = PS2_DIR_CENTER;

void GPIOF_Handler(void){

	uint8_t test;
//	static int up = 0;
//	static int down = 0;
//	static int left = 0;
//	static int right = 0;
 	//	CHANGE_DIR = true;
		printf("here");
//	if(!CHANGE_DIR){
//		CHANGE_DIR = true;
//		printf("%u\n", io_expander_read_reg(MCP23017_GPIOB_R));
//	}
	if(left_pressed()){
		//up++;
		dir_ship = true;
		printf("left\n");
	}else if(right_pressed()){
		dir_ship = false;
		printf("right\n");
	}else{}
//	else if(down_pressed()){
//		//down++;
//		SHIP_Y_COORD = SHIP_Y_COORD + 5;
//	}else if(right_pressed()){
//		//right++;
//		SHIP_X_COORD = SHIP_X_COORD + 5;
//	}else if(left_pressed()){
//		//left++;
//		SHIP_X_COORD = SHIP_X_COORD - 5;
//	}else{}
	
	// clear interrupt
	//	test = io_expander_read_reg(MCP23017_GPIOB_R);
	GPIOF->ICR |= 0x01; 	
	
}
//*****************************************************************************
// Returns the most current direction that was pressed.
//*****************************************************************************
PS2_DIR_t ps2_get_direction(void)
{
	
	PS2_DIR_t return_val;
	// check high
	if( PS2_X_DATA > 0xC00)
  {
      return_val = PS2_DIR_LEFT;
  }
	// check low
  else if ( PS2_X_DATA < 0x400)
  {
      return_val = PS2_DIR_RIGHT;  
  }
	// check high
  else if ( PS2_Y_DATA > 0xC00)
  {
      return_val = PS2_DIR_UP;  
  }
	// check low
  else if ( PS2_Y_DATA < 0x400)
  {
      return_val = PS2_DIR_DOWN;  
  }
	// default
	else{
		return_val = PS2_DIR_CENTER;
	}
	return return_val;
}
//*****************************************************************************
// TIMER1 ISR is used to blink LED every 1 sec
//*****************************************************************************
void TIMER1A_Handler(void){
	
	
	BLINK_LED = true;
	 // Clear the interrupt 
	TIMER1->ICR |= TIMER_ICR_TATOCINT;


}
//*****************************************************************************
// TIMER2 ISR is used to determine when to move the Invader
//*****************************************************************************
void TIMER2A_Handler(void)
{	
   
  // check to move the invader  
	if (contact_edge(PS2_DIR,  INVADER_X_COORD, INVADER_Y_COORD, invaderHeightPixels ,invaderWidthPixels )){
			move_image(PS2_DIR, &INVADER_X_COORD, &INVADER_Y_COORD , invaderHeightPixels, invaderWidthPixels);
			ALERT_INVADER = true;
	}
  // Clear the interrupt 
	TIMER2->ICR |= TIMER_ICR_TATOCINT;
	
}

//*****************************************************************************
// TIMER3 ISR is used to determine when to move the spaceship
//*****************************************************************************
void TIMER3A_Handler(void)
{	
 
	char input[80];
				memset(input, 0, 80);	
			  scanf("%79[^\n]", input);
				if(*input == ' '){
					  if(pause_game){
								pause_game = false;
						}
						else{
							pause_game = true;
						}
				}
				TIMER3->ICR |= TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// TIMER4 ISR is used to trigger the ADC
//*****************************************************************************
void TIMER4A_Handler(void)
{	
	// triggers adc
	ADC0->PSSI |= ADC_PSSI_SS2;
	// Clear the interrupt
	TIMER4->ICR |= TIMER_ICR_TATOCINT; 
}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{
  // read the FIFOs
  PS2_X_DATA = ADC0->SSFIFO2;
	PS2_Y_DATA = ADC0->SSFIFO2;
	PS2_DIR = ps2_get_direction();
	
	// Clear the interrupt
	ADC0->ISC |= ADC_ISC_IN2;

}