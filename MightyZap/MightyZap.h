#pragma once

#include "stm32f7xx_hal.h"
#include "stdlib.h"
#include "stm32f7xx_it.h"
///////////////// utility for value ///////////////////////////
#define IRP_MAKEint(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define IRP_MAKEDint(a, b)     ((unsigned long)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned long)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define IRP_LOint(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define IRP_HIint(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define IRP_LOBYTE(w)           ((unsigned char)(((unsigned short)(w)) & 0xff))
#define IRP_HIBYTE(w)           ((unsigned char)((((unsigned short)(w)) >> 8) & 0xff))

#define IRP_RX_BUF_SIZE 255  //0x3FF
#define IRP_PARAMETER_BUF_SIZE 128
#define IRP_BROADCAST_ID		(254) /* 0xFE */


/* timing defines */
#define RX_TIMEOUT_COUNT2		(1600L) //(1000L) //porting
#define NANO_TIME_DELAY			(12000)
//#define RX_TIMEOUT_COUNT1  	(RX_TIMEOUT_COUNT2*90L)// -> 110msec  before Ver 1.11e
#define RX_TIMEOUT_COUNT1  		(RX_TIMEOUT_COUNT2*128L)//  -> 156msec for ID 110 safe access after Ver 1.11f //porting ydh

#define IRP_PACKET_TYPE1	1 //IRP protocol 1.0 packet type
#define IRP_PACKET_TYPE2	2 //IRP protocol 2.0 packet type

 /* defines message of communication
 * */
#define	COMM_TXSUCCESS		(0)
#define COMM_RXSUCCESS		(1)
#define COMM_TXFAIL			(2)
#define COMM_RXFAIL			(3)
#define COMM_TXERROR		(4)
#define COMM_RXWAITING		(5)
#define COMM_RXTIMEOUT		(6)
#define COMM_RXCORRUPT		(7)

enum IRP_INSTRUCTION {
	CMD_PING = 0xF1,
	CMD_READ = 0xF2,
	CMD_WRITE = 0xF3,
	CMD_REG_WRITE = 0xF4,
	CMD_ACTION = 0xF5,
	CMD_FACTORY_RESET = 0xF6,
	CMD_REBOOT = 0xF8,
	CMD_SYSTEM_WRITE = 0xF9,   // 0x0D
	CMD_STATUS = 0x55,
	CMD_SYNC_READ = 0x82,
	CMD_SYNC_WRITE = 0x73,  // 0x83
	CMD_BULK_READ = 0x92,
	CMD_BULK_WRITE = 0x93
};

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

void begin(UART_HandleTypeDef *huart, int baud);
void setPacketType(int type);

void BaudRate(UART_HandleTypeDef *huart, int bID, int baud_num);
int BaudRate(UART_HandleTypeDef *huart, int bID);

void ServoID(UART_HandleTypeDef *huart, int bID, int new_ID);
int ServoID(UART_HandleTypeDef *huart, int bID);

void goalPosition(UART_HandleTypeDef *huart, int bID, int value);
int goalPosition(UART_HandleTypeDef *huart, int bID);

void goalCurrent(UART_HandleTypeDef *huart, int bID, int value);

int presentPosition(UART_HandleTypeDef *huart, int bID);
int presentCurrent(UART_HandleTypeDef *huart, int bID);

int readByte(UART_HandleTypeDef *huart, int bID, int bAddress);
int writeint(UART_HandleTypeDef *huart, int bID, int bAddress, short wData);
int readint(UART_HandleTypeDef *huart, int bID, int bAddress);
int txPacket(UART_HandleTypeDef *huart, int bID, int bInstruction, int bParameterLength);
int rxPacket(UART_HandleTypeDef *huart, int bRxLength);
int txRxPacket(UART_HandleTypeDef *huart, int bID, int bInst, int bTxParaLen);

void returnMode(UART_HandleTypeDef *huart, int bID, int mode);
int writeByte(UART_HandleTypeDef *huart, int bID, int bAddress, int bData);
int ping(UART_HandleTypeDef *huart, int bID);
void forceEnable(UART_HandleTypeDef *huart, int bID, int value);
int forceEnable(UART_HandleTypeDef *huart, int bID);
int syncWrite(UART_HandleTypeDef *huart, int start_addr, int data_length, int *param, int param_length);