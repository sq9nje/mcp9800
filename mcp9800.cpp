/**************************************************************************/
/*! 
    @file     mcp9800.cpp
    @author   Przemek Sadowski SQ9NJE
	@license  BSD (see license.txt)
	
	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include "mcp9800.h"

/**************************************************************************/
/*! 
    @brief  Instantiates a new MCP9808 class

    @param[in]	address
    			The address of the sensor IC on the I2C bus. Defaults to 
    			the value of MCP_ADDR defined in mcp9800.h
*/
/**************************************************************************/
mcp9800::mcp9800(uint8_t address = MCP_ADDR)
{
	mcp_address = address;
}

/**************************************************************************/
/*! 
    @brief  Returns the I2C bus address of the sensor. 
    		This is not auto detection, this method simply returns what the 
    		value has been set to.
*/
/**************************************************************************/
uint8_t mcp9800::getAddress()
{
	return mcp_address;
}

/**************************************************************************/
/*! 
    @brief  Sets the I2C address of the sensor.
*/
/**************************************************************************/
void mcp9800::setAddress(uint8_t address)
{
	mcp_address = address;
}

/**************************************************************************/
/*! 
    @brief  Reads the configuration register.
*/
/**************************************************************************/
uint8_t mcp9800::readConfig() {
	Wire.beginTransmission(MCP_ADDR);
	Wire.write(MCP_CONFIG);
	Wire.endTransmission();
	Wire.requestFrom(MCP_ADDR,1);
	return Wire.read();
}

/**************************************************************************/
/*! 
    @brief  Writes new values to the configuration register.
*/
/**************************************************************************/
void mcp9800::writeConfig(uint8_t conf)
{
	Wire.beginTransmission(MCP_ADDR);
	Wire.write(MCP_CONFIG);
	Wire.write(conf);
	Wire.endTransmission();
}

/**************************************************************************/
/*! 
    @brief  Sets the conversion resolution.

    @param[in]	resolution
    			Supported values are MCP_9_BIT, MCP_10_BIT, MCP_11_BIT or
    			MCP_12_BIT
*/
/**************************************************************************/
void mcp9800::setResolution(uint8_t resolution)
{
	uint8_t conf;
	conf = readConfig();
	conf &= ~(3 << MCP_ADC_RES);
	conf |= (resolution << MCP_ADC_RES);
	writeConfig(conf);
}

/**************************************************************************/
/*! 
    @brief  Enables or disables the device shutdown mode.
*/
/**************************************************************************/
void mcp9800::setPowerMode(uint8_t mode)
{
	uint8_t conf;
	conf = readConfig();
	conf &= ~(1 << MCP_SHUTDOWN);
	conf |= (mode << MCP_SHUTDOWN);
	writeConfig(conf);
}

/**************************************************************************/
/*! 
    @brief  Switches between Comparator Mode and Interrupt Mode.
*/
/**************************************************************************/
void mcp9800::setComparatorMode(uint8_t mode)
{
	uint8_t conf;
	conf = readConfig();
	conf &= ~(1 << MCP_INT_MODE);
	conf |= (mode << MCP_INT_MODE);
	writeConfig(conf);
}

/**************************************************************************/
/*! 
    @brief  Sets the polarity of alerts.
*/
/**************************************************************************/
void mcp9800::setAlertPolarity(uint8_t polarity)
{
	uint8_t conf;
	conf = readConfig();
	conf &= ~(1 << MCP_ALERT_POL);
	conf |= (polarity << MCP_ALERT_POL);
	writeConfig(conf);
}

/**************************************************************************/
/*! 
    @brief  Enables one shot mode. The device must be in shutdown mode 
    		first.
*/
/**************************************************************************/
void mcp9800::oneShot()
{
	uint8_t conf;
	conf = readConfig();
	conf |= (1 << MCP_ONE_SHOT);
	writeConfig(conf);
}

/**************************************************************************/
/*! 
    @brief  Reads the temperature from the sensor. Returns raw binary data.
*/
/**************************************************************************/
uint16_t mcp9800::readTemp()
{    
	uint16_t temp;
	Wire.beginTransmission(MCP_ADDR);
	Wire.write(MCP_TEMP);
	Wire.endTransmission();
	Wire.requestFrom(MCP_ADDR,2);
	temp = Wire.read()<<8;
	temp |= Wire.read();
	return temp;
}

/**************************************************************************/
/*! 
    @brief  Converts raw binary temperature data to a floating point value
    		in deg. C
*/
/**************************************************************************/
float mcp9800::toFloat(uint16_t temp)
{
	float t;
	uint8_t frac;

	t = temp >> 8;
	frac = temp & 0x00ff;

	if (frac & 0x80) t+=0.5f;
	if (frac & 0x40) t+=0.25f;
	if (frac & 0x20) t+=0.125f;
	if (frac & 0x10) t+=0.0625f;

	return t;
}

/**************************************************************************/
/*! 
    @brief  Returns temperature value in degrees C
*/
/**************************************************************************/
float mcp9800::readTempC()
{
	return toFloat(readTemp());
}

// float mcp9800::temp()
// {
//   uint8_t TempByte1, TempByte2;
  
//    Wire.beginTransmission(MCP_ADDR); 
//    Wire.write(0x01); // Address to Configuration register 
//    Wire.write(0x64); // ADC resolution: 12-bit, 
//    Wire.endTransmission(); 
//    // Read temperature 
//    Wire.beginTransmission(MCP_ADDR); 
//    Wire.write((byte)0x00); // Pointer to temperature register 
//    Wire.endTransmission();   
//    Wire.beginTransmission(MCP_ADDR); 
//    Wire.requestFrom(MCP_ADDR, 2); // Now read two bytes of temperature data 
//    if (Wire.available()) 
//    { 
//      TempByte1 = Wire.read(); 
//      TempByte2 = Wire.read();   
//    }
//    int Temperature = ((TempByte1 << 8) | TempByte2); 
//    Temperature = Temperature >> 4; 
//    float TempC = 1.0*Temperature*0.0625;
   
//    return TempC;
   
// }