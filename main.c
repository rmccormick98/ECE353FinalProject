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
volatile uint16_t SHIP_Y_COORD = 259;
volatile uint16_t INVADER_X_COORD = 50;
volatile uint16_t INVADER_Y_COORD = 61;

volatile uint16_t BALL_X_COORD_1 = 80;
volatile uint16_t BALL_Y_COORD_1 = 180;

volatile uint16_t BALL_X_COORD_2 = 80;
volatile uint16_t BALL_Y_COORD_2 = 180;

volatile int trump_ball_cnt = 1;
volatile int clinton_ball_cnt = 1;


volatile bool ALERT_INVADER = true;
volatile bool BLINK_LED = false;
volatile bool pause_game = false;
volatile bool dir_ship = true;
// bool to determine which character has ball
// true for top, false for bottom
volatile bool ball_possesion_1 = true;
volatile bool ball_possesion_2 = false;

//ended up using hit_trump_1 and hit_clinton_1 in replace of ball_stopped

volatile bool hit_trump_1 = true; 
volatile bool hit_clinton_1 = false;

volatile bool hit_trump_2 = false; 
volatile bool hit_clinton_2 = true;

//volatile bool clear_ball = false;

//used eventually to display lives with red leds of io expander, going to arbitatrily choose trump as invader
volatile uint16_t lives_trump = 4;    
volatile uint16_t lives_clinton = 4; 

// global varaible flag to move ship


//*****************************************************************************
//*****************************************************************************
// same as check game over in hw3
// invader is the character and ship is the ball
bool check_char(
				volatile uint16_t BALL_X_COORD, 
        volatile uint16_t BALL_Y_COORD, 
        uint8_t ball_height, 
        uint8_t ball_width,
        volatile uint16_t char_x_coord, 
        volatile uint16_t char_y_coord, 
        uint8_t char_height, 
        uint8_t char_width

){
	
	// rectangle variables for ball
	uint16_t  top_s = BALL_Y_COORD - (ball_height/2); 
	uint16_t  bottom_s = BALL_Y_COORD + (ball_height/2);
	uint16_t right_s = BALL_X_COORD + (ball_width/2);
	uint16_t  left_s = BALL_X_COORD - (ball_width/2);
	// rectangle variables for invader
	uint16_t top_r =  char_y_coord - (char_height/2); 
	uint16_t bottom_r =  char_y_coord + (char_height/2); 
	uint16_t right_r = char_x_coord + (char_width/2); 
	uint16_t left_r = char_x_coord - (char_width/2);

		// checks space ship underneath and right overlap
		if(	 (bottom_r >= top_s && bottom_r <= bottom_s) && 
			(right_r >= left_s && right_r <= right_s)){
			//while(1){}
				return true;
			}
			// checks space ship underneath and left overlap
		else if ((bottom_r >= top_s && bottom_r <= bottom_s) && 
			(left_r >= left_s && left_r <= right_s)){
				return true;
		}
			// checks space ship above and right over lap
		else if ((top_r >= top_s && top_r <= bottom_s) && 
			(right_r >= left_s && right_r <= right_s)){
				return true;
		}
			// checks space ship above and left over lap
		else if ((top_r >= top_s && top_r <= bottom_s) && 
			(left_r >= left_s && left_r <= right_s)){
				return true;
			// the below conditions check the case when the space ship 
			// and invader overlap head on. This is necessary because
			// the space ship area is less than the invader
		}else if ((bottom_r >= top_s && bottom_r <= bottom_s) && 
			(left_r <= left_s && right_r >= right_s)){
				return true;
		}else if ((top_r <= bottom_s && top_s <= top_r) && 
			(left_r <= left_s && right_r >= right_s)){
				return true;
		}else if ((right_s >= left_r && left_r >= left_s) && 
			(top_r <= top_s && bottom_r >= bottom_s)){
			return true;
		}else if ((right_r >= left_s && right_s >= right_r ) && 
			(top_r <= top_s && bottom_r >= bottom_s)){
			return true;
		}
	// default case
  return false;
	
	
	
}
void lives_led(){
	int led_value;
	
	if(lives_clinton == 4 && lives_trump== 4){
		 led_value = 0xFF;
	}
	else if(lives_clinton == 4 && lives_trump == 3){
		 led_value = 0xF7;
	}
	else if(lives_clinton == 4  && lives_trump == 2){
		 led_value = 0xF3;
	}
	else if(lives_clinton == 4  && lives_trump == 1){
		led_value = 0xF1;
	}
	else if(lives_clinton == 4  && lives_trump == 0){
		led_value = 0xF0;
	}
	else if(lives_clinton == 3 && lives_trump == 4){
		 led_value = 0x7F;
	}
	else if(lives_clinton == 3 && lives_trump == 3){
		 led_value = 0x77;
	}
	else if(lives_clinton == 3  && lives_trump == 2){
		 led_value = 0x73;
	}
	else if(lives_clinton == 3  && lives_trump == 1){
		led_value = 0x71;
	}
	else if(lives_clinton == 3  && lives_trump == 0){
		led_value = 0x70;
	}
	else if(lives_clinton == 2 && lives_trump == 4){
		 led_value = 0x3F;
	}
	else if(lives_clinton == 2 && lives_trump == 3){
		 led_value = 0x37;
	}
	else if(lives_clinton == 2  && lives_trump == 2){
		 led_value = 0x33;
	}
	else if(lives_clinton == 2  && lives_trump == 1){
		led_value = 0x31;
	}
	else if(lives_clinton == 2  && lives_trump == 0){
		led_value = 0x30;
	}
	else if(lives_clinton == 1 && lives_trump == 4){
		 led_value = 0x1F;
	}
	else if(lives_clinton == 1 && lives_trump == 3){
		 led_value = 0x17;
	}
	else if(lives_clinton == 1  && lives_trump == 2){
		 led_value = 0x13;
	}
	else if(lives_clinton == 1  && lives_trump == 1){
		led_value = 0x11;
	}
	else if(lives_clinton == 1  && lives_trump == 0){
		led_value = 0x10;
	}
	else if(lives_clinton == 0 && lives_trump == 4){
		 led_value = 0x0F;
	}
	else if(lives_clinton== 0 && lives_trump == 3){
		 led_value = 0x07;
	}
	else if(lives_clinton == 0  && lives_trump == 2){
		 led_value = 0x03;
	}
	else if(lives_clinton == 0  && lives_trump == 1){
		led_value = 0x01;
	}
	else if(lives_clinton == 0  && lives_trump == 0){
		led_value = 0x00;
	}
	
  io_expander_write_reg(MCP23017_GPIOA_R, led_value);	

}

