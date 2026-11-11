//** this is the current version of main.c **//

//** recent changes include modifying lab 11 and deleting what isn't needed **//


/********************************************************************************
* Battleship implementation
* @file battleship.c
* @author Bridgette Whatcott-Bledsoe and Evyn Anderson
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */


#include "main.h"
#include "usb_host.h"
#include "seg7.h"

/* Private variables */
I2C_HandleTypeDef hi2c1;
I2S_HandleTypeDef hi2s3;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim7;


/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
void MX_USB_HOST_Process(void);


/* declare local variables*/
int DelayValue = 50;
char ramp = 0;
char RED_BRT = 0;
char GREEN_BRT = 0;
char BLUE_BRT = 0;
char RED_STEP = 1;
char GREEN_STEP = 2;
char BLUE_STEP = 3;
char DIM_Enable = 0;
char Music_ON = 0;
int TONE = 0;
int COUNT = 0;
int INDEX = 0;
int Note = 0;
int Save_Note = 0;
int Vibrato_Depth = 1;
int Vibrato_Rate = 40;
int Vibrato_Count = 0;
char Animate_On = 0;
char Message_Length = 0;
char *Message_Pointer;
char *Save_Pointer;
int Delay_msec = 0;
int Delay_counter = 0;

/* displaying "battleship" across display */
char Message[] =
		{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
		CHAR_B, CHAR_A, CHAR_T, CHAR_T, CHAR_L, CHAR_E, CHAR_S, CHAR_H, CHAR_I, CHAR_P,
		SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE};
/* Declare array for Song */
Music Song[100];
/* USER CODE END 0 */
/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
 /* USER CODE BEGIN 1 */
 /* USER CODE END 1 */
 /* MCU Configuration--------------------------------------------------------*/
 /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 HAL_Init();
 /* USER CODE BEGIN Init */
 /* USER CODE END Init */
 /* Configure the system clock */
 SystemClock_Config();
 /* USER CODE BEGIN SysInit */
 /* USER CODE END SysInit */
 /* Initialize all configured peripherals */
 MX_GPIO_Init();
 //MX_I2C1_Init();
 //MX_I2S3_Init();
 //MX_SPI1_Init();
 //MX_USB_HOST_Init();
 MX_TIM7_Init();
 /* USER CODE BEGIN 2 */
 /*** Configure GPIOs ***/
 GPIOD->MODER = 0x55555555; // set all Port D pins to outputs
 GPIOA->MODER |= 0x000000FF; // Port A mode register - make A0 to A3 analog pins
 GPIOE->MODER |= 0x55555555; // Port E mode register - make E0 to E15 outputs
 GPIOC->MODER |= 0x0; // Port C mode register - all inputs
 GPIOE->ODR = 0xFFFF; // Set all Port E pins high
 /*** Configure ADC1 ***/
 RCC->APB2ENR |= 1<<8;  // Turn on ADC1 clock by forcing bit 8 to 1 while keeping other bits unchanged
 ADC1->SMPR2 |= 1; // 15 clock cycles per sample
 ADC1->CR2 |= 1;        // Turn on ADC1 by forcing bit 0 to 1 while keeping other bits unchanged
 /*** Turn on CRC Clock in AHB1ENR to enable CRC hardware ***/
