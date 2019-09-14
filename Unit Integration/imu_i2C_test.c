#include "main_init.c" //so that the main code will not be too long just for declaration
#define MAX_RX_DATA_SIZE 512
#define TX_LENGTH 1u
#define RX_LENGTH 1u
#define MSS_I2C_DUMMY_ADDR 0x10u

uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

//*********************************************** IMU Test S1**********************
			//MSS_I2C_0 Pins - SCL - G16 - Pin 8 of P9 ; SDA - G17 - Pin 9 of P9
			uint8_t rx_buffer[RX_LENGTH];
			uint8_t read_length = RX_LENGTH;

			//Check the slave Addresses
			//IMU - A&G - Slave Address(R) - 0xD5 or 0xD7 - NOTE - these are 8 bit
			//IMU - M - Slave Address(R) - 0x39 or 0x3D - NOTE - these are 8 bit
			//CORRECT - 7BIT - READ and WRITE - MAG Address - 0x1E
			//CORRECT - 7BIT READ and WRITE - A&G Address - 0x6B
			uint8_t IMU_slave_addr = 0x1E;


			//Add the Register Address here
			//CTRL_REG_G - 0x10
			//MAG - CTLR Reg 1 - 0x20
			uint8_t tx_buffer[TX_LENGTH] = {0x29};


			uint8_t write_length = TX_LENGTH;

			mss_i2c_status_t status;

			// Initialize MSS I2C peripheral
			MSS_I2C_init( &g_mss_i2c0, MSS_I2C_DUMMY_ADDR, MSS_I2C_PCLK_DIV_960 );


			while(1){
			uint8_t k = 0;
			uint16_t m = 0;
			uint16_t n = 0;
			//Initialize the RX_Buffer to 0

			// Read data from Channel 0 of CoreI2C instance.
			//MSS_I2C_write( &g_mss_i2c0, IMU_slave_addr, tx_buffer, write_length,MSS_I2C_RELEASE_BUS );
			while(k<RX_LENGTH){
				rx_buffer[k]=0;
				k++;
			}
			//MSS_I2C_read( &g_mss_i2c0, IMU_slave_addr, rx_buffer, read_length, MSS_I2C_RELEASE_BUS );
			//status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );

			MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, tx_buffer,write_length,rx_buffer, read_length, MSS_I2C_RELEASE_BUS );
			status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
		}

	//**************************************************************************************
	}