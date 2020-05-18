#include "MightyZap.h"
#include "stdint.h"

int mParamBuffer[IRP_PARAMETER_BUF_SIZE];
uint8_t mTxBuffer[IRP_RX_BUF_SIZE];
uint8_t mRxBuffer[IRP_RX_BUF_SIZE];
int mBusUsed;
int SmartDelayFlag;

int mPktIdIndex;
int mPktLengthIndex;
int mPktInstIndex;
int mPktErrorIndex;
int mRxLength;
int mPacketType;

int mCommStatus;
// additions to return proper COMM_* status
int mIRPtxrxStatus;
// additions to permit non-default Status Return Level settings without returning errors
int gbIRPStatusReturnLevel;

int mbLengthForPacketMaking;
int mbIDForPacketMaking;
int mbInstructionForPacketMaking;
uint8_t rx_temp2[255];

uint8_t rx_temp, a, rx_tmp_arr[10];
uint8_t showme2, b[4], bChecksum1, bLength_check, bChecksum3, temp_rx[20];

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

void begin(UART_HandleTypeDef *huart, int baud) {
  
	HAL_HalfDuplex_EnableTransmitter(huart);
	mIRPtxrxStatus = 0;
	mBusUsed = 0;// only 1 when tx/rx is operated
	SmartDelayFlag = 1;

	setPacketType(IRP_PACKET_TYPE1);
}

void setPacketType(int type) {
	mPacketType = type;
	if (mPacketType == IRP_PACKET_TYPE2) {
		mPktIdIndex = 4;
		mPktLengthIndex = 5;
		mPktInstIndex = 7;
		mPktErrorIndex = 8;
	}
	else {  
		mPktIdIndex = 3;
		mPktLengthIndex = 4;
		mPktInstIndex = 5;
		mPktErrorIndex = 5;
	}
}

void BaudRate(UART_HandleTypeDef *huart, int bID, int baud_num) {
	writeByte(huart, bID, 4, baud_num);
}

int BaudRate(UART_HandleTypeDef *huart, int bID) {
	for (int i = 0; i < sizeof(mRxBuffer); i++)
		mRxBuffer[i] = 0;
	return readByte(huart, bID, 4);
}

void ServoID(UART_HandleTypeDef *huart, int bID, int new_ID) {
	writeByte(huart, bID, 3, new_ID);
}
int ServoID(UART_HandleTypeDef *huart, int bID) {
	for (int i = 0; i < sizeof(mRxBuffer); i++)
		mRxBuffer[i] = 0;
	return readByte(huart, bID, 3);
}

void goalPosition(UART_HandleTypeDef *huart, int bID, int value) {
	writeint(huart, bID, 0x86, value);
}

int goalPosition(UART_HandleTypeDef *huart, int bID) {
	return readint(huart,bID, 0x86);
}

void goalCurrent(UART_HandleTypeDef *huart, int bID, int value) {
	writeint(huart, bID, 0x34, value);
}

int presentPosition(UART_HandleTypeDef *huart, int bID) {
	return readint(huart, bID, 0x8c);
}

int presentCurrent(UART_HandleTypeDef *huart, int bID) {
	return readint(huart, bID, 0xAC);
}


int readByte(UART_HandleTypeDef *huart, int bID, int bAddress) {
	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = 1;
	if (txRxPacket(huart, bID, CMD_READ, 2)) {
		mCommStatus = 1;
		return(rx_temp2[6]); //refer to 1.0 packet structure
	}
	else {
		mCommStatus = 0;
	}
}


int writeint(UART_HandleTypeDef *huart, int bID, int bAddress, short wData) {
	int param_length = 0;
  
	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = ((unsigned char)(((unsigned short)(wData)) & 0xff));//IRP_LOBYTE(wData);//(int)(wData&0xff);
	mParamBuffer[2] = ((unsigned char)((((unsigned short)(wData)) >> 8) & 0xff));//IRP_HIBYTE(wData);//(int)((wData>>8)&0xff);
	param_length = 3;
        
	return txRxPacket(huart, bID, CMD_WRITE, param_length);
}

