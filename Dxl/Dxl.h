#pragma once

#define PING_INSTRUCTION_ADD            1
/* Error값에 대한 정보
0x01 Result Fail          전송된 instruction packet을 처리하는데 실패한 경우
0x02 Instruction Error  정의되지 않은 instruction을 사용한 경우, 
                             Reg Write 없이 Action을 사용한 경우
0x03 CRC Error          전송된 Packet의 CRC 값이 맞지 않는 경우
0x04 Data Range Error   해당 Address에 쓰려는 Data가 최소/최대값의 범위를 벗어난 경우
0x05 Data Length Error  해당 Address의 테이터 길이보다 짧은 데이터를 적으려고 한 경우
0x06 Data Limit Error   해당 Address에 쓰려는 Data가 Limit 값을 벗어난 경우
0x07 Access Error       Read Only 혹은 정의되지 않은 Address에 값을 쓰려는 한 경우,
                        Write Only 혹은 정의되지 않은 Address에 값을 읽으려고 한 경우,
                        Torque Enable(ROM Lock)상태에서 ROM 영역에 값을 쓰려고 하는 경우
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