#define TX_LENGTH 8u
#define RX_LENGTH 72u

int main() {
		//Turn off the watchdog
		SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

//*********************************************** Thruster Test S1**********************
		//MSS_I2C_0 Pins - SCL - G16 - Pin 8 of P9 ; SDA - G17 - Pin 9 of P9
		uint8_t rx_buffer[RX_LENGTH];
		uint8_t read_length = RX_LENGTH;

		//Check the slave Addresses 
		//IMU - A&G - Slave Address(R) - 0xD5 or 0xD7
		//IMU - M - Slave Address(R) - 0x39 or 0x3D 
		uint8_t IMU_slave_addr = 0xD5;


		//Add the Register Address here 
		//CTRL_REG_G - 0x10 
		//MAG - CTLR Reg 1 - 0x20
		uint8_t tx_buffer[TX_LENGTH] = {0x10};


		uint8_t write_length = TX_LENGTH;
		i2c_instance_t IMU_test_i2c;


		mss_i2c_status_t status;

		// Initialize MSS I2C peripheral
		MSS_I2C_init( &g_mss_i2c0, IMU_slave_addr, I2C_PCLK_DIV_256 );


		while(1){
		uint8_t k = 0;
		//Initialize the RX_Buffer to 0
		while(k<RX_LENGTH){
			rx_buffer[k]=0;
			k++;
		}
		// Read data from Channel 0 of CoreI2C instance.
		MSS_I2C_write( &g_mss_i2c0, IMU_slave_addr, tx_buffer, write_length,MSS_I2C_RELEASE_BUS );

		MSS_I2C_read( &g_mss_i2c0, IMU_slave_addr, rx_buffer, read_length, MSS_I2C_RELEASE_BUS );
		status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );

		//I2C_write_read( &IMU_test_i2c, IMU_slave_addr, tx_buffer, 
		//write_length,rx_buffer, read_length, I2C_RELEASE_BUS );
		status = I2C_wait_complete( &IMU_test_i2c, I2C_NO_TIMOUT );
	}

//**************************************************************************************
}