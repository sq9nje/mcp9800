/**************************************************************************/
/*! 
    @file     mcp9800.h
    @author   Przemek Sadowski SQ9NJE
	  @license  BSD (see license.txt)
	
	  @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#ifndef __MCP9800_H__
#define __MCP9800_H__

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "utility/twi.h"
#include "Wire.h"

/* Default address for MCP9800/02 */
#define MCP_ADDR        (0x48)		// MCP9800/02A0
//#define MCP_ADDR      (0x4D)		// MCP9800/02A5

/* MCP registers */
#define MCP_TEMP        (0x00)
#define MCP_CONFIG      (0x01)
#define MCP_HYSTR       (0x02)
#define MCP_LIMIT       (0x03)

/* CONFIG register bits */
#define MCP_ONE_SHOT    (7)
#define MCP_ADC_RES     (5)
#define MCP_FAULT_QUEUE (3)
#define MCP_ALERT_POL   (2)
#define MCP_INT_MODE    (1)
#define MCP_SHUTDOWN    (0)

/* MCP conversion resolution */
#define MCP_9_BIT		(0x00)
#define MCP_10_BIT		(0x01)
#define MCP_11_BIT		(0x02)
#define MCP_12_BIT		(0x03)

/* MCP power mode */
#define MCP_PWR_UP		(0)
#define MCP_PWR_DOWN	(1)

/* MCP interrupt/comparator mode */
#define MCP_INTERRUPT	(1)
#define MCP_COMPARATOR	(0)

/* MCP alert polarity */
#define MCP_POL_HI		(1)
#define MCP_POL_LOW		(0)

class mcp9800 {
 private:
	uint8_t mcp_address;
 public:
   mcp9800(uint8_t address);
   uint8_t  getAddress();
   void     setAddress(uint8_t address);
   uint8_t  readConfig();
   void     writeConfig(uint8_t conf);
   void     setResolution(uint8_t resolution);
   void     setPowerMode(uint8_t mode);
   void     setComparatorMode(uint8_t mode);
   void     setAlertPolarity(uint8_t polarity);
   void     oneShot();
   uint16_t readTemp();
   float    toFloat(uint16_t temp);
   float    readTempC();
//   float    temp();
};

#endif	/*__MCP9800_H__*/