#include "mss_i2c.h"
#include "mss_uart.h"
#include "core_i2c.h"
#include "core_uart_apb.h"
#define MAX_RX_DATA_SIZE 512
//This the baud value after fine tuning to acheive a 9600 baud rate 
//The waveforms for this baud rate have been confirmed in an oscilloscope
#define BAUD_VALUE_9600 653
#define COREUART3_BASE_ADDR 	0x50001000u
#define COREUART3_BAUD_VALUE 	BAUD_VALUE_9600
UART_instance_t uhf_test_uart;
uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

//******* PiLOT UHF Test S1*******************\\

	//Initializing the UHF UART
	//Change the baud rate according to the Baud Rate of the PiLOT UHF module
	UART_init(&uhf_test_uart, COREUART3_BASE_ADDR,COREUART3_BAUD_VALUE, (DATA_8_BITS | NO_PARITY));

	uint8_t beacon_test_command_1[] = {'!','','e','l','l','o','w','o','r','l','d','\r'};
	uint8_t beacon_test_command_2[] = {'!','P','i','L','O','T','T','T','C','i','s',
	'b','e','t','t','e','r','t','h','a','n','a','i','r','t','e','l','\r'};


	while(1){
		//Send the command to the UHF radio
		UART_send(&uhf_test_uart,(const uint8_t *)&beacon_test_command,sizeof(beacon_test_command));
	}
}
