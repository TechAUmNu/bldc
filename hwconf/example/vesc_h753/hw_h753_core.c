/*
	Copyright 2022 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "hw.h"

#include "ch.h"
#include "hal.h"
#include "utils_math.h"
#include "mc_interface.h"
#include "terminal.h"
#include "commands.h"

#include <math.h>

// Variables
static volatile bool i2c_running = false;
static mutex_t shutdown_mutex;
static float bt_diff = 0.0;

// I2C configuration
//static const I2CConfig i2cfg = {
//		OPMODE_I2C,
//		100000,
//		STD_DUTY_CYCLE
//};

static const I2CConfig i2cfg = {
  .timingr          = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) |
                      STM32_TIMINGR_SDADEL(2U) | STM32_TIMINGR_SCLH(15U) |
                      STM32_TIMINGR_SCLL(21U),
  .cr1              = 0,
  .cr2              = 0
};

// Private functions
static void terminal_shutdown_now(int argc, const char **argv);
static void terminal_button_test(int argc, const char **argv);

void hw_init_gpio(void) {
	chMtxObjectInit(&shutdown_mutex);

	// LEDs
//	palSetPadMode(LED_GREEN_GPIO, LED_GREEN_PIN,
//			PAL_MODE_OUTPUT_PUSHPULL |
//			PAL_STM32_OSPEED_HIGHEST);
//	palSetPadMode(LED_RED_GPIO, LED_RED_PIN,
//			PAL_MODE_OUTPUT_PUSHPULL |
//			PAL_STM32_OSPEED_HIGHEST);


	// Test pin for debugging
	palSetPadMode(PIN_TEST_GPIO, PIN_TEST_PIN,
				PAL_MODE_OUTPUT_PUSHPULL |
				PAL_STM32_OSPEED_HIGHEST);
	PIN_TEST_ON();



	// GPIOA Configuration: Channel 1 to 3 as alternate function push-pull
//	palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(GPIO_AF_TIM1) |
//			PAL_STM32_OSPEED_HIGHEST |
//			PAL_STM32_PUPDR_FLOATING);
//	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(GPIO_AF_TIM1) |
//			PAL_STM32_OSPEED_HIGHEST |
//			PAL_STM32_PUPDR_FLOATING);
//	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(GPIO_AF_TIM1) |
//			PAL_STM32_OSPEED_HIGHEST |
//			PAL_STM32_PUPDR_FLOATING);

//	INIT_BR();

	// Hall sensors
//	palSetPadMode(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1, PAL_MODE_INPUT_PULLUP);
//	palSetPadMode(HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2, PAL_MODE_INPUT_PULLUP);
//	palSetPadMode(HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3, PAL_MODE_INPUT_PULLUP);

	// Phase filters
//	palSetPadMode(PHASE_FILTER_GPIO, PHASE_FILTER_PIN,
//			PAL_MODE_OUTPUT_PUSHPULL |
//			PAL_STM32_OSPEED_HIGHEST);
//	PHASE_FILTER_OFF();

	// Current filter
//	palSetPadMode(CURRENT_FILTER_GPIO, CURRENT_FILTER_PIN,
//			PAL_MODE_OUTPUT_PUSHPULL |
//			PAL_STM32_OSPEED_HIGHEST);

//	CURRENT_FILTER_OFF();

	// Sensor port voltage
//	SENSOR_PORT_3V3();
//	palSetPadMode(SENSOR_VOLTAGE_GPIO, SENSOR_VOLTAGE_PIN,
//			PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);

	// ZCD-pin
//	palSetPadMode(ZCD_GPIO, ZCD_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
//	palClearPad(ZCD_GPIO, ZCD_PIN);

	// CAN_EN-pin
//	palSetPadMode(CAN_EN_GPIO, CAN_EN_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
//	palClearPad(CAN_EN_GPIO, CAN_EN_PIN);

	// ADC Pins
//	palSetPadMode(GPIOA, 0, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_ANALOG);
//
//	palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_ANALOG);
//
//	palSetPadMode(GPIOB, 0, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOB, 1, PAL_MODE_INPUT_ANALOG);
//
//	palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOC, 2, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOC, 3, PAL_MODE_INPUT_ANALOG);
//	palSetPadMode(GPIOC, 4, PAL_MODE_INPUT_ANALOG);
//
//	// DAC as voltage reference for shunt amps
//	palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
//	rccEnableDAC1(TRUE);
//	DAC1->CR |= DAC_CR_EN1;
//	DAC1->DHR12R1 = 2047;
}

// TODO EM: need to work out the correct sample times
void hw_setup_adc_channels(void) {
	palSetPadMode(GPIOB, 1, PAL_MODE_INPUT_ANALOG);      // PB1 - IN5 - SENS1 		- FAST - Reacts to PB1
	palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_ANALOG);      // PA7 - IN7 - CURR1 		- SLOW - Reacts to PA7
	palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_ANALOG);      // PA6 - IN3 - EXT1 		- FAST - Reacts to PA6
	palSetPadMode(GPIOC, 5, PAL_MODE_INPUT_ANALOG);      // PC5 - IN8 - TEMP_MOTOR	- SLOW - Reacts to PC5

	palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_ANALOG);      // PA2 - IN14 - SENS2		- SLOW - Reacts to PA2
	palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_ANALOG);      // PA3 - IN15 - CURR2		- SLOW - Reacts to PA3
	palSetPadMode(GPIOB, 0, PAL_MODE_INPUT_ANALOG);      // PB0 - IN9 - TEMP_MOS	- SLOW - Reacts to ?
	palSetPadMode(GPIOC, 4, PAL_MODE_INPUT_ANALOG);      // PC4 - IN4 - EXT2 		- FAST - Reacts to PC4

	palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG);      // PC0 - IN10 - SENS3		- SLOW - Reacts to PC0
	palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG);      // PC1 - IN11 - CURR3		- SLOW - Reacts to PC1
	palSetPadMode(GPIOC, 2, PAL_MODE_INPUT_ANALOG);      // PC2 - IN0 - VIN			- FAST - Reacts to PC2
	palSetPadMode(GPIOC, 3, PAL_MODE_INPUT_ANALOG);      // PC3 - IN1 - EXT3		- FAST - Reacts to PC3

	// ADC1
	hw_setup_adc_channel_helper(ADC1, 5, 1, ADC_SMPR_SMP_16P5); // SENS1
	ADC1->PCSEL |= ADC_PCSEL_PCSEL_5;
	hw_setup_adc_channel_helper(ADC1, 7, 2, ADC_SMPR_SMP_16P5); // CURR1
	ADC1->PCSEL |= ADC_PCSEL_PCSEL_7;
	hw_setup_adc_channel_helper(ADC1, 8, 3, ADC_SMPR_SMP_16P5); // TEMP MOT
	ADC1->PCSEL |= ADC_PCSEL_PCSEL_8;
	hw_setup_adc_channel_helper(ADC1, 3, 4, ADC_SMPR_SMP_16P5); // EXT
	ADC1->PCSEL |= ADC_PCSEL_PCSEL_3;


	// ADC2
	hw_setup_adc_channel_helper(ADC2, 14, 1, ADC_SMPR_SMP_16P5); // SENS2
	ADC2->PCSEL |= ADC_PCSEL_PCSEL_14;
	hw_setup_adc_channel_helper(ADC2, 15, 2, ADC_SMPR_SMP_16P5); // CURR2
	ADC2->PCSEL |= ADC_PCSEL_PCSEL_15;
	hw_setup_adc_channel_helper(ADC2, 9, 3, ADC_SMPR_SMP_16P5); // TEMP MOS
	ADC2->PCSEL |= ADC_PCSEL_PCSEL_9;
	hw_setup_adc_channel_helper(ADC2, 4, 4, ADC_SMPR_SMP_16P5); // EXT2
	ADC2->PCSEL |= ADC_PCSEL_PCSEL_4;


	// ADC3
	hw_setup_adc_channel_helper(ADC3, 10, 1, ADC_SMPR_SMP_16P5); // SENS3
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_10;
	hw_setup_adc_channel_helper(ADC3, 11, 2, ADC_SMPR_SMP_16P5); // CURR3
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_11;
	hw_setup_adc_channel_helper(ADC3, 0, 3, ADC_SMPR_SMP_16P5); // VIN
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_0;
	hw_setup_adc_channel_helper(ADC3, 1, 4, ADC_SMPR_SMP_16P5); // EXT3
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_1;
}

void hw_start_i2c(void) {
	i2cAcquireBus(&HW_I2C_DEV);

	if (!i2c_running) {
		palSetPadMode(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN,
				PAL_MODE_ALTERNATE(HW_I2C_GPIO_AF) |
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);
		palSetPadMode(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN,
				PAL_MODE_ALTERNATE(HW_I2C_GPIO_AF) |
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);

		i2cStart(&HW_I2C_DEV, &i2cfg);
		i2c_running = true;
	}

	i2cReleaseBus(&HW_I2C_DEV);
}

void hw_stop_i2c(void) {
	i2cAcquireBus(&HW_I2C_DEV);

	if (i2c_running) {
		palSetPadMode(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN, PAL_MODE_INPUT);
		palSetPadMode(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN, PAL_MODE_INPUT);

		i2cStop(&HW_I2C_DEV);
		i2c_running = false;

	}

	i2cReleaseBus(&HW_I2C_DEV);
}

/**
 * Try to restore the i2c bus
 */