int readint(UART_HandleTypeDef *huart, int bID, int bAddress) {
	for (int i = 0; i < sizeof(mRxBuffer); i++)
		mRxBuffer[i] = 0;

	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = 2;
	if (txRxPacket(huart, bID, CMD_READ, 2)) {
		return IRP_MAKEint(mRxBuffer[6], mRxBuffer[7]);//( (((int)mRxBuffer[6])<<8)+ mRxBuffer[5] );
	}
	else {
		return 0xffff;
	}
}

int txPacket(UART_HandleTypeDef *huart, int bID, int bInstruction, int bParameterLength) {

	int bCount, bCheckSum, bPacketLength;

	int offsetParamIndex;

	mTxBuffer[0] = 0xff;
	mTxBuffer[1] = 0xff;
	mTxBuffer[2] = 0xff; //
	mTxBuffer[3] = bID;  //[2]
	mTxBuffer[4] = bParameterLength + 2; //[3] //2(int) <- instruction(1int) + checksum(1int) 
	mTxBuffer[5] = bInstruction; //[4]

	offsetParamIndex = 6; //5
	bPacketLength = bParameterLength + 3 + 4; //+2+4;

        
	//copy parameters from mParamBuffer to mTxBuffer
	for (bCount = 0; bCount < bParameterLength; bCount++)
	{
		mTxBuffer[bCount + offsetParamIndex] = mParamBuffer[bCount];
	}

	// chech sum
	bCheckSum = 0;
	for (bCount = 3; bCount < bPacketLength - 1; bCount++) { //except 0xff,checksum //bCount = 2;
		bCheckSum += mTxBuffer[bCount];
	}
	mTxBuffer[bCount] = ~bCheckSum; //Writing Checksum with Bit Inversion

	HAL_HalfDuplex_EnableTransmitter(huart);
        
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
        
	HAL_UART_Transmit(huart, mTxBuffer, bPacketLength, 10);
        	
	HAL_HalfDuplex_EnableReceiver(huart);
        
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
        
	while (!(HAL_UART_GetState(huart) == HAL_UART_STATE_READY));
        
	return(bPacketLength); // return packet length
}

int rxPacket(UART_HandleTypeDef *huart, int bRxLength) {

	unsigned long ulCounter = 0, ulTimeLimit = 0;
	int bCount = 0, bLength = 0, bChecksum = 0, bChecksum2 = 0;
	
	int bTimeout;

	bTimeout = 0;
	if (bRxLength == 255 || bRxLength == 0xffff)
		ulTimeLimit = RX_TIMEOUT_COUNT1;
	else
		ulTimeLimit = RX_TIMEOUT_COUNT2;
	
	for (bCount = 0; bCount < bRxLength; bCount++)
	{
          
		ulCounter = 0;
		while (!(HAL_UART_GetState(huart) == HAL_UART_STATE_READY))
		{
			if (ulCounter++ > ulTimeLimit)
			{
				bTimeout = 1;
				break;
			}
		}
                
		if (bTimeout) 
                  break;
                
                a = HAL_UART_Receive(huart, &rx_temp, 1, 10);//this->readRaw(); // MightyZap_Serial->read(); // get packet data from USART device

                if (a == 0) {
			mRxBuffer[bCount] = rx_temp;
			b[0] += 1;
		}
		else if (a == 1)
			b[1] += 1;
		else if (a == 2)
			b[2] += 1;
		else if (a == 3)
			b[3] += 1;
	}
	bLength = bCount;
	bLength_check = bLength;
	bChecksum = 0;

	if (mTxBuffer[mPktIdIndex] != IRP_BROADCAST_ID)
	{

		if (bTimeout && bRxLength != 255)
		{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
#endif
			mIRPtxrxStatus |= (1 << COMM_RXTIMEOUT);
			;
			return 0;
		}

		if (bLength > 3) //checking available length.
		{
			if (mRxBuffer[0] != 0xff || mRxBuffer[1] != 0xff || mRxBuffer[2] != 0xff)
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);//RXHEADER); //|| mRxBuffer[2] != 0xff
			else if (mRxBuffer[mPktIdIndex] != mTxBuffer[mPktIdIndex])
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);//RXID);
			else if (mRxBuffer[mPktLengthIndex] != bLength - mPktInstIndex)
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);//RXLENGTH);
			else {
				for (bCount = 3; bCount < bLength; bCount++) { //bCount = 2
					bChecksum += mRxBuffer[bCount]; //Calculate checksum of received data for compare
					bChecksum1 = bChecksum;
				}
				if (bChecksum != 0xff)
					mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);//RXCHECKSUM);
			}


			if (mRxBuffer[0] != 0xff || mRxBuffer[1] != 0xff || mRxBuffer[2] != 0xff) { //|| mRxBuffer[2] != 0xff
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
#endif
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);//RXHEADER);
				return 0;
			}


			if (mRxBuffer[mPktIdIndex] != mTxBuffer[mPktIdIndex])  //id check
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				if (update_crc_ir(0, mRxBuffer, bRxLength - 2) == bChecksum)
#endif
					mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);
				return 0;
			}

			if (mRxBuffer[mPktLengthIndex] != bLength - mPktInstIndex) // status packet length check
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
#endif
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);
				return 0;
			}

			for (bCount = 3; bCount < bLength - 1; bCount++) { //bCount = 2
				bChecksum2 += mRxBuffer[bCount]; //Calculate checksum of received data for compare
			}

			bChecksum &= 0xff;

			if (bChecksum != 0xff)
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
#endif
				mIRPtxrxStatus |= (1 << COMM_RXCORRUPT);
				return 0;
			}
		}//(bLength > 3)
	}//end of Rx status packet check
	return bLength;
}


