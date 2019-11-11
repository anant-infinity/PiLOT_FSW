#include "mss_i2c.h"
#include "mss_uart.h"
#include "core_i2c.h"
#include "core_uart_apb.h"
#define MAX_RX_DATA_SIZE 512
//change this according to the PiLOT FGPA project
#define BAUD_VALUE_9600 653
#define COREUART3_BASE_ADDR 	0x50001000u
#define COREUART3_BAUD_VALUE 	BAUD_VALUE_9600
UART_instance_t uhf_test_uart;
uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

//*********************************************** PiLOT UHF Test S1*************************************\\
	//Change this according to the PiLOT FPGA project

	//UHF uses ArmCoreUart3 - This corresponds to TX_2 and RX_2
	//TX_2 - K17 - BB - P13 , Pin 22
	//RX_2 - B1 - BB - P14 , Pin 22
	//GND - BB - P7 , Pin 2


	//Initializing the UHF UART
	//Change the baud rate according to the Baud Rate of the PiLOT UHF module
	UART_init(&uhf_test_uart, COREUART3_BASE_ADDR,COREUART3_BAUD_VALUE, (DATA_8_BITS | NO_PARITY));
	//MSS_UART - if needed
	//MSS_UART_init( &g_mss_uart0,MSS_UART_57600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |MSS_UART_ONE_STOP_BIT );

	//All read commands without CRC
	//Command to read Status Control Word
	uint8_t beacon_test_command[] = {'!','P','i','L','O','T','T','T','C','i','s','b','e','t','t','e','r','t','h','a','n','a','i','r','t','e','l','\r'};
	//uint8_t beacon_test_command[] = {'!','P','i','L','O','T','T','T','C','i','s','b','e','t','t','e','r','t','h','a','n','a','i','r','t','e','l','\r'};
	//uint8_t beacon_test_command[] = {'!','','e','l','l','o','w','o','r','l','d','\r'};
	//Id we want to implement CCSDS protocol in the PiLOT mission
	//Some information about the CCSDS Header
	//Packet structure is: (0000 1000) (APP_ID) (GGSS SSSS)(SSSS SSSS) (DDDD DDDD)(DDDD DDDD)(TIME Stamp 5 bytes)(Data)
	//(0000 1000) the 5th bit is the secondary header flag, 1 if TRUE
	//GG - grouping Flag - 10 or 11
	//SS SSSS SSSS SSSS - 00 0000 0000 0001
	//DDDD DDDD DDDD DDDD - (8+5-1 = 12) - 0000 0000 0000 1100
	//Secondary Header - 5 Byte dummy time stamp - 0x00 , 0x00 , 0x00 ,0xFF, 0x00
	//Main Data: 1 2 3
	// 0x08, 0x10 , 0x80 , 0x01 , 0x00, 0x0C , SEC HED , 0x01 , 0x02, 0x03
	//uint8_t write_beaconMessage[] = {'E','S','+','W','2','2','F','B','1','5',/**/,'\r'};
	//Add Fletcher Code Here - 2Bytes - Dummy Fletcher - 0x0F 0x0F

	while(1){
		//Send the command to the UHF radio
		UART_send(&uhf_test_uart,(const uint8_t *)&beacon_test_command,sizeof(beacon_test_command));

		//Initialize the RX_BUFFER to have all zeros
		int k = 0;
		while(k<MAX_RX_DATA_SIZE){
			rx_data[k]=0;
			k++;
		}

		//Reading data from the Receive Buffer
		//rx_size = UART_get_rx( &g_mss_uart0, rx_data, sizeof(rx_data));
	}
}