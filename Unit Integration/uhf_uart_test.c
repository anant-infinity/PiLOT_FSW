UART_instance_t uhf_test_uart;
#define MAX_RX_DATA_SIZE 512

//change this according to the PiLOT FGPA prject 
#define COREUART3_BASE_ADDR 	0x5000D000u
#define COREUART3_BAUD_VALUE 	BAUD_VALUE_115200

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
	UART_init(&uhf_test_uart, COREUART3_BASE_ADDR,COREUART3_BAUD_VALUE, (DATA_8_BITS | NO_PARITY)); //Confirm Baud Rate


	//All read commands without CRC
	//Command to read Status Control Word
	uint8_t read_test_command[] = {'E','S','+','R','2','2','0','0','\r'};

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
		UART_send(&uhf_test_uart,(const uint8_t *)&read_test_command,sizeof(read_test_command));
		
		//Initialize the RX_BUFFER to have all zeros
		int k = 0;
		while(k<MAX_RX_DATA_SIZE){
			rx_data[k]=0;
			k++;
		}

		//Reading data from the Receive Buffer
		rx_size = UART_get_rx( &uhf_test_uart, rx_data, sizeof(rx_data));
	}