void hw_try_restore_i2c(void) {
	if (i2c_running) {
		i2cAcquireBus(&HW_I2C_DEV);

		palSetPadMode(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN,
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);

		palSetPadMode(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN,
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);

		palSetPad(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN);
		palSetPad(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN);

		chThdSleep(1);

		for(int i = 0;i < 16;i++) {
			palClearPad(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN);
			chThdSleep(1);
			palSetPad(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN);
			chThdSleep(1);
		}

		// Generate start then stop condition
		palClearPad(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN);
		chThdSleep(1);
		palClearPad(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN);
		chThdSleep(1);
		palSetPad(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN);
		chThdSleep(1);
		palSetPad(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN);

		palSetPadMode(HW_I2C_SCL_PORT, HW_I2C_SCL_PIN,
				PAL_MODE_ALTERNATE(HW_I2C_GPIO_AF) |
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);

		palSetPadMode(HW_I2C_SDA_PORT, HW_I2C_SDA_PIN,
				PAL_MODE_ALTERNATE(HW_I2C_GPIO_AF) |
				PAL_STM32_OTYPE_OPENDRAIN |
				PAL_STM32_OSPEED_MID1 |
				PAL_STM32_PUPDR_PULLUP);

		HW_I2C_DEV.state = I2C_STOP;
		i2cStart(&HW_I2C_DEV, &i2cfg);

		i2cReleaseBus(&HW_I2C_DEV);
	}
}