// returns new ball_stopped value
// returns value of ball_stopped
 void throw_ball(
					//bool ball_possesion_1,
					//bool ball_stopped,
					volatile uint16_t *ball_x, 
					volatile uint16_t *ball_y,
					volatile uint16_t *ball_x_2, 
					volatile uint16_t *ball_y_2,			
					uint8_t image_height, 
					uint8_t image_width)
{
	bool hit;
	bool edge;
	//printf("%u\n", *ball_y );
	/* Three cases for each character: 
	 *	1) not on the edge or contacted a character -> continue moving ball good
	 *	2) miss contacted edge and not character, stop and give ball to character
	 *	3) hit character -> stop and give ball to character
	 */
		if(ball_possesion_1){
			hit = check_char(BALL_X_COORD_1, BALL_Y_COORD_1, ballHeightPixels, ballWidthPages, SHIP_X_COORD, SHIP_Y_COORD, space_shipHeightPixels, space_shipWidthPixels);
		
			edge = contact_edge(PS2_DIR_DOWN, *ball_x, *ball_y, ballHeightPixels, ballWidthPages);
			
			if(!hit_trump_1){
				if( !hit && edge ){ //still moving down the screen
					*ball_y = *ball_y + 1;
					hit_trump_1 = false;
			
				}else if (hit && edge) { //ball was still moving and contacted character, decrement lives and set hit_trump_1 to true so ball will follow character
					lives_trump = lives_trump - 1;
					hit_trump_1 = true;
					trump_ball_cnt = trump_ball_cnt + 1;
				
				}else if(!edge){ //ball is on edge and character picked it up, do not decrement lives, set hit_trump_1 to true so ball will follow character
					// miss and ball is on edge
					if(hit){
						hit_trump_1 = true;
						trump_ball_cnt = trump_ball_cnt + 1;
					}
					else{
					    hit_trump_1 = false;
					}
				}
			}
			// if bottom has ball
		}else{
			hit = check_char(BALL_X_COORD_1, BALL_Y_COORD_1, ballHeightPixels, ballWidthPages, INVADER_X_COORD, INVADER_Y_COORD, invaderHeightPixels, invaderWidthPixels);
			
			edge = contact_edge(PS2_DIR_UP, *ball_x, *ball_y, ballHeightPixels, ballWidthPages);
			
			if(!hit_clinton_1){
				if( !hit && edge ){ 
					*ball_y = *ball_y - 1;
					hit_clinton_1 = false;
			
				}else if (hit && edge) { 
					lives_clinton = lives_clinton - 1;
					hit_clinton_1 = true;
					clinton_ball_cnt = clinton_ball_cnt + 1;
			
				}else if(!edge){   
					if(hit){
						hit_clinton_1 = true;
						clinton_ball_cnt = clinton_ball_cnt + 1;
					}
					else{
							hit_clinton_1 = false;
					}
				}
			}
	}
		
	
	if(ball_possesion_2){
			hit = check_char(BALL_X_COORD_2, BALL_Y_COORD_2, ballHeightPixels, ballWidthPages, SHIP_X_COORD, SHIP_Y_COORD, space_shipHeightPixels, space_shipWidthPixels);
		
			edge = contact_edge(PS2_DIR_DOWN, *ball_x_2, *ball_y_2, ballHeightPixels, ballWidthPages);
			
			if(!hit_trump_2){
				if( !hit && edge ){ //still moving down the screen
					*ball_y_2 = *ball_y_2 + 1;
					hit_trump_2 = false;
			
				}else if (hit && edge) { //ball was still moving and contacted character, decrement lives and set hit_trump_1 to true so ball will follow character
					lives_trump = lives_trump - 1;
					hit_trump_2 = true;
					trump_ball_cnt = clinton_ball_cnt + 1;
				
				}else if(!edge){ //ball is on edge and character picked it up, do not decrement lives, set hit_trump_1 to true so ball will follow character
					// miss and ball is on edge
					if(hit){
						hit_trump_2 = true;
						trump_ball_cnt = clinton_ball_cnt + 1;
					}
					else{
					    hit_trump_2 = false;
					}
				}
			}
			// if bottom has ball
		}else{
			hit = check_char(BALL_X_COORD_2, BALL_Y_COORD_2, ballHeightPixels, ballWidthPages, INVADER_X_COORD, INVADER_Y_COORD, invaderHeightPixels, invaderWidthPixels);
			
			edge = contact_edge(PS2_DIR_UP, *ball_x_2, *ball_y_2, ballHeightPixels, ballWidthPages);
			
			if(!hit_clinton_2){
				if( !hit && edge ){ 
					*ball_y_2 = *ball_y_2 - 1;
					hit_clinton_2 = false;
			
				}else if (hit && edge) { 
					lives_clinton = lives_clinton - 1;
					hit_clinton_2 = true;
					clinton_ball_cnt = clinton_ball_cnt + 1;
			
				}else if(!edge){   
					if(hit){
						hit_clinton_2 = true;
						clinton_ball_cnt = clinton_ball_cnt + 1;
					}
					else{
							hit_clinton_2 = false;
					}
				}
			}
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
		bool start_game = false;
		
	  bool clear_image_1 = true;
	  bool clear_image_2 = true;
	//uint16_t ball_r = 15;
	//	bool hit_char = false;
		
		SHIP_X_COORD = 190;
		SHIP_Y_COORD = 259;
		
	//DisableInterrupts();
	init_hardware();
//EnableInterrupts();
	

    while(1){
//			while(!start_game){
//				lcd_draw_image(
//                          103,                       // X Center Point
//                          title2WidthPixels,   // Image Horizontal Width
//                          119,                       // Y Center Point
//                          title2HeightPixels,  // Image Vertical Height
//                          title2Bitmaps,       // Image
//                          LCD_COLOR_BLUE,           // Foreground Color
//                          LCD_COLOR_RED          // Background Color
//                        );
//				
//			}
			
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
		
					

				 lcd_draw_image(
                          INVADER_X_COORD,          // X Center Point
                          invaderWidthPixels,       // Image Horizontal Width
                          INVADER_Y_COORD,          // Y Center Point
                          invaderHeightPixels,      // Image Vertical Height
                          invaderBitmaps,           // Image
                          LCD_COLOR_RED,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
					throw_ball(&BALL_X_COORD_1, &BALL_Y_COORD_1,&BALL_X_COORD_2, &BALL_Y_COORD_2, ballHeightPixels, ballWidthPages);
					
				if(!hit_clinton_1 && !hit_trump_1){
					clear_image_1 = true;
					lcd_draw_image(
													BALL_X_COORD_1,          // X Center Point
                          ballWidthPages,       // Image Horizontal Width
                          BALL_Y_COORD_1,          // Y Center Point
                          ballHeightPixels,      // Image Vertical Height
                          ballBitmaps,           // Image
                          LCD_COLOR_GREEN,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
					
					
				}
				else if(clear_image_1){ //clearing the image from the screen, not sure if this the best way to do that
						clear_image_1 = false;
						lcd_draw_image(
													BALL_X_COORD_1,          // X Center Point
                          ballWidthPages,       // Image Horizontal Width
                          BALL_Y_COORD_1,          // Y Center Point
                          ballHeightPixels,      // Image Vertical Height
                          ballBitmaps,           // Image
                          LCD_COLOR_BLACK,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
				}
				
				if(!hit_clinton_2 && !hit_trump_2){
					clear_image_2 = true;
					lcd_draw_image(
													BALL_X_COORD_2,          // X Center Point
                          ballWidthPages,       // Image Horizontal Width
                          BALL_Y_COORD_2,          // Y Center Point
                          ballHeightPixels,      // Image Vertical Height
                          ballBitmaps,           // Image
                          LCD_COLOR_BLUE,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
					
					
				}
				else if(clear_image_2) { //clearing the image from the screen, not sure if this the best way to do that
							clear_image_2 = false;
							lcd_draw_image(
													BALL_X_COORD_2,          // X Center Point
                          ballWidthPages,       // Image Horizontal Width
                          BALL_Y_COORD_2,          // Y Center Point
                          ballHeightPixels,      // Image Vertical Height
                          ballBitmaps,           // Image
                          LCD_COLOR_BLACK,            // Foreground Color
                          LCD_COLOR_BLACK           // Background Color
                        );	
					
				}
				
		if(hit_clinton_1){
			  BALL_X_COORD_1 = INVADER_X_COORD;
			  BALL_Y_COORD_1 = INVADER_Y_COORD + (invaderHeightPixels / 2) + (ballHeightPixels/2);
			
		}
		if(hit_trump_1){
			  BALL_X_COORD_1 = SHIP_X_COORD;
			  BALL_Y_COORD_1 = SHIP_Y_COORD - (space_shipHeightPixels / 2) - (ballHeightPixels/2);
			
		}
		if(hit_clinton_2){
			  BALL_X_COORD_2 = INVADER_X_COORD;
			  BALL_Y_COORD_2 = INVADER_Y_COORD + (invaderHeightPixels / 2) + (ballHeightPixels/2);
			
		}
		if(hit_trump_2){
			  BALL_X_COORD_2 = SHIP_X_COORD;
			  BALL_Y_COORD_2 = SHIP_Y_COORD - (space_shipHeightPixels / 2) - (ballHeightPixels/2);
			
		}

	
		
		
		if((touch_event > 0)){  //
				x = ft6x06_read_x();
				y = ft6x06_read_y();
			  if(ball_possesion_1 && (y<=160) && hit_trump_1){
				  trump_ball_cnt = trump_ball_cnt - 1;
					hit_trump_1 = false;
			
					ball_possesion_1 = !ball_possesion_1;
				}
				else if(ball_possesion_2 && (y<=160) && hit_trump_2){                 
				
					hit_trump_2 = false;
					trump_ball_cnt = trump_ball_cnt - 1;
					ball_possesion_2 = !ball_possesion_2;
				}
				if((!ball_possesion_1) &&  (y>=160) && hit_clinton_1){
						
							hit_clinton_1 = false;	
					    clinton_ball_cnt = clinton_ball_cnt - 1;
					    ball_possesion_1 = !ball_possesion_1;
				}
				else if((!ball_possesion_2) &&  (y>=160) && hit_clinton_2){              
						
							hit_clinton_2 = false;
					    clinton_ball_cnt = clinton_ball_cnt - 1;
					    ball_possesion_2 = !ball_possesion_2;
				}
		}	
		
		if(BLINK_LED){
				BLINK_LED = false;
				// toggle
				GPIOF->DATA = GPIOF->DATA ^ BLUE_M;
			}
		lives_led();
			

		}
		}
