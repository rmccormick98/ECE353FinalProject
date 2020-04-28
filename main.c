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

#include "main.h"

// global variable for the ship position
volatile bool CHANGE_DIR = true;
volatile uint16_t SHIP_X_COORD = 190;
volatile uint16_t SHIP_Y_COORD = 270;
volatile uint16_t INVADER_X_COORD = 50;
volatile uint16_t INVADER_Y_COORD = 40;

volatile uint16_t BALL_X_COORD = 80;
volatile uint16_t BALL_Y_COORD = 180;

volatile bool ALERT_INVADER = true;
volatile bool BLINK_LED = false;
volatile bool pause_game = false;
volatile bool dir_ship = true;
// bool to determine which character has ball
// true for top, false for bottom
volatile bool ball_possesion = true;
// bool to determine is ball is moving or possessed
volatile bool ball_stopped = true;
// global varaible flag to move ship

//*****************************************************************************
//*****************************************************************************
// returns new ball_stopped value
bool throw_ball(
					bool ball_possesion,
					bool ball_stopped,
					volatile uint16_t *ball_x, 
					volatile uint16_t *ball_y,
					uint8_t image_height, 
					uint8_t image_width)
{
	printf("%u\n", *ball_y );
	//BUG: need to change possesion
	if(!ball_stopped){
		// if top has has ball
		if(ball_possesion){
			if(contact_edge(PS2_DIR_DOWN, *ball_x, *ball_y, ballHeightPixels, ballWidthPages)){
				*ball_y = *ball_y + 1;
				return false;
			}else{ return true; }
		}else{
			if(contact_edge(PS2_DIR_UP, *ball_x, *ball_y, ballHeightPixels, ballWidthPages)){
				*ball_y = *ball_y - 1;
				return false;
			}else{ return true; }
	}
}else{
	return true;
}

}


bool switch_direction(volatile uint16_t x_coord, uint8_t image_width)
		{
					if((x_coord - (image_width/2)) <= 0 ){
									dir_ship = false;
					}
					if((x_coord + (image_width/2)) >= 240 ){
								dir_ship = true;
					}
							
			
		}


void move_image(
        volatile PS2_DIR_t direction,
        volatile uint16_t *x_coord, 
        volatile uint16_t *y_coord, 
        uint8_t image_height, 
        uint8_t image_width
)
{
		// check left
    if(direction  == PS2_DIR_LEFT){
					*x_coord = *x_coord - 1;
	  }
		// check right
	  else if (direction == PS2_DIR_RIGHT){
					*x_coord = *x_coord + 1;
		}
		// check down
		else if (direction == PS2_DIR_DOWN){
					*y_coord = *y_coord + 1;
		}
		// check up
		else if (direction == PS2_DIR_UP){
					*y_coord = *y_coord - 1;
		}
		
}

bool contact_edge(
    volatile PS2_DIR_t direction,
    volatile uint16_t x_coord, 
    volatile uint16_t y_coord, 
    uint8_t image_height, 
    uint8_t image_width
)
{
					// check left edg3
					if(direction == PS2_DIR_LEFT){
							if((x_coord - (image_width/2)) > 0 ){
									return true;
							}
							else {
									return false;
							}
					}
					// check right edge
					else if(direction == PS2_DIR_RIGHT){
							if((x_coord + (image_width/2)) < 239 ){
									return true;
							}else{
									return false;
							}
					}
					// check bottom edge
					else if (direction == PS2_DIR_DOWN){
							if((y_coord + (image_height/2)) < 320){
									return true;
							}else{
									return false;
							}
					}
					// check top edge
					else if (direction == PS2_DIR_UP){
							if((y_coord - (image_height/2)) > 0){
									return true;
							}
							else{
									return false;
							}
					}
					// default 
					else {
							return true;
					}
}


void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
		uint16_t x,y;
		uint8_t touch_event;
		
		SHIP_X_COORD = 190;
		SHIP_Y_COORD = 270;
		
	//DisableInterrupts();
	init_hardware();
//EnableInterrupts();
	
	printf("test\n");
    while(1){
		while(pause_game){
				 // lcd_clear_screen(LCD_COLOR_BLACK);
		
		}
			touch_event = ft6x06_read_td_status();
	
		
		
		if(dir_ship){
			SHIP_X_COORD--;
			 lcd_draw_image(
                          SHIP_X_COORD,                       // X Center Point
                          space_shipWidthPixels,   // Image Horizontal Width
                          SHIP_Y_COORD,                       // Y Center Point
                          space_shipHeightPixels,  // Image Vertical Height
                          space_shipBitmaps,       // Image
                          LCD_COLOR_BLUE,           // Foreground Color
                          LCD_COLOR_BLACK          // Background Color
                        );
			
		}else{
			SHIP_X_COORD++;
			lcd_draw_image(
                          SHIP_X_COORD,                       // X Center Point
                          space_shipWidthPixels,   // Image Horizontal Width
                          SHIP_Y_COORD,                       // Y Center Point
                          space_shipHeightPixels,  // Image Vertical Height
                          space_shipBitmaps,       // Image
                          LCD_COLOR_BLUE,           // Foreground Color
                          LCD_COLOR_BLACK          // Background Color
                        );
		}
		
		switch_direction(SHIP_X_COORD, space_shipWidthPixels);
		
//		if(CHANGE_DIR)
//          {
//            CHANGE_DIR = false;
//            
//            lcd_draw_image(
//                          SHIP_X_COORD,                       // X Center Point
//                          space_shipWidthPixels,   // Image Horizontal Width
//                          SHIP_Y_COORD,                       // Y Center Point
//                          space_shipHeightPixels,  // Image Vertical Height
//                          space_shipBitmaps,       // Image
//                          LCD_COLOR_BLUE,           // Foreground Color
//                          LCD_COLOR_BLACK          // Background Color
//                        );
//		}
					
		  if(ALERT_INVADER)
					{
            ALERT_INVADER = false;
            
             lcd_draw_image(
                          INVADER_X_COORD,          // X Center Point
                          invaderWidthPixels,       // Image Horizontal Width
                          INVADER_Y_COORD,          // Y Center Point
                          invaderHeightPixels,      // Image Vertical Height
                          invaderBitmaps,           // Image
                          LCD_COLOR_RED,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );
					}
				 lcd_draw_image(
                          INVADER_X_COORD,          // X Center Point
                          invaderWidthPixels,       // Image Horizontal Width
                          INVADER_Y_COORD,          // Y Center Point
                          invaderHeightPixels,      // Image Vertical Height
                          invaderBitmaps,           // Image
                          LCD_COLOR_RED,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
					ball_stopped = throw_ball(ball_possesion, ball_stopped, &BALL_X_COORD, &BALL_Y_COORD, ballHeightPixels, ballWidthPages);
					printf("%d/n", ball_stopped);
					lcd_draw_image(
													BALL_X_COORD,          // X Center Point
                          ballWidthPages,       // Image Horizontal Width
                          BALL_Y_COORD,          // Y Center Point
                          ballHeightPixels,      // Image Vertical Height
                          ballBitmaps,           // Image
                          LCD_COLOR_GREEN,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
			if(ball_stopped && (touch_event > 0)){
				x = ft6x06_read_x();
				y = ft6x06_read_y();
				ball_stopped = false;
				ball_possesion = false;
			printf("X coordinate= %d Y coordinate= %d\n", x, y);
		}
			if(BLINK_LED){
				BLINK_LED = false;
				// toggle
				GPIOF->DATA = GPIOF->DATA ^ BLUE_M;
			}
			

		}
		}
