/*
 * @brief Blinky example using sysTick
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


#if defined (BOARD_EDU_CIAA_NXP)
#define TP1_1 (1)	/* Test IDE LPCXpresso */
#define TP1_2 (2)	/* Test periph_systick LPCOpen */
#define TP1_3 (3)	/* Test LEDs & RGB-LEDs EDU-CIAA-NXP */
#define TP1_4 (4)	/* Test Push Buttons LPCOpen */
#define TP1_5 (5)	/* Test LEDs & RGB-LEDs & Push Buttons EDU-CIAA-NXP */
#define TP1_6_LED (6)	/* Test State Machines for LEDs & RGB-LEDs & Push Buttons EDU-CIAA-NXP */
#define TP1_6_BUTTON (7)
#define TP1_6_TODO (8)

#define TEST (TP1_6_TODO)
#endif


#define TICKRATE_HZ (1000)	/* 1000 ticks per second */

#if TEST==TP1_2 || TEST==TP1_3 || TEST==TP1_4 || TEST==TP1_5 || TEST==TP1_6_LED || TEST==TP1_6_BUTTON || TEST==TP1_6_TODO
#define TICKRATE_10HZ (10)				/* 10 ticks per second */
#define TICKRATE_100HZ (100)			/* 100 ticks per second */
#define TICKRATE_1000HZ (1000)			/* 1000 ticks per second */
#define TICKRATE_HZ1 (TICKRATE_1000HZ)

#define LED_TOGGLE_1MS (1)
#define LED_TOGGLE_10MS (10)
#define LED_TOGGLE_20MS (20)
#define LED_TOGGLE_100MS (100)
#define LED_TOGGLE_250MS (250)
#define LED_TOGGLE_500MS (500)
#define LED_TOGGLE_1000MS (1000)

#define LED_TOGGLE_MS1 (LED_TOGGLE_250MS * 1000 / TICKRATE_1000HZ)
#define LED_TOGGLE_MS2 (LED_TOGGLE_500MS * TICKRATE_100HZ / 1000)

#define BUTTON_DEBOUNCE_25MS (25)
#define BUTTON_DEBOUNCE_50MS (50)
#define BUTTON_DEBOUNCE_MS1 (BUTTON_DEBOUNCE_50MS * 1000 / TICKRATE_1000HZ)

#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

#if TEST==TP1_2 || TEST==TP1_3 || TEST==TP1_4 || TEST==TP1_5 || TEST==TP1_6_LED || TEST==TP1_6_BUTTON || TEST==TP1_6_TODO
volatile bool LED_Time_Flag = false;
#endif

#if TEST==TP1_4 || TEST==TP1_5 || TEST==TP1_6_LED || TEST==TP1_6_BUTTON || TEST==TP1_6_TODO
volatile bool BUTTON_Status_Flag = false;
#endif

#if TEST==TP1_6_LED || TEST==TP1_6_BUTTON || TEST==TP1_6_TODO
volatile bool BUTTON_Time_Flag = true;
bool LED_SM_Flag; /* Flag para cambiar de estado. true=cambiar de estado. false=quedarse en el estado */
uint32_t LED_SM_State; /* Variable que representa al estado. 0=REPOSO. 1=TITILAR */
uint32_t LED_SM_Counter; /* Contador para titilar */
uint32_t BUTTON_SM_State; /* Variable que representa al estado del pulsador. 0=NO OPRIMIDO. 1=ESPERAR. 2=OPRIMIDO */
uint32_t BUTTON_SM_Counter;
#endif

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* TESTEO DE LA PLACA (TP1_1): */
#if TEST==TP1_1
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
static uint32_t tick_ct = 0;
void SysTick_Handler(void)
{
	tick_ct += 1;
	if ((tick_ct % 50) == 0) {
		Board_LED_Toggle(0);
	}
}

int main(void)
{

	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	while (1) {
		__WFI();
	}
}
#endif


/* CAMBIO DE PERÍODO DE PARPADEO DEL LED (TP1_2): */
#if TEST==TP1_2
void SysTick_Handler(void)
{
	LED_Time_Flag = true;
}


/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	uint32_t LED_Toggle_Counter = 0;

	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	while (1) {
		__WFI();


		if (LED_Time_Flag == true) {
			LED_Time_Flag = false;

			if (LED_Toggle_Counter == 0) {
				LED_Toggle_Counter = LED_TOGGLE_MS1;
				Board_LED_Toggle(3);
			}
			else
				LED_Toggle_Counter--;
		}
	}
}

#endif


/* PRUEBA DE TODOS LOS LEDS DE LA PLACA (TP1_3): */
#if TEST==TP1_3
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
		LED_Time_Flag = true;
}

void LED_Toggle_Rotate_Toggle_Init(void)
{
	Board_LED_Set(LED3, LED_ON);
}

