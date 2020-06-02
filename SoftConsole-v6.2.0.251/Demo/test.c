/*******************************************************************************
 * (c) Copyright 2008-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * This example project demonstrates communications with an external flash
 * device.
 *
 * Please refer to the file README.txt for further details about this example.
 *
 * SVN $Revision: 5510 $
 * SVN $Date: 2013-03-29 18:29:47 +0000 (Fri, 29 Mar 2013) $
 */

#include "firmware/drivers/mss_spi/mss_spi.h"
#include "winbondflash.h"
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/drivers/mss_sys_services/mss_sys_services.h"
#include "firmware/CMSIS/system_m2sxxx.h"

#define BUFFER_A_SIZE  5

/* Manufactuer and device IDs for SPI Flash. */
/*
#define FLASH_MANUFACTURER_ID   (uint8_t)0x1F
#define FLASH_DEVICE_ID         (uint8_t)0x48
 */
/*
 * Static global variables
 */

static uint8_t g_flash_wr_buf[BUFFER_A_SIZE];
static uint8_t g_flash_rd_buf[BUFFER_A_SIZE];

static const mss_gpio_id_t LED[4] = {
		MSS_GPIO_2, MSS_GPIO_3,
		MSS_GPIO_4, MSS_GPIO_8,
};

void Flash_write();
void Flash_read();
int scannumber();
void Init();
size_t UART_Polled_Rx
(
		mss_uart_instance_t * this_uart,
		uint8_t * rx_buff,
		size_t buff_size
);
uint32_t address = 0;
uint16_t loop_count;
uint8_t ch;
uint8_t i;
/*==============================================================================
  Messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
		"\r\n\r\n\
**********************************************************************\r\n\
***************** SmartFusion2 MSS SPI Flash example *****************\r\n\
**********************************************************************\r\n\
This example project demonstrates the use of the SmartFusion2 MSS SPI\r\n\
block.It reads and writes the content of an external SPI flash device\r\n\
---------------------------------------------------------------------\r\n";

void delay() {
	int d = SystemCoreClock / 128;
	while (d-- > 0)
		;
}

/***************************************************************************//**
 * main function.
 */
int main()
{
	volatile uint8_t errors = 0;
	uint8_t buff[10];
	uint8_t man_id, dev_id, dev_type;


	MSS_UART_init(&g_mss_uart0,
			MSS_UART_115200_BAUD,
			MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_polled_tx_string(&g_mss_uart0, g_greeting_msg);

	MSS_GPIO_init();
	/*--------------------------------------------------------------------------
	 * Flash Driver Initialization
	 */
	FLASH_init();

	FLASH_global_unprotect();

	/*--------------------------------------------------------------------------
	 * Check SPI Flash part manufacturer and device ID.*/
	FLASH_read_device_id(&man_id, &dev_type, &dev_id);
	itoa(man_id, buff, 16);
	MSS_UART_polled_tx_string(&g_mss_uart0, "\r\nManuf ID:");
	MSS_UART_polled_tx_string(&g_mss_uart0, buff);

	memset(buff, 0, sizeof(buff));
	itoa(dev_type, buff, 16);
	MSS_UART_polled_tx_string(&g_mss_uart0, "\tDev type:");
	MSS_UART_polled_tx_string(&g_mss_uart0, buff);

	memset(buff, 0, sizeof(buff));
	itoa(dev_id, buff, 16);
	MSS_UART_polled_tx_string(&g_mss_uart0, "\tDev ID:");
	MSS_UART_polled_tx_string(&g_mss_uart0, buff);
	memset(buff, 0, sizeof(buff));

	for (i = 0; i < 4; i++) {
		MSS_GPIO_config(LED[i], MSS_GPIO_OUTPUT_MODE);
	}
	while(1)
	{
		Init();
	}
	return 0;
}
/*--------------------------------------------------------------------------
 * Write Data to Flash.
 */

void Init()
{
	MSS_UART_polled_tx_string(&g_mss_uart0,(const uint8_t*)"\n\rEnter your choice and press Enter\n\r 1.Write \n\r 2.Read \n\r " );
	ch = scannumber();

	if(ch==1)
		Flash_write();
	else if(ch==2)
		Flash_read();
	else
		MSS_UART_polled_tx_string( &g_mss_uart0,(const uint8_t*)"Enter correct choice\n\r" );
}
void Flash_write()
{
	address = 0;
	for(loop_count = 0; loop_count <(BUFFER_A_SIZE); loop_count++)
	{
		g_flash_wr_buf[loop_count] = 0;
	}
//	FLASH_erase_64k_block(0);
	FLASH_chip_erase();
	MSS_UART_polled_tx_string(&g_mss_uart0, "Enter any 5 values between 0 to 99 to write to Flash Memory \n\r\n\r");

	for(i=0;i<5;i++)
	{

		g_flash_wr_buf[i] = scannumber();
		while(g_flash_wr_buf[i]<0 || g_flash_wr_buf[i]> 99 )
		{
			MSS_UART_polled_tx_string( &g_mss_uart0,(const uint8_t*)"Enter only 2 digit number\n\r" );
			Init();
		}
	}
	FLASH_program(address, g_flash_wr_buf, sizeof(g_flash_wr_buf));

}
/*--------------------------------------------------------------------------
 * Read Data From Flash.
 */
void Flash_read()
{
	address = 0;
	for(loop_count = 0; loop_count <(BUFFER_A_SIZE); loop_count++)
	{
		g_flash_rd_buf[loop_count] = 0;
	}
	FLASH_read(address, g_flash_rd_buf, sizeof(g_flash_wr_buf));
	MSS_UART_polled_tx_string(&g_mss_uart0, " \n\rRead Data From Flash: ");
//	for(i=0;i<(BUFFER_A_SIZE);i++)

		MSS_UART_polled_tx_string(&g_mss_uart0, g_flash_rd_buf);


	MSS_UART_polled_tx_string(&g_mss_uart0, " \n\rRead operation is completed \n\r ");
}


size_t
UART_Polled_Rx
(
		mss_uart_instance_t * this_uart,
		uint8_t * rx_buff,
		size_t buff_size
)
{
	size_t rx_size = 0U;

	while( rx_size < buff_size )
	{
		while ( ((this_uart->hw_reg->LSR) & 0x1) != 0U  )
		{
			rx_buff[rx_size] = this_uart->hw_reg->RBR;
			++rx_size;
		}
	}

	return rx_size;
}
int scannumber()
{
	uint8_t rxbuff,x[8]={0},i=0;//[8];
	mss_uart_instance_t * this_uart = &g_mss_uart0;
	int num;
	while(i<5)
	{
		UART_Polled_Rx( &g_mss_uart0, &rxbuff, sizeof(rxbuff) );

		if(rxbuff == '\n')
		{
			rxbuff= this_uart->hw_reg->RBR;
			rxbuff= this_uart->hw_reg->RBR;
			break;
		}
		else
			x[i++]=rxbuff;
	}
	num = atoi(x);
	return num;
}


