#include "mss_i2c.h" //so that the main code will not be too long just for declaration
#include "core_uart_apb.h"
#include "mss_uart.h"

#define COREUART0_BASE_ADDR 	0x50000000u
#define COREUART1_BASE_ADDR 	0x50001000u
#define COREUART2_BASE_ADDR 	0x50002000u

//This value needs to be calculated depending on the speed of operation
//Can refer IS4
#define BAUD_VALUE_57600    25
#define MAX_RX_DATA_SIZE 256

#define TX_LENGTH 1u
#define RX_LENGTH 1u
#define MSS_I2C_DUMMY_ADDR 0x10u

UART_instance_t rs422_test_uart;

uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

uint8_t rx_data[MAX_RX_DATA_SIZE];

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

//*********************************************** UART Testing S1**********************
	//MSS_UART0 working , MSS_UART_1 - Working
	//CoreUART2 with Isolator U21 - working
	//Initializing the AtmoLite Command UART
		UART_init(&rs422_test_uart, COREUART2_BASE_ADDR, BAUD_VALUE_57600, (DATA_8_BITS | NO_PARITY));
		MSS_UART_init(&g_mss_uart1, MSS_UART_57600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
		//AtmoLite Read Protocol Version Command
		uint8_t atmolite_read_protocol_ver[] = {0x55};


		while(1){
		//Sending a particular UART command
		//MSS_UART_polled_tx(&g_mss_uart1,(const uint8_t *)&atmolite_read_protocol_ver,sizeof(atmolite_read_protocol_ver));

		UART_send(&rs422_test_uart,(const uint8_t *)&atmolite_read_protocol_ver,sizeof(atmolite_read_protocol_ver));
		}
//		uint8_t k = 0;
//
//		//Initialize RX Buffer to zero
//		while(k<MAX_RX_DATA_SIZE){
//			rx_data[k]=0;
//			k++;
//		}
//		//Get the Response from the ADCS
//		//Reading data from the Receive Buffer
//		rx_size = UART_get_rx(&rs422_test_uart, rx_data, sizeof(rx_data));
//
//		}
	//**************************************************************************************
	}