void LED_Toggle_Rotate_Toggle(void)
{
	static uint32_t LED_Toggle_Counter = 0;
	static uint32_t idx = LED3;

	if (LED_Time_Flag == true) {
		LED_Time_Flag = false;

		if (LED_Toggle_Counter == 0) {
			LED_Toggle_Counter = LED_TOGGLE_MS1;

			Board_LED_Toggle(idx);

			((idx > 0) ? idx-- : (idx = LED3));

			Board_LED_Toggle(idx);
		}
		else
			LED_Toggle_Counter--;
	}
}

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	LED_Toggle_Rotate_Toggle_Init();

	while (1) {
		__WFI();

		LED_Toggle_Rotate_Toggle();
	}
}

#endif



/* PRUEBA DE LOS PULSADORES DE LA PLACA (TP1_4): */
#if TEST==TP1_4

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
 void SysTick_Handler(void)
{
	LED_Time_Flag = true;

	if (Buttons_GetStatus())
		BUTTON_Status_Flag = true;
	else
		BUTTON_Status_Flag = false;
}


/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	while (1) {
		__WFI();

		if (BUTTON_Status_Flag == true) {
			BUTTON_Status_Flag = false;
			Board_LED_Toggle(LED3);
		}
	}
}

#endif

/* PRUEBA DE LOS PULSADORES DE LA PLACA (TP1_5): */
#if TEST==TP1_5

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
 void SysTick_Handler(void)
{
	LED_Time_Flag = true;

	if (Buttons_GetStatus())
		BUTTON_Status_Flag = true;
	else
		BUTTON_Status_Flag = false;
}


void LED_Toggle_Rotate_Toggle_Init(void)
{
	Board_LED_Set(LED3, LED_ON);
}

void LED_Toggle_Rotate_Toggle(void)
{
	static uint32_t LED_Toggle_Counter = 0;
	static uint32_t idx = LED3;

	if (LED_Time_Flag == true) {
		LED_Time_Flag = false;

		if (LED_Toggle_Counter == 0) {
			LED_Toggle_Counter = LED_TOGGLE_MS1;

			Board_LED_Toggle(idx);

			((idx > 0) ? idx-- : (idx = LED3));

			Board_LED_Toggle(idx);
		}
		else
			LED_Toggle_Counter--;
	}
}

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	LED_Toggle_Rotate_Toggle_Init();

	while (1) {
		__WFI();

		if (BUTTON_Status_Flag == true) {
			BUTTON_Status_Flag = false;
			LED_Toggle_Rotate_Toggle();
		}
	}
}
#endif

/* Máquina de estados para hacer la rutina de titilar (TP1_6_LED): */
#if TEST==TP1_6_LED

void SysTick_Handler(void)
{
	LED_Time_Flag = true;
	BUTTON_Time_Flag = true;

	if (Buttons_GetStatus())
		BUTTON_Status_Flag = true;
	else
		BUTTON_Status_Flag = false;

}


void LED_State_Machine_Init(void)
{
	LED_SM_Flag = false;
	LED_SM_State = 0;
	LED_SM_Counter = 0;
	Board_LED_Toggle(LED3);
}

void LED_State_Machine(void)
{
	static uint32_t idx = LED3;

	/* LED State Machine */
	if (LED_Time_Flag == true) {
		LED_Time_Flag = false;

		switch (LED_SM_State) {
			case 0:
				if (LED_SM_Flag == true) {
					LED_SM_Flag = false;
					LED_SM_State = 1;
					DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);
				}
				break;

			case 1:
				if (LED_SM_Counter != 0)
					LED_SM_Counter--;
				else {
					LED_SM_Counter = LED_TOGGLE_MS1;
					Board_LED_Toggle(idx);

					((idx > 0) ? idx-- : (idx = LED3));

					Board_LED_Toggle(idx);
				}

				if (LED_SM_Flag == true) {
					LED_SM_Flag = false;
					LED_SM_State = 0;
					DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);
				}
				break;
		}
	}
}


/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	DEBUGOUT("\r\n -- TP1_5 : Test State Machines --\r\n");

	LED_State_Machine_Init();
	DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);

	//BUTTON_State_Machine_Init();
	//DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);

	while (1) {
		__WFI();

		/* LED State Machine */
		LED_State_Machine();

		/* BUTTON State Machine */
		//BUTTON_State_Machine();
	}
}

#endif


#if TEST==TP1_6_BUTTON

void SysTick_Handler(void)
{
	LED_Time_Flag = true;
	BUTTON_Time_Flag = true;

	if (Buttons_GetStatus())
		BUTTON_Status_Flag = true;
	else
		BUTTON_Status_Flag = false;

}


void LED_State_Machine_Init(void)
{
	LED_SM_Flag = false;
	LED_SM_State = 0;
	LED_SM_Counter = 0;
	Board_LED_Toggle(LED3);
}

void BUTTON_State_Machine_Init(void)
{
	BUTTON_SM_State = 0;
	BUTTON_SM_Counter = 0;
}

