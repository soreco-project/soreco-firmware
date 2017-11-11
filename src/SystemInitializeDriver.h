#ifndef SYSTEMINITIALIZEDRIVER_H
#define SYSTEMINITIALIZEDRIVER_H

#include <Arduino.h>

/**
 * SystemInitializeDriver is to configure the uC and it's peripherals. 
 */
class SystemInitializeDriver {
public:
	/**
	 * Initialize the pin configuration (PinMux) required by application.
	 */
	static void initPinConfig(void);
	
private:
	/**
    * Default constructor.
    */
    SystemInitializeDriver(void);
	
	/**
	* Destructor.
	*/
	~SystemInitializeDriver(void);

    /**
    * Private copy constructor.
    */
    SystemInitializeDriver(const SystemInitializeDriver&);
	
	/**
	* Private assignment constructor.
	*/
	SystemInitializeDriver& operator=(const SystemInitializeDriver&);

};

#endif //SYSTEMINITIALIZEDRIVER_H