int txRxPacket(UART_HandleTypeDef *huart, int bID, int bInst, int bTxParaLen) {
	mIRPtxrxStatus = 0;
	int bTxLen, bRxLenEx, bTryCount;
	mBusUsed = 1;
	mRxLength = bRxLenEx = bTxLen = 0;
	
	for (bTryCount = 0; bTryCount < 1; bTryCount++)
	{
		/**************************************   Transfer packet  ***************************************************/
		bTxLen = txPacket(huart, bID, bInst, bTxParaLen);
                
		if (bTxLen == (bTxParaLen + 4 + 3))
			mIRPtxrxStatus = (1 << COMM_TXSUCCESS); //+4+2
                
		if (bInst == CMD_PING) {
			if (bID == IRP_BROADCAST_ID)	mRxLength = bRxLenEx = 0xff;
			else mRxLength = bRxLenEx = 7; //6; // basic response packet length			
		}
		else if (bInst == CMD_READ) {
			mRxLength = bRxLenEx = 7 + mParamBuffer[1]; //6+
		}
		else if (bID == IRP_BROADCAST_ID) {
			if (bInst == CMD_SYNC_READ || bInst == CMD_BULK_READ) mRxLength = bRxLenEx = 0xffff; //only 2.0 case
			else mRxLength = bRxLenEx = 0; // no response packet
		}
		else {
			if (gbIRPStatusReturnLevel > 1) {
				if (mPacketType == IRP_PACKET_TYPE1) mRxLength = bRxLenEx = 7; //6 //+mParamBuffer[1];
				else mRxLength = bRxLenEx = 11;
			}
			else {
				mRxLength = bRxLenEx = 0;
			}
		}
                
		if (bRxLenEx) {
                  if (SmartDelayFlag == 1)
				/**************************************   Receive packet  ***************************************************/
                          mRxLength = rxPacket(huart, bRxLenEx);//rxPacket(bRxLenEx);

		}//bRxLenEx is exist
                
	}
        
        
	mBusUsed = 0;
        
	if ((mRxLength != bRxLenEx) && (mTxBuffer[mPktIdIndex] != IRP_BROADCAST_ID))
	{
		
#ifdef	PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
		TxDStringC("\r\n ->[IRP]Err: ");
		printBuffer(mTxBuffer, bTxLen);
		TxDStringC("\r\n <-[IRP]Err: ");
		printBuffer(mRxBuffer, mRxLength);
#endif

#ifdef	PRINT_OUT_TRACE_ERROR_PRINT_TO_USART2
	
#endif
	}
	else if ((mRxLength == 0) && (mTxBuffer[mPktInstIndex] == CMD_PING)) {
		return 0;
	}

#ifdef PRINT_OUT_PACKET_TO_USART2
	TxDStringC("\r\n ->[TX Buffer]: ");
	printBuffer(mTxBuffer, bTxLen);
	TxDStringC("\r\n <-[RX Buffer]: ");
	printBuffer(mRxBuffer, mRxLength);
#endif
	mIRPtxrxStatus = (1 << COMM_RXSUCCESS);
	return 1;
}

