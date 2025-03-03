/*
	Copyright 2016 - 2025 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef MCPWM_COMMON_H_
#define MCPWM_COMMON_H_

// Common macros

////////////////////////////////////////////////////

#define TIMER_UPDATE_CH1_0() \
		TIM1->CCER &= ~TIM_CCER_CC1E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC1M_2; \
		TIM1->CCER |= TIM_CCER_CC1E; \
		TIM1->CCER &= ~TIM_CCER_CC1NE;

#define TIMER_UPDATE_M2_CH1_0() \
		TIM8->CCER &= ~TIM_CCER_CC1E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC1M_2; \
		TIM8->CCER |= TIM_CCER_CC1E; \
		TIM8->CCER &= ~TIM_CCER_CC1NE;

///

#define TIMER_UPDATE_CH1_POS() \
		TIM1->CCER &= ~TIM_CCER_CC1E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; \
		TIM1->CCER |= TIM_CCER_CC1E; \
		TIM1->CCER |= TIM_CCER_CC1NE;

#define TIMER_UPDATE_M2_CH1_POS() \
		TIM8->CCER &= ~TIM_CCER_CC1E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; \
		TIM8->CCER |= TIM_CCER_CC1E; \
		TIM8->CCER |= TIM_CCER_CC1NE;

///

#define TIMER_UPDATE_CH1_NEG() \
		TIM1->CCER &= ~TIM_CCER_CC1E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC1M_2; \
		TIM1->CCER |= TIM_CCER_CC1E; \
		TIM1->CCER |= TIM_CCER_CC1NE;

#define TIMER_UPDATE_M2_CH1_NEG() \
		TIM8->CCER &= ~TIM_CCER_CC1E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC1M_2; \
		TIM8->CCER |= TIM_CCER_CC1E; \
		TIM8->CCER |= TIM_CCER_CC1NE;

////////////////////////////////////////////////////

#define TIMER_UPDATE_CH2_0() \
		TIM1->CCER &= ~TIM_CCER_CC2E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC2M_2; \
		TIM1->CCER |= TIM_CCER_CC2E; \
		TIM1->CCER &= ~TIM_CCER_CC2NE;

#define TIMER_UPDATE_M2_CH2_0() \
		TIM8->CCER &= ~TIM_CCER_CC2E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC2M_2; \
		TIM8->CCER |= TIM_CCER_CC2E; \
		TIM8->CCER &= ~TIM_CCER_CC2NE;

///

#define TIMER_UPDATE_CH2_POS() \
		TIM1->CCER &= ~TIM_CCER_CC2E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; \
		TIM1->CCER |= TIM_CCER_CC2E; \
		TIM1->CCER |= TIM_CCER_CC2NE;

#define TIMER_UPDATE_M2_CH2_POS() \
		TIM8->CCER &= ~TIM_CCER_CC2E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; \
		TIM8->CCER |= TIM_CCER_CC2E; \
		TIM8->CCER |= TIM_CCER_CC2NE;

///

#define TIMER_UPDATE_CH2_NEG() \
		TIM1->CCER &= ~TIM_CCER_CC2E; \
		TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM1->CCMR1 |= TIM_CCMR1_OC2M_2; \
		TIM1->CCER |= TIM_CCER_CC2E; \
		TIM1->CCER |= TIM_CCER_CC2NE;

#define TIMER_UPDATE_M2_CH2_NEG() \
		TIM8->CCER &= ~TIM_CCER_CC2E; \
		TIM8->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; \
		TIM8->CCMR1 |= TIM_CCMR1_OC2M_2; \
		TIM8->CCER |= TIM_CCER_CC2E; \
		TIM8->CCER |= TIM_CCER_CC2NE;

////////////////////////////////////////////////////

#define TIMER_UPDATE_CH3_0() \
		TIM1->CCER &= ~TIM_CCER_CC3E; \
		TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM1->CCMR2 |= TIM_CCMR2_OC3M_2; \
		TIM1->CCER |= TIM_CCER_CC3E; \
		TIM1->CCER &= ~TIM_CCER_CC3NE;

#define TIMER_UPDATE_M2_CH3_0() \
		TIM8->CCER &= ~TIM_CCER_CC3E; \
		TIM8->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM8->CCMR2 |= TIM_CCMR2_OC3M_2; \
		TIM8->CCER |= TIM_CCER_CC3E; \
		TIM8->CCER &= ~TIM_CCER_CC3NE;

///

#define TIMER_UPDATE_CH3_POS() \
		TIM1->CCER &= ~TIM_CCER_CC3E; \
		TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; \
		TIM1->CCER |= TIM_CCER_CC3E; \
		TIM1->CCER |= TIM_CCER_CC3NE;

#define TIMER_UPDATE_M2_CH3_POS() \
		TIM8->CCER &= ~TIM_CCER_CC3E; \
		TIM8->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM8->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; \
		TIM8->CCER |= TIM_CCER_CC3E; \
		TIM8->CCER |= TIM_CCER_CC3NE;

///

#define TIMER_UPDATE_CH3_NEG() \
		TIM1->CCER &= ~TIM_CCER_CC3E; \
		TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM1->CCMR2 |= TIM_CCMR2_OC3M_2; \
		TIM1->CCER |= TIM_CCER_CC3E; \
		TIM1->CCER |= TIM_CCER_CC3NE;

#define TIMER_UPDATE_M2_CH3_NEG() \
		TIM8->CCER &= ~TIM_CCER_CC3E; \
		TIM8->CCMR2 &= ~TIM_CCMR2_OC3M_Msk; \
		TIM8->CCMR2 |= TIM_CCMR2_OC3M_2; \
		TIM8->CCER |= TIM_CCER_CC3E; \
		TIM8->CCER |= TIM_CCER_CC3NE;

////////////////////////////////////////////////////

#define TIMER_CONTROL_UPDATE() (TIM1->EGR = TIM_EGR_COMG)
#define TIMER_M2_CONTROL_UPDATE() (TIM8->EGR = TIM_EGR_COMG)

////////////////////////////////////////////////////

#endif /* MCPWM_COMMON_H_ */
