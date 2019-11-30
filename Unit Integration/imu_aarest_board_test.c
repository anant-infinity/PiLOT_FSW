#include "mss_i2c.h"
#include "mss_uart.h"
#include "core_i2c.h"
#include "core_uart_apb.h"
# include <stdio.h>
# include <string.h>

#define MAX_RX_DATA_SIZE 512
//change this according to the PiLOT FGPA project
#define BAUD_VALUE_9600 653
#define COREUART3_BASE_ADDR 	0x50000000u
#define COREUART3_BAUD_VALUE 	BAUD_VALUE_9600
#define TX_LENGTH 1u
#define RX_LENGTH 1u
#define MSS_I2C_DUMMY_ADDR 0x10u
UART_instance_t logging_uart;
uint8_t rx_data[MAX_RX_DATA_SIZE];
size_t rx_size = 0;

int main() {
	//Turn off the watchdog
	SYSREG->WDOG_CR = 0; //watchdog on/off is purposely separated from peripheral init

		//*********************************************** IMU Test S1**********************
					//MSS_I2C_0 Pins - SCL - G16 - Pin 8 of P9 ; SDA - G17 - Pin 9 of P9
					uint8_t rx_buffer[RX_LENGTH];
					uint8_t rx_buffer_xL[RX_LENGTH];
					uint8_t rx_buffer_xH[RX_LENGTH];
					uint8_t rx_buffer_yL[RX_LENGTH];
					uint8_t rx_buffer_yH[RX_LENGTH];
					uint8_t rx_buffer_zL[RX_LENGTH];
					uint8_t rx_buffer_zH[RX_LENGTH];

					uint8_t read_length = RX_LENGTH;

					uint8_t write_length = TX_LENGTH;

					// For AAReST OBC - MAG Address - 0x1C
					// For AAReST OBC - A&G Address - 0x6A
					uint8_t IMU_slave_addr = 0x1C;


					//Magnetometer Registers
					//uint8_t read_who_am_I[TX_LENGTH] = {0x0F};
					//uint8_t read_CTRL_REG3_M[TX_LENGTH] = {0x22};
					//uint8_t read_CTRL_REG2_M[TX_LENGTH] = {0x21};
					uint8_t write_CTRL_REG3_M[TX_LENGTH+1] = {0x22, 0x00};
					uint8_t read_mag_out_X_L[TX_LENGTH] = {0x28};
					uint8_t read_mag_out_X_H[TX_LENGTH] = {0x29};
					uint8_t read_mag_out_Y_L[TX_LENGTH] = {0x2A};
					uint8_t read_mag_out_Y_H[TX_LENGTH] = {0x2B};
					uint8_t read_mag_out_Z_L[TX_LENGTH] = {0x2C};
					uint8_t read_mag_out_Z_H[TX_LENGTH] = {0x2D};



					//Accelorometer Registers
					//uint8_t read_OUT_X_L_XL[TX_LENGTH] = {0x28};
					//uint8_t read_OUT_X_H_XL[TX_LENGTH] = {0x29};

					//Add the Register Address here
					//CTRL_REG_G - 0x10
					//MAG - CTLR Reg 1 - 0x20
					//uint8_t tx_buffer[TX_LENGTH] = {0x0F};
					//uint8_t tx_buffer[TX_LENGTH+1] = {0x20,0xC0};
					//uint8_t read__CNTR[TX_LENGTH] = {0x20};
					//uint8_t tx_read[TX_LENGTH] = {0x22};
					//uint8_t read_mag_data[TX_LENGTH-1] = {0x29};


					mss_i2c_status_t status;

					// Initialize MSS I2C peripheral
					MSS_I2C_init( &g_mss_i2c0, MSS_I2C_DUMMY_ADDR, MSS_I2C_PCLK_DIV_960 );

					//Initialize Uart Peripheral for data logging
					UART_init(&logging_uart, COREUART3_BASE_ADDR,COREUART3_BAUD_VALUE, (DATA_8_BITS | NO_PARITY));

					while(1){


					// reading a control register
					//MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_CTRL_REG2_M, write_length, rx_buffer, read_length, MSS_I2C_RELEASE_BUS );
					//status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );

					// Writing to control register to modify to continous conversion mode
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, write_CTRL_REG3_M, write_length+1, rx_buffer, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );

					// Read X LSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_X_L, write_length, rx_buffer_xL, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
					// Read X MSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_X_H, write_length, rx_buffer_xH, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
					// Read Y LSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_Y_L, write_length, rx_buffer_yL, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
					// Read Y MSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_Y_H, write_length, rx_buffer_yH, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
					// Read Z LSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_Z_L, write_length, rx_buffer_zL, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );
					// Read Z MSB
					MSS_I2C_write_read(&g_mss_i2c0, IMU_slave_addr, read_mag_out_Z_H, write_length, rx_buffer_zH, read_length, MSS_I2C_RELEASE_BUS );
					status = MSS_I2C_wait_complete( &g_mss_i2c0, MSS_I2C_NO_TIMEOUT );

					uint16_t xvalue = 0x0000;
					uint16_t yvalue = 0x0000;
					uint16_t zvalue = 0x0000;

					// Concatenate Lower and Higher Bytes 
					// Order of bytes changed because this is little to big endian converssion

					xvalue = rx_buffer_xL[0];  // Insert the High byte into the lower byte of tvalue
					xvalue <<= 8;  // Left shift the lower byte into the higher byte of tvalue
					xvalue += rx_buffer_xH[0]; // Insert the Low byte into the lower byte of tvalue

					// Concatenate Lower and Higher Bytes
					yvalue = rx_buffer_yL[0];  // Insert the High byte into the lower byte of tvalue
					yvalue <<= 8;  // Left shift the lower byte into the higher byte of tvalue
					yvalue += rx_buffer_yH[0]; // Insert the Low byte into the lower byte of tvalue

					// Concatenate Lower and Higher Bytes
					zvalue = rx_buffer_zL[0];  // Insert the High byte into the lower byte of tvalue
					zvalue <<= 8;  // Left shift the lower byte into the higher byte of tvalue
					zvalue += rx_buffer_zH[0]; // Insert the Low byte into the lower byte of tvalue


					UART_send(&logging_uart,(const uint16_t *)&xvalue,sizeof(xvalue));
					UART_send(&logging_uart,(const uint16_t *)&yvalue,sizeof(yvalue));
					UART_send(&logging_uart,(const uint16_t *)&zvalue,sizeof(zvalue));




				}

			//**************************************************************************************
	}

