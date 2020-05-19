#include "AD7606.h"

void AD7606_Reset() {
  HAL_GPIO_WritePin(GPIOG, AD7606_RST_Pin, GPIO_PIN_SET);
  Delay_us(2);
  HAL_GPIO_WritePin(GPIOG, AD7606_RST_Pin, GPIO_PIN_RESET);  // RST Set -> Reset = Data Clear
}
void AD7606_VoltageRange_Setting(int nVoltage) {
  if (nVoltage == 5) {
    HAL_GPIO_WritePin(GPIOD, AD7606_RAGE_Pin, GPIO_PIN_RESET);
  }
  else if (nVoltage == 10) {
    HAL_GPIO_WritePin(GPIOD, AD7606_RAGE_Pin, GPIO_PIN_SET);
  }
}
void AD7606_OverSampling_Setting(int nMode) {
  switch (nMode)
  {
  case 0:// No OS 
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_RESET);  
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_RESET);    
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_RESET);	
    break;
  case 1:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_RESET);
    break;
  case 2:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_RESET);
    break;
  case 3:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_RESET);
    break;
  case 4:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_SET);
    break;
  case 5:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_SET);
    break;
  case 6:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_SET);
    break;
  case 7:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_SET);
    break;
  default:
    HAL_GPIO_WritePin(GPIOD, AD7606_OS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, AD7606_OS2_Pin, GPIO_PIN_RESET);
    break;
  }
}
void AD7606_DataReadSignal() {
  HAL_GPIO_WritePin(GPIOG, AD7606_CVB_Pin, GPIO_PIN_RESET);	// CVB reset	
  HAL_GPIO_WritePin(AD7606_CVA_GPIO_Port, AD7606_CVA_Pin, GPIO_PIN_RESET);   // CVA reset
  Delay_us(5);
  HAL_GPIO_WritePin(GPIOG, AD7606_CVB_Pin, GPIO_PIN_SET);	// CVB set
  HAL_GPIO_WritePin(AD7606_CVA_GPIO_Port, AD7606_CVA_Pin, GPIO_PIN_SET);	// CVA set

  HAL_GPIO_WritePin(GPIOG, AD7606_CS_Pin, GPIO_PIN_RESET);   // CS
  while (HAL_GPIO_ReadPin(AD7606_BUSY_GPIO_Port, AD7606_BUSY_Pin) == GPIO_PIN_SET);	// until waiting BUSY reset
  Delay_us(5);
}

void AD7606_DataRead(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint16_t* pSpiValue){//int32_t  uint16_t* pSpiValue){
  for (int i = 0; i < 8; i++) 
    pSpiValue[i] = ((uint16_t)pData[2 * (i)] << 8) | ((uint16_t)pData[2 * (i)+1]);
  
  HAL_GPIO_WritePin(GPIOG, AD7606_CS_Pin, GPIO_PIN_SET);   // CS
}