void BUTTON_State_Machine(void)
{
	/* BUTTON State Machine */
	if (BUTTON_Time_Flag == true) {
		BUTTON_Time_Flag = false;

		switch (BUTTON_SM_State) {
			case 0:
				if (BUTTON_Status_Flag == true) {
					BUTTON_SM_Counter = BUTTON_DEBOUNCE_MS1;
					BUTTON_SM_State = 1;
					DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
				}
				break;

			case 1:
				if (BUTTON_SM_Counter != 0)
					BUTTON_SM_Counter--;
				else {
					if (BUTTON_Status_Flag == false) {
						BUTTON_SM_State = 0;
					}
					else {
						BUTTON_Status_Flag = false;
						BUTTON_SM_State = 2;
						DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
						LED_SM_Flag = !LED_SM_Flag;
						//LED_SM_Flag = true;
					}
				}
				break;

			case 2:
				if (BUTTON_Status_Flag == false) {
					//BUTTON_SM_State = 0;
					BUTTON_SM_State = 1;
					BUTTON_SM_Counter = BUTTON_DEBOUNCE_MS1;
					DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
				}
				break;
		}
	}
}


/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	DEBUGOUT("\r\n -- TP1_5 : Test State Machines --\r\n");

	LED_State_Machine_Init();
	DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);

	BUTTON_State_Machine_Init();
	DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);

	while (1) {
		__WFI();

		/* LED State Machine */
		//LED_State_Machine();
		if (LED_SM_Flag==true){
			Board_LED_Set(LED3, LED_ON);
		}
		else
			Board_LED_Set(LED3, LED_OFF);

		/* BUTTON State Machine */
		BUTTON_State_Machine();
	}
}

#endif

#if TEST==TP1_6_TODO

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	LED_Time_Flag = true;
	BUTTON_Time_Flag = true;

	if (Buttons_GetStatus())
		BUTTON_Status_Flag = true;
	else
		BUTTON_Status_Flag = false;

}

void LED_State_Machine_Init(void)
{
	LED_SM_Flag = false;
	LED_SM_State = 0;
	LED_SM_Counter = 0;
	Board_LED_Toggle(LED3);
}

void LED_State_Machine(void)
{
	static uint32_t idx = LED3;

	/* LED State Machine */
	if (LED_Time_Flag == true) {
		LED_Time_Flag = false;

		switch (LED_SM_State) {
			case 0:
				if (LED_SM_Flag == true) {
					LED_SM_Flag = false;
					LED_SM_State = 1;
					DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);
				}
				break;

			case 1:
				if (LED_SM_Counter != 0)
					LED_SM_Counter--;
				else {
					LED_SM_Counter = LED_TOGGLE_MS1;
					Board_LED_Toggle(idx);

					((idx > 0) ? idx-- : (idx = LED3));

					Board_LED_Toggle(idx);
				}

				if (LED_SM_Flag == true) {
					LED_SM_Flag = false;
					LED_SM_State = 0;
					DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);
				}
				break;
		}
	}
}


void BUTTON_State_Machine_Init(void)
{
	BUTTON_SM_State = 0;
	BUTTON_SM_Counter = 0;
}

void BUTTON_State_Machine(void)
{
	/* BUTTON State Machine */
	if (BUTTON_Time_Flag == true) {
		BUTTON_Time_Flag = false;

		switch (BUTTON_SM_State) {
			case 0:
				if (BUTTON_Status_Flag == true) {
					BUTTON_SM_Counter = BUTTON_DEBOUNCE_MS1;
					BUTTON_SM_State = 1;
					DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
				}
				break;

			case 1:
				if (BUTTON_SM_Counter != 0)
					BUTTON_SM_Counter--;
				else {
					if (BUTTON_Status_Flag == false) {
						BUTTON_SM_State = 0;
					}
					else {
						//BUTTON_Status_Flag = false;
						BUTTON_SM_State = 2;
						DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
						LED_SM_Flag = true;
					}
				}
				break;

			case 2:
				if (BUTTON_Status_Flag == false) {
					//BUTTON_SM_State = 0;
					BUTTON_SM_State = 1;
					BUTTON_SM_Counter = BUTTON_DEBOUNCE_MS1;
					DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);
				}
				break;
		}
	}
}


/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	DEBUGOUT("\r\n -- TP1_5 : Test State Machines --\r\n");

	LED_State_Machine_Init();
	DEBUGOUT("\r\n          : LED_State_Machine    => LED_SM_State:    %d --\r\n", LED_SM_State);

	BUTTON_State_Machine_Init();
	DEBUGOUT("\r\n          : BUTTON_State_Machine => BUTTON_SM_State: %d --\r\n", BUTTON_SM_State);

	while (1) {
		__WFI();

		/* LED State Machine */
		LED_State_Machine();

		/* BUTTON State Machine */
		BUTTON_State_Machine();
	}
}

#endif