void returnMode(UART_HandleTypeDef *huart, int bID, int mode) {
	writeByte(huart, bID, 16, mode);
}

int writeByte(UART_HandleTypeDef *huart, int bID, int bAddress, int bData) {
	int param_length = 0;

	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = bData;
	param_length = 2;

	return txRxPacket(huart, bID, CMD_WRITE, param_length);
}

int ping(UART_HandleTypeDef *huart, int bID) {

	if (txRxPacket(huart, bID, CMD_PING, 0)) {
		if (mPacketType == IRP_PACKET_TYPE1)
			return (mRxBuffer[3]); //1.0
		else
			return IRP_MAKEint(mRxBuffer[9], mRxBuffer[10]);
	}
	else {
		return 0xff;  //no irp in bus.
	}
}

void forceEnable(UART_HandleTypeDef *huart, int bID, int value) {
	writeByte(huart, bID, 0x80, value);
}

int forceEnable(UART_HandleTypeDef *huart, int bID) {
	return readByte(huart, bID, 0x80);
}

// start_addr  0x86(goal position), data_length : 읽어올 데이터 바이트 수 / 2 , param : 배열 형식 {id1, data, ...} , param_length : param의 배열 크기
int syncWrite(UART_HandleTypeDef *huart, int start_addr, int data_length, int *param, int param_length) {

	int i = 0, j = 0, k = 0, num = 0;
        
	num = param_length / (data_length + 1); //ID+DATA1+DATA2..
        
	if (mPacketType == IRP_PACKET_TYPE2) {

		mParamBuffer[0] = IRP_LOBYTE(start_addr);
		mParamBuffer[1] = IRP_HIBYTE(start_addr);
		mParamBuffer[2] = IRP_LOBYTE(data_length * 2);
		mParamBuffer[3] = IRP_HIBYTE(data_length * 2);

		for (i = 4; i < (4 + num * (1 + data_length * 2)); i += (1 + data_length * 2)) {
			mParamBuffer[i] = (int)param[k++]; //ID
			for (j = 0; j < (data_length * 2); j += 2) {
				mParamBuffer[i + j + 1] = IRP_LOBYTE(param[k]); //low int
				mParamBuffer[i + j + 2] = IRP_HIBYTE(param[k]); //high int
				k++;
			}
		}

		return txRxPacket(huart, IRP_BROADCAST_ID, CMD_SYNC_WRITE, i);
	}
	else {

		mbLengthForPacketMaking = 0;
		mbIDForPacketMaking = IRP_BROADCAST_ID;
		mbInstructionForPacketMaking = CMD_SYNC_WRITE;
		mCommStatus = 0;
		mParamBuffer[mbLengthForPacketMaking++] = start_addr;
		mParamBuffer[mbLengthForPacketMaking++] = data_length * 2;
		for (i = mbLengthForPacketMaking; i < num*(1 + data_length * 2); i += (1 + data_length * 2)) {
			mParamBuffer[i] = param[k++]; //ID
			for (j = 0; j < (data_length * 2); j += 2) {
				mParamBuffer[i + j + 1] = IRP_LOBYTE(param[k]); //low int
				mParamBuffer[i + j + 2] = IRP_HIBYTE(param[k]);; //high int
				k++;
			}
		}
		mbLengthForPacketMaking = i;
		return txRxPacket(huart, mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	}

}