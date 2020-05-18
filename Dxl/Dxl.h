#pragma once

#define PING_INSTRUCTION_ADD            1
/* Error���� ���� ����
0x01 Result Fail          ���۵� instruction packet�� ó���ϴµ� ������ ���
0x02 Instruction Error  ���ǵ��� ���� instruction�� ����� ���, 
                             Reg Write ���� Action�� ����� ���
0x03 CRC Error          ���۵� Packet�� CRC ���� ���� �ʴ� ���
0x04 Data Range Error   �ش� Address�� ������ Data�� �ּ�/�ִ밪�� ������ ��� ���
0x05 Data Length Error  �ش� Address�� ������ ���̺��� ª�� �����͸� �������� �� ���
0x06 Data Limit Error   �ش� Address�� ������ Data�� Limit ���� ��� ���
0x07 Access Error       Read Only Ȥ�� ���ǵ��� ���� Address�� ���� ������ �� ���,
                        Write Only Ȥ�� ���ǵ��� ���� Address�� ���� �������� �� ���,
                        Torque Enable(ROM Lock)���¿��� ROM ������ ���� ������ �ϴ� ���
*/
#define RETURN_DELAY_TIME_ADD              9       // data bits : 1
#define OPERATION_MODE_ADD                  11      // data bits : 1
#define TORQUE_ENABLE_ADD                    64      // data bits : 1
#define STATUS_RETURN_LEVEL_ADD         68      // data bits : 1
#define GOAL_VELOCITY_ADD                     104     // data bits : 4
#define GOAL_POSITION_ADD                      116     // data bits : 4
#define PRESENT_POSITION_ADD                132     // data bits : 4

#define RETURN_DELAY_TIME                     2       //Micro sec
#define OPERATION_MODE_CURRENT          0
#define OPERATION_MODE_VELOCITY         1
#define OPERATION_MODE_POSITION          3
#define OPERATION_MODE_ROTOR              4
#define STATUS_RETURN_LEVEL_READ        1
#define STATUS_RETURN_LEVEL_ALL          2
#define TORQUE_DISABLE                           0
#define TORQUE_ENABLE                   1

#define DATA_BITS_ONE                   1
#define DATA_BITS_TWO                   2
#define DATA_BITS_FOUR                  4

#include "stdlib.h"
#include "stm32f7xx_it.h"
#include "stm32f7xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;


// check-sum  9
unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

void Dxl_init(UART_HandleTypeDef *huart, uint8_t id, int mode);
void Dxl_ping(UART_HandleTypeDef *haurt, uint8_t id, uint8_t address, int32_t data_arr, uint16_t data_length);
void Dxl_FactoryReset(UART_HandleTypeDef *huart, uint8_t id, uint8_t option);
void Dxl_Action(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint32_t data);

// write
void Dxl_Write(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, uint32_t data, uint16_t data_length);
void Dxl_RegWrite(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, uint32_t data, uint16_t data_length);
void Dxl_SyncWrite(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, uint32_t* data_arr, uint16_t data_length, uint16_t id_length);
void Dxl_SyncWriteCover(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, int32_t* data_arr, uint16_t data_length, uint16_t id_length);
void Dxl_WriteMinus(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, int32_t data, uint16_t data_length);
// read
void Dxl_Read(UART_HandleTypeDef *huart, uint8_t* buf,uint8_t* id, uint32_t* data);
void Dxl_Read_DMA(uint8_t* buf, uint8_t size, uint8_t* id, int32_t* data);
void Dxl_ReadInstruction(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint8_t data_length);
void Dxl_SyncReadInstruction(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, uint8_t data_length, uint16_t id_length);

// change EEPROM Data
void Dxl_ChangeEEPROM(UART_HandleTypeDef *huart, uint8_t id, uint8_t address ,uint32_t data);
void Dxl_SyncChangeEEPROM(UART_HandleTypeDef *huart, uint8_t* id, uint8_t address ,uint8_t* data, uint16_t id_length);

void Dxl_Read_Interface(uint8_t* buf, uint8_t size, uint8_t* id, int32_t* data);



void Dxl_ReadInstruction2(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint8_t data_length);