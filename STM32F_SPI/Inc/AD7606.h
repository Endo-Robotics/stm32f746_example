#pragma once

#include "stm32f7xx_hal.h"
/*
AD7606을 사용하기 위하여 설정해야 Pin

OS0 : GPIO Output
OS1 : GPIO Output
OS2 : GPIO Output
RST : GPIO Output
CVA : GPIO Output
CVB : GPIO Output
BUSY : GPIO Input
RAGE : GPIO Output
CS : GPIO Output


RD : SPI clk
DB7 : SPI Data Out
*/

extern void Delay_us(uint32_t us);

void AD7606_Reset();
void AD7606_VoltageRange_Setting(int nVoltage);
void AD7606_OverSampling_Setting(int nMode);
void AD7606_DataReadSignal();
void AD7606_DataRead(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint16_t* pSpiValue);