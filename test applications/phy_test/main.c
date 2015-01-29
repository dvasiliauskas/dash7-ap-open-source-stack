/*
 *  Created on: Nov 22, 2012
 *  Authors:
 * 		maarten.weyn@artesis.be
 *  	glenn.ergeerts@artesis.be
 *  	alexanderhoet@gmail.com
 */

#include <string.h>
#include <stdio.h>

#include <d7aoss.h>

#include <hal/button.h>
#include <hal/leds.h>
#include <hal/rtc.h>
#include <hal/system.h>
#include <framework/log.h>

#define RX_MODE

static uint8_t tx_data[16] = {16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

/*
 */
phy_tx_cfg_t tx_cfg = {
    .spectrum_id={ 0x00, 0x00}, // TODO
	.sync_word_class=1,
    .eirp=0
};

phy_rx_cfg_t rx_cfg = {
	.spectrum_id={ 0x00, 0x00}, // TODO
	.sync_word_class=1,
	.length=0,
	.timeout=1000,
	.scan_minimum_energy = -140
};

void start_rx(void)
{
	phy_rx(&rx_cfg);
}

void start_tx(void)
{
	phy_idle();
	phy_tx(&tx_cfg);
}

void rx_callback(phy_rx_data_t* rx_data)
{
//	if(memcmp(buffer, rx_data->data, sizeof(buffer)) == 0)
	led_toggle(1);
	start_rx();
}

uint8_t tx_buffer[128];
uint8_t rx_buffer[128];


int main(void)
{
    system_init(tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));

	phy_init();
	phy_set_rx_callback(rx_callback);

	#ifdef RX_MODE
		start_rx();
		system_lowpower_mode(4,1);
	#else
		queue_push_u8_array(&tx_queue, tx_data, sizeof(tx_data));
		start_tx();
	#endif

	return 0;
}

