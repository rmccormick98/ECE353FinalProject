#include "debounce_buttons.h"


//*****************************************************************************
// 
//*****************************************************************************
bool debounce_fsm(DEBOUNCE_STATES *state, bool pin_logic_level)
{
		//static DEBOUNCE_STATES state = DEBOUNCE_ONE;


  
  switch (*state)
  {
    case DEBOUNCE_ONE:
    {
      if(pin_logic_level)
      {
        *state = DEBOUNCE_ONE;
      }
      else
      {
        *state = DEBOUNCE_1ST_ZERO;
      }
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(pin_logic_level)
      {
        *state = DEBOUNCE_ONE;
      }
      else
      {
        *state = DEBOUNCE_2ND_ZERO;
      }
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level)
      {
        *state = DEBOUNCE_ONE;
      }
      else
      {
        *state = DEBOUNCE_PRESSED;
      }
      break;
    }
    case DEBOUNCE_PRESSED:
    {
      if(pin_logic_level)
      {
        *state = DEBOUNCE_ONE;
      }
      else
      {
        *state = DEBOUNCE_PRESSED;
      }
      break;
    }
    default:
    {
      while(1){};
    }
  }
  
  if(*state == DEBOUNCE_2ND_ZERO )
  {
    return true;
  }
  else
  {
    return false;
  }
}


bool up_pressed(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  uint8_t pin_logic_level;
  pin_logic_level = io_expander_read_reg(MCP23017_GPIOB_R);
  pin_logic_level &= 1<<U_PIN;
  return debounce_fsm(&state, pin_logic_level);
}

//*****************************************************************************
// Detect a single button press regardless of how long the user presses the 
// down button.
//*****************************************************************************
bool down_pressed(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  uint8_t pin_logic_level;
  pin_logic_level = io_expander_read_reg(MCP23017_GPIOB_R);
  pin_logic_level &= 1<<D_PIN;
  return debounce_fsm(&state, pin_logic_level);
}

//*****************************************************************************
// Detect a single button press regardless of how long the user presses the 
// left button.
//*****************************************************************************
bool left_pressed(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  uint8_t pin_logic_level;
  pin_logic_level = io_expander_read_reg(MCP23017_GPIOB_R);
  pin_logic_level &= 1<<L_PIN;
  return debounce_fsm(&state, pin_logic_level);
}

//*****************************************************************************
// Detect a single button press regardless of how long the user presses the 
// right button.
//*****************************************************************************
bool right_pressed(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  uint8_t pin_logic_level;
  pin_logic_level = io_expander_read_reg(MCP23017_GPIOB_R);
  pin_logic_level &= 1<<R_PIN;
  return debounce_fsm(&state, pin_logic_level);
}