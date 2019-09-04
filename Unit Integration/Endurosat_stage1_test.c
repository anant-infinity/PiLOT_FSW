UART_instance_t uhf_test_uart;
#define MAX_RX_DATA_SIZE 512

#define COREUART3_BASE_ADDR 	0x5000D000u
#define COREUART3_BAUD_VALUE 	BAUD_VALUE_115200

uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init


//*********************************************** EnduroSat UHF Test S1*************************************\\
	//UHF uses ArmCoreUart3 - This corresponds to TX_2 and RX_2
	//TX_2 - K17 - BB - P13 , Pin 22
	//RX_2 - B1 - BB - P14 , Pin 22
	//GND - BB - P7 , Pin 2
	//UHF Enable GPIO - GPIO_61 - K21 - Baseboard Pin 3 , P13

	//Initializing the EnduroSat UHF UART
	UART_init(&uhf_test_uart, COREUART3_BASE_ADDR,COREUART3_BAUD_VALUE, (DATA_8_BITS | NO_PARITY)); //Confirm Baud Rate

	//Enabling the UHF
	MSS_GPIO_set_output(MSS_GPIO_3, 1);

			//EnduroSat Reading the Control Word
			//Command: ES+R[AA]00<CR>

			//Test Command that Didn't work
			//ES- ESC , CR - 0x0D - 13 in Decimal , [AA] - 0x22 - 34 in decimal
			//uint8_t read_statuscontrolword[] = {27,43,82,34,0,0,13};
			//Alternatively ES could mean E in Ascii (decimal 69) and S in ASCII (Decimal 83)
			//uint8_t read_statuscontrolword[] = {69,83,43,82,34,0,0,13};
			//0 - Ascii zero - Decimal 48
			//uint8_t read_statuscontrolword[] = {69,83,43,82,34,48,48,13};

			//Test Command 4 - WORKING
			//0 - Ascii zero - Decimal 48, 2 - Ascii 2 - Decimal 50
			//uint8_t read_statuscontrolword[] = {69,83,43,82,50,50,48,48,13};

			//Test Command 5
			//Can try the command with CRC
			//Do not put Carraige return while calculating CRC
			//CRC calculator using the xHF application - 0xBD888E1F
			//command with CRC - ES+R[AA]00[B][CCCCCCCC]<CR>
			//THIS WORKED , but only able to see 16 Bytes of the Responce
			//uint8_t read_statuscontrolword_withCRC[] = {69,83,43,82,50,50,48,48,32,66,68,56,56,56,69,49,70,13};

			//All read commands without CRC
			//Command to read Status Control Word
			uint8_t read_statuscontrolword[] = {'E','S','+','R','2','2','0','0','\r'};

			//Command to read Radio frequency
			uint8_t read_radiofrequency[] = {'E','S','+','R','2','2','0','1','\r'};

			//Command Read UPTIME
			uint8_t read_uptime[] = {'E','S','+','R','2','2','0','2','\r'};

			//Command transmitted packets
			uint8_t read_transpackets[] = {'E','S','+','R','2','2','0','3','\r'};

			//Command Received packets
			uint8_t read_recvpackets[] = {'E','S','+','R','2','2','0','4','\r'};

			//Beacon Message Period
			uint8_t read_beacinperiod[] = {'E','S','+','R','2','2','0','7','\r'};

			//Destination Call Sign
			uint8_t read_destcallsign[] = {'E','S','+','R','2','2','F','5','\r'};

			//Source Call Sign
			uint8_t read_sourcecallsign[] = {'E','S','+','R','2','2','F','6','\r'};

			//Morse Code read
			//Worked but only with 16 bytes
			uint8_t read_morsecode[] = {'E','S','+','R','2','2','F','7','\r'};

			//Beacon Message
			//Works but is AX.25 encoded , Also is larger than 16 bytes.
			uint8_t read_beaconmsg[] = {'E','S','+','R','2','2','F','B','\r'};

			//Write Status Control Word
			//to set beacon - SCW - 0x33 , 0x43
			uint8_t write_statuscontrolword_BEAC[] = {'E','S','+','W','2','2','0','0','3','3','4','3','\r'};

			//Write Status Control Word
			//to DISABLE  beacon - SCW - 0x33 , 0x03
			uint8_t write_statuscontrolword_NOBEAC[] = {'E','S','+','W','2','2','0','0','3','3','0','3','\r'};
			//Write Beacon Message
			//ES+W22FB[LL][B…B]<CR>

			//LL is the number of bytes in BBBBB
			//For the test packet of 21 Bytes , LL is 0x10
			//[B…B] is the beacon message variable size content in ASCII format.

			//Adding the CCSDS Header
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


			// Command to Configure Beacon Message
			uint8_t write_beaconMessage[] = {'E','S','+','W','2','2','F','B','1','0', 0x08, 0x10 , 0x80 ,
					0x01 , 0x00, 0x0C , 0x00 , 0x00 , 0x00 ,0xFF, 0x00 , 0x01 , 0x00, 0x01, 0x0F, 0x0F,'\r'};

			// Read Beacon Content
			uint8_t read_beaconMessage[] = {'E','S','+','R','2','2','F','B','\r'};

			// Write Beacon Period
			uint8_t write_beaconperiod[] = {'E','S','+','W','2','2','0','7','0','0','0','0','0','0','0','1','\r'};


			while(1){
			UART_send(&uhf_test_uart,(const uint8_t *)&read_statuscontrolword,sizeof(read_statuscontrolword));
			//Get the Response from the UHF
			//Reading data from the Receive Buffer
			int k = 0;
			while(k<MAX_RX_DATA_SIZE){
				rx_data[k]=0;
				k++;
			}
			rx_size = UART_get_rx( &uhf_test_uart, rx_data, sizeof(rx_data));
			}