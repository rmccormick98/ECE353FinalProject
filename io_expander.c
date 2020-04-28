#include "io_expander.h"

//*****************************************************************************
// Initialize the I2C peripheral
//*****************************************************************************
bool io_expander_init(void){
	  if(gpio_enable_port(IO_EXPANDER_GPIO_BASE) == false)
  {
    return false;
  }
  
  // Configure SCL 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL)== false)
  {
    return false;
  }
    

  
  // Configure SDA 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_open_drain(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL)== false)
  {
    return false;
  }
    
  //  Initialize the I2C peripheral
  if( initializeI2CMaster(IO_EXPANDER_I2C_BASE)!= I2C_OK)
  {
    return false;
  }
  
  return true;

}
void io_expander_write_reg(uint8_t reg, uint8_t data){
  
	i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

  // Send the register Address
  i2cSendByte( IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);

  // Send data
  i2cSendByte( IO_EXPANDER_I2C_BASE, data, I2C_MCS_RUN | I2C_MCS_STOP);

}
uint8_t io_expander_read_reg(uint8_t reg){
	i2c_status_t status;
	uint8_t data;
// Set I2C address
i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE); 

// Send the Address 
i2cSendByte( IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);

// Set I2C address, 
i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_READ);

// Issue Re-Start, Read IODIR Register 
status = i2cGetByte( 
                     IO_EXPANDER_I2C_BASE, 
                     &data, 
                     I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP
                    );
	
	return data;
}
//*****************************************************************************
// Configure the I/O expander to take input from the buttons.
//*****************************************************************************
i2c_status_t configure_buttons(void)
{
	i2c_status_t status;
	uint8_t reg;
	
	// enable port f
	gpio_enable_port(IO_EXPANDER_IRQ_GPIO_BASE);
	// enable digital pin
	gpio_config_digital_enable(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	// config as an input
	gpio_config_enable_input(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	// enable pull up
	//gpio_config_enable_pullup(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	// irq
	gpio_config_falling_edge_irq(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	// set gpiof 
	
	NVIC_SetPriority(gpio_get_irq_num(IO_EXPANDER_IRQ_GPIO_BASE),1);
  NVIC_EnableIRQ(gpio_get_irq_num(IO_EXPANDER_IRQ_GPIO_BASE));
	// fails here
	//gpio_config_falling_edge_irq(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	// test with different pin

	

	
//	// Configure GPIOB port as input
  	io_expander_write_reg(MCP23017_IODIRA_R, 0x00);
//	io_expander_write_reg(MCP23017_IODIRB_R, 0xFF);
//	//if (status != I2C_OK) return status;
//	
//	// Enable pull-up resistors

//	io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);
//	
//	// set GPINTEN register
//	io_expander_write_reg(MCP23017_GPINTENB_R, 0x0F);
//	// enable defval register
//	io_expander_write_reg(MCP23017_DEFVALB_R, 0x0F);
//	// set int con
//	io_expander_write_reg(MCP23017_INTCONB_R, 0x0F);
	
	// Set PortB [7:0] to be inputs
  io_expander_write_reg(MCP23017_IODIRB_R, 0x0F);
  
  // Enable Pull-ups.
  io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);
  
  // Read the INTCAP Register to clear any outstanding interrupts
  reg =  io_expander_read_reg(MCP23017_INTCAPB_R);
    
  // Read the MCP23017_GPIOB_R Register to clear any outstanding interrupts
  reg = io_expander_read_reg(MCP23017_GPIOB_R);

  // Configure pins to generate interrupts on pins 3-0
  io_expander_write_reg(MCP23017_GPINTENB_R, 0x0F);
  
  // Interrupt when the signal is low    
 // io_expander_write_reg(MCP23017_DEFVALB_R, 0x0F);

  // Pins 3-0 interrupt based on the value in DEFVAL    
  io_expander_write_reg(MCP23017_INTCONB_R, 0x00);
  
  // Interrupts are generated when a change from the previous value occurs.
//  io_expander_write_reg(MCP23017_IOCONB_R, 0x0F);
  
  // Read the INTCAP Register to clear any outstanding interrupts
  //reg =  io_expander_read_reg(MCP23017_INTCAPB_R);
    
  // Read the MCP23017_GPIOB_R Register to clear any outstanding interrupts
  reg = io_expander_read_reg(MCP23017_GPIOB_R);    
	
	
	
	return status;
}