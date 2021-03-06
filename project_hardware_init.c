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

#include "project_hardware_init.h"

bool init_hardware(void){
	
	init_serial_debug(true, false);
	lcd_config_gpio();
  lcd_config_screen();
	ft6x06_init();
  lcd_clear_screen(LCD_COLOR_BLACK);
  ps2_initialize();

	if(!io_expander_init()){
					return false;
	 }
	io_expander_write_reg(MCP23017_GPIOA_R, 0xFF);
	configure_buttons();
	// configure timers
	 // initial timers config
//	gp_timer_config_32(TIMER1_BASE,TIMER_TAMR_TAMR_PERIOD, 50000000, false, true);
//	gp_timer_config_32(TIMER2_BASE,TIMER_TAMR_TAMR_PERIOD, 1000000, false, true);
//	gp_timer_config_32(TIMER3_BASE,TIMER_TAMR_TAMR_PERIOD, 500000, false, true);
//  gp_timer_config_32(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD, 50000, false, true);	
	 // test config timers
	gp_timer_config_32(TIMER1_BASE,TIMER_TAMR_TAMR_PERIOD, 50000000, false, true);
	gp_timer_config_32(TIMER2_BASE,TIMER_TAMR_TAMR_PERIOD, 900000, false, true);
	gp_timer_config_32(TIMER3_BASE,TIMER_TAMR_TAMR_PERIOD, 400000, false, true);
	gp_timer_config_16(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD, 40000, false, true);
	 
	 
	 // enable leds on launch pad
	 
	 
	 
	 lp_io_init();
	 eeprom_init();
	 
	return true;
	 
}

