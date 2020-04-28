#ifndef __DEBOUNCE_H__
#define __DEBOUNCE_H__

#include <stdbool.h>
#include <stdint.h>
#include "launchpad_io.h"
#include "io_expander.h"

// Four buttons correspond to these GPIOB pins on the IO expander
#define R_PIN			3
#define L_PIN			2
#define D_PIN			1
#define U_PIN			0

// Debounce states
typedef enum 
{
  DEBOUNCE_ONE,
  DEBOUNCE_1ST_ZERO,
  DEBOUNCE_2ND_ZERO,
  DEBOUNCE_PRESSED
} DEBOUNCE_STATES;

//*****************************************************************************
// The debounce state machine
//*****************************************************************************
extern bool debounce_fsm(DEBOUNCE_STATES *state, bool pin_logic_level);

extern bool up_pressed(void);

extern bool down_pressed(void);

extern bool left_pressed(void);

extern bool right_pressed(void);

void debounce_wait(void);

#endif