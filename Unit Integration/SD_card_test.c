#include "mss_i2c.h" //so that the main code will not be too long just for declaration
#include "core_uart_apb.h"
#include "mss_uart.h"
#include "mss_spi.h"
#include "mss_gpio.h"

#define COREUART0_BASE_ADDR 	0x50000000u
#define COREUART1_BASE_ADDR 	0x50001000u
#define COREUART2_BASE_ADDR 	0x50002000u

// MSS_GPIO_10 is the SD Card Enable GPIO

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
mss_spi_instance_t * SD0_SPI = &g_mss_spi1;

//uint8_t SD_Init(){
//	MSS_SPI_init(SD0_SPI);
//	MSS_SPI_configure_master_mode(SD0_SPI, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 512, 8);	//the clock divider has to be fixed after testing in harsh env
//                                                                                    //100Mhz/255~392k
//	uint8_t rx_buffer[1];
//	//uint8_t CMD0[] = {0x40, 0x00,0x00, 0x00,0x00, 0x95,0xff};
//	uint8_t CMD0[] = {0x55};
//	uint32_t i;
//
//
//	for(i =0; i<10; i++){
//		MSS_SPI_transfer_frame(SD0_SPI, 0xff);
//	}
//	i = 0;
//	MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//	do{		// add counter for number of trying
//
//		//MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//		//MSS_GPIO_set_outputs( 0x00);
//		MSS_SPI_transfer_block(SD0_SPI, CMD0, 7, rx_buffer, 1);  //checked
//		//MSS_SPI_clear_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//		//MSS_GPIO_set_outputs( 0x01);
//		i += 1;
//	}while(rx_buffer[0]!=1 && i != 255);// rx_buffer[0]!=1  or  i!=255 //sometimes CMD0 failed
//
//	if(i == 255){
//		return 0;
//	}else{
//		i = 0;
//	}
//	uint8_t rx_b[5];
//	//rx_buffer[0] = 1;    //????????????????????????????????????????????????????????????????????
//	uint8_t CMD8[] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87, 0xff};	// last 0xff is use to give sd card buffer clock
//	uint8_t flag = 0;
//	do{	// add counter for number of trying
//		//MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//		//MSS_GPIO_set_outputs( 0x00);
//		MSS_SPI_transfer_block(SD0_SPI, CMD8, 7, rx_b, 5);             //checked ,return 0X1AA
//		//MSS_SPI_clear_slave_select(SD0_SPI, MSS_SPI_SLAVE_0); //  0x1AA means that the card is SDC version 2 and it can work at voltage range of 2.7 to 3.6 volts
//		//MSS_GPIO_set_outputs( 0x01);
//
//		i = i + 1;
//
//		if(rx_b[4] == 0xaa && rx_b[3] == 0x01 && rx_b[2] == 0x00 && rx_b[1] == 0x00)
//		{
//			rx_buffer[0] = 1;
//			i = 0;
//			flag = 1;
//			break;
//		}
//	}while(rx_buffer[0] && i!=255);
//	//uint8_t j=0;
//
//	if (flag == 1)
//	{
//		// sending CMD55 and then ACMD41 to get it out  of idle mode.
//		uint8_t CMD55[] = {0x77, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};	// last 0xff is use to give sd card buffer clock
//		uint8_t ACMD41[] = {0x69, 0x40, 0x00, 0x00, 0x00, 0xFF, 0xFF};	// last 0xff is use to give sd card buffer clockACMD41[] = {0x69, 0x41, 0x00, 0x00, 0x00, 0xFF,0xFF}
//		//uint8_t SCMD[] = {0x77 , 0x00 , 0x00, 0x00, 0x00 , 0xFF, 0xFF};
//		do{
//				i = i+1;
//				//MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//				//MSS_GPIO_set_outputs( 0x00);
//				MSS_SPI_transfer_block(SD0_SPI, CMD55, 7, rx_buffer, 1);
//				MSS_SPI_transfer_frame(SD0_SPI, 0xFF);
//				MSS_SPI_transfer_block(SD0_SPI, ACMD41, 7, rx_buffer, 1);
//				MSS_SPI_transfer_frame(SD0_SPI, 0xFF);
//
//
//				//MSS_GPIO_set_outputs( 0x01);
//				//MSS_SPI_clear_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//
//				//MSS_GPIO_set_outputs( 0x01);
//				//for(j=0; j<30; j++){
//				//	MSS_SPI_transfer_frame(SD0_SPI, 0xff);
//				//}
//
//
//				//MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//				//MSS_GPIO_set_outputs( 0x00);
//				//MSS_GPIO_set_outputs( 0x00);
//
//				//MSS_SPI_clear_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
//				//for(j=0; j<24; j++){
//				//	MSS_SPI_transfer_frame(SD0_SPI, 0xff);
//				//}
//
//				//MSS_GPIO_set_outputs( 0x01);
//			}while(rx_buffer[0] && i!=50000); //any of them is 0 (i=4 or rx_buffer[0]=0), get out of the loop //stucked sometimes
//	}
//	return 1;
//}

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

	MSS_GPIO_init();
	MSS_GPIO_config( MSS_GPIO_10 , MSS_GPIO_OUTPUT_MODE );
	//Enable the SD Card
	MSS_GPIO_set_output( MSS_GPIO_10, 1);

	MSS_SPI_init(SD0_SPI);
	MSS_SPI_configure_master_mode(SD0_SPI, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 512, 8);	//the clock divider has to be fixed after testing in harsh env
	                                                                                    //100Mhz/255~392k
		uint8_t rx_buffer[1];
		//uint8_t CMD0[] = {0x40, 0x00,0x00, 0x00,0x00, 0x95,0xff};
		uint8_t CMD0[] = {0x55};
		uint32_t i;


		for(i =0; i<10; i++){
			MSS_SPI_transfer_frame(SD0_SPI, 0xff);
		}
		i = 0;
		MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
		do{		// add counter for number of trying

			//MSS_SPI_set_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
			//MSS_GPIO_set_outputs( 0x00);
			MSS_SPI_transfer_block(SD0_SPI, CMD0, 7, rx_buffer, 1);  //checked
			//MSS_SPI_clear_slave_select(SD0_SPI, MSS_SPI_SLAVE_0);
			//MSS_GPIO_set_outputs( 0x01);
			i += 1;
		}while(rx_buffer[0]!=1 && i != 255);// rx_buffer[0]!=1  or  i!=255 //sometimes CMD0 failed

//		while(1){
//		//Sending a particular UART command
//		//MSS_UART_polled_tx(&g_mss_uart1,(const uint8_t *)&atmolite_read_protocol_ver,sizeof(atmolite_read_protocol_ver));
//		//Enable the SD Card
//		MSS_GPIO_set_output( MSS_GPIO_10, 1);
//
//		uint8_t stat = SD_Init();
//		//MSS_GPIO_set_output( MSS_GPIO_10, 0);
//
//		//UART_send(&rs422_test_uart,(const uint8_t *)&atmolite_read_protocol_ver,sizeof(atmolite_read_protocol_ver));
//		}
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