// RCC->AHB1ENR |= 1<<12;
 /*****************************************************************************************************
 These commands are handled as part of the MX_TIM7_Init() function and don't need to be enabled
 RCC->AHB1ENR |= 1<<5; // Enable clock for timer 7
 __enable_irq(); // Enable interrupts
 NVIC_EnableIRQ(TIM7_IRQn); // Enable Timer 7 Interrupt in the NVIC controller
 *******************************************************************************************************/
 TIM7->PSC = 199; //250Khz timer clock prescaler value, 250Khz = 50Mhz / 200
 TIM7->ARR = 1; // Count to 1 then generate interrupt (divide by 2), 125Khz interrupt rate to increment byte counter for 78Hz PWM
 TIM7->DIER |= 1; // Enable timer 7 interrupt
 TIM7->CR1 |= 1; // Enable timer counting
 /* USER CODE END 2 */
 /* Infinite loop */
 /* USER CODE BEGIN WHILE */
 while (1)
 {
	  int i,j;
	  Message_Pointer = &Message[0];
	  Save_Pointer = &Message[0];
	  Message_Length = sizeof(Message)/sizeof(Message[0]);
	  Delay_msec = 200;
	  Animate_On = 1;


	  HAL_Delay(5000);           // Delay 5 seconds to allow message to scroll

	  Animate_On = 0;            // Stop scrolling message
//	  Seven_Segment(CRC_Tx);     // Display CRC for sent message
	  HAL_Delay(1000);           // Delay 1 second
	  for (i=0 ; i<8 ; i++)      // Clear the display
	  	  {
	  		  Seven_Segment_Digit(i,SPACE,0);
	  	  }

	  HAL_Delay(500);           // Delay 1/2 second
	  HAL_Delay(100);          // Delay for 1 second


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 0;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */









































//BEGIN TA HELPER FILE//

// include statement//

#include "tmpl.h"

// forward declarations
static void poll_switches ( void );
static void poll_potentiometers ( void );
static void store_message ( const char *p_message );
static void state_start ( void );
static fn_state *state_p1_place;
static fn_state *state_p2_place;
static fn_state *state_p1_turn;
static fn_state *state_p2_turn;
static fn_state *state_result;

// data
static fn_state *_pfn_states[] =
{
[STATE_START] = state_start,
[STATE_P1_PLACE] = state_p1_place,
[STATE_P2_PLACE] = state_p2_place,
[STATE_P1_TURN] = state_p1_turn,
[STATE_P2_TURN] = state_p2_turn,
[STATE_RESULT] = state_result,
};


struct
{
	unsigned short sw; // the switches (16 switches -> 16 bits -> short)
	unsigned short ld; // the led's (16 leds -> 16 bits -> short)
	bool pc10; // true IF PC10 is down ELSE false
	bool pc11; // true IF PC11 is down ELSE false
	unsigned short _pa[3]; // raw potentiometer values (3 potentiometers -> 3
	//values -> array of 3 shorts)
	unsigned int time; // the time (in ms) since the board started
	//(incremented by timer interrupt)
}
device_state =
{
	.sw = 0,
	.ld = 0,
	.pc10 = false,
	.pc11 = false,
	._pa =
{
	0, // <- PA1
	0, // <- PA2
	0, // <- PA3
},
};





// player data
player players[PLAYER_QUANTITY] =
{
		
		
// player A's data
[PLAYER_A] =		
{
		
		
// player A's maps
.maps =
		
{
		
		
// player A's ship map is empty when the game starts
[SHIP] =
{
.horizontal = { STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
		      { STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
		      { STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
{

		
},
.vertical = { STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
		    { STILL, STILL, STILL, STILL, STILL, STILL, STILL,STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
{
},
},


// player A's shot map is empty when the game starts
[SHOT] =
{
.horizontal = { 0 },
.vertical = { 0 },
}
}
},


// Player B's data
[PLAYER_B] =
{
		
// player B's maps
.maps =
{
// player B's ship map is empty when the game starts
[SHIP] =
{
.horizontal = {STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
			  {STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
			  {STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
			  
			  
.vertical = { STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
			{ STILL, STILL, STILL, STILL, STILL, STILL, STILL,STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, STILL, },
{
		
}

},
// player B's shot map is empty when the game starts
[SHOT] =
{
.horizontal = { 0 },
.vertical = { 0 },
}
},
},
};


extern void input_poll ( void )
{
// poll the switches
poll_switches();
// poll the potentiometers
poll_potentiometers();
// todo: poll any other input devices
//
// done
return;
}
static void poll_switches ( void )
{
// todo: store the state of the switches in device_state.sw
//
// done
return;
}
static void poll_potentiometers ( void )
{
// todo: iterate through each potentiometer and store the value in the
corresponding index of device_state._pa
//
// done
return;



}
extern void timer_step ( void )
{
// todo: call this function from the SysTick handler
//
// increment the time
device_state.time++;
// done
return;
}
extern void draw_board ( map _map )
{
// initialized data
unsigned char _board[8] = { 0 };
bool should_dim = (device_state.time % 5 == 0 );
bool should_blink = ( (device_state.time / 250) % 2 == 0 );
// draw the horizontal segments
for (size_t i = 0; i < 8; i++)
{
// set the top segment based on the horizontal[0] map
switch(_map.horizontal[0][i])
{
case STILL:
_board[i] |= 0;
break;
case HIT:
_board[i] |= SEG7_TOP;
break;
case MISS:
_board[i] |= should_dim ? SEG7_TOP : 0;
break;
case CURSOR:
_board[i] |= should_blink ? SEG7_TOP : 0;
break;
}
// todo: set the middle segment based on the horizontal[1] map
//
// todo: set the bottom segment based on the horizontal[2] map
//
}
// draw the vertical segments
for (size_t i = 0; i < 8; i++)
{
// todo: set the top left segment based on the vertical[0] map
//
// todo: set the top right segment based on the vertical[0] map
//
// todo: set the bottom left segment based on the vertical[1] map
//
// todo: set the bottom right segment based on the vertical[1] map
//
}
// write the board array to the seven segment display
for(int i = 0; i < 8; i++)
GPIOE->ODR = (0XFF00 | ~_board[i]) & ~(1<<((7-i)+8)),
GPIOE->ODR |= 0xFF00;
// done
return;
}
extern void game_logic ( void )
{
// static data
static int state = STATE_START;
// update
_pfn_states[state]();
// state transition
switch (state)
{
case STATE_START:
if ( device_state.pc10 )
state = STATE_P1_PLACE;
break;
// player 1 places boats
case STATE_P1_PLACE:
// transition
if ( 0 )
state = STATE_P2_PLACE;
break;
// player 2 places boats
case STATE_P2_PLACE:
// transition
if ( 0 )
state = STATE_P2_PLACE;
break;
// player 1 takes a shot
case STATE_P1_TURN:
// transition
if ( 0 )
state = STATE_P2_TURN;
else if ( 0 )
state = STATE_RESULT;
break;
// player 2 takes a shot
case STATE_P2_TURN:
// transition
if ( 0 )
state = STATE_P1_TURN;
else
state = STATE_RESULT;
break;
// game over
case STATE_RESULT:
if ( device_state.pc10 )
state = STATE_START;
break;
// unknown state -> reset
default:
state = STATE_START;
break;
}
// done
return;
}
void state_start ( void )
{
// todo
//
// done
return;
}
void state_p1_place ( void )
{
// todo
//
// done
return;
}
void state_p2_place ( void )
{
// todo
//
// done
return;
}
void state_p1_turn ( void )
{
// todo
//
// done
return;
}
void state_p2_turn ( void )
{
// todo
//
// done
return;
}
void state_result ( void )
{
// todo
//
// done
return;
}










