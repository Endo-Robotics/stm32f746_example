#include "Dxl.h"

uint8_t Rx_Buffer[40];
extern uint8_t rs485_flag;
extern UART_HandleTypeDef huart6;

uint8_t Dxl_ID_All[10] = {1,2,3,4,5,6,7,8,9,10};
uint32_t  Dxl_Pos_All[10];

uint8_t Dxl_ID[8] = {1,2,3,4,5,6,7,8};
uint8_t Dxl_Cover_ID[2] = {9.10};

uint8_t Dxl_ID4_Top[4] = {1,2,3,4},Dxl_ID4_Down[4] = {5,6,7,8};

int32_t  Dxl_Pos[8];
uint8_t Dxl_ReadID[10] = {0,0,0,0,0,0,0,0,0,0};

uint32_t All_Zero[10]= {0,0,0,0,0,0,0,0,0,0};
uint32_t All_One[10]= {1,1,1,1,1,1,1,1,1,1};
uint32_t All_Three[10]= {3,3,3,3,3,3,3,3,3,3};
uint32_t All_Four[10]= {4,4,4,4,4,4,4,4,4,4};
uint32_t All_Five[10] = {5,5,5,5,5,5,5,5,5,5};
uint32_t All_Ten[10] = {10,10,10,10,10,10,10,10,10,10};
uint32_t All_Fifty[10] = {50,50,50,50,50,50,50,50,50,50};
uint32_t All_Hun[10] = {100,100,100,100,100,100,100,100,100,100};

uint32_t All_Zero_2[2]= {0,0};
uint32_t All_One_2[2]= {1,1};
uint32_t All_Three_2[2] = {3,3};
uint32_t All_Four_2[2]= {4,4};
uint32_t All_Five_2[2] = {5,5};
uint32_t All_Sixteen_2[2] = {16,16};

uint8_t All_Four4[4] = {4,4,4,4};

uint32_t All_Zero_8[8]= {0,0,0,0,0,0,0,0};
uint32_t All_One_8[8]= {1,1,1,1,1,1,1,1};
uint32_t All_Four_8[8]= {4,4,4,4,4,4,4,4};

//void Dxl_Write(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, uint32_t data, uint16_t data_length);

unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
  unsigned short i, j;
  unsigned short crc_table[256] = {
          0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
          0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
          0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
          0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
          0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
          0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
          0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
          0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
          0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
          0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
          0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
          0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
          0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
          0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
          0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
          0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
          0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
          0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
          0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
          0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
          0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
          0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
          0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
          0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
          0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
          0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
          0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
          0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
          0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
          0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
          0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
          0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
  };

  for (j = 0; j < data_blk_size; j++)
  {
          i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
          crc_accum = (crc_accum << 8) ^ crc_table[i];
  }

  return crc_accum;
}

int t;
void Dxl_init(UART_HandleTypeDef *huart, uint8_t id, int mode){
  Dxl_Write(&huart6,id,TORQUE_ENABLE_ADD,TORQUE_DISABLE,DATA_BITS_ONE);                      // address : 64  토크 on/off default : 0
  HAL_Delay(3);
  if(mode == 0) {
    Dxl_Write(&huart6,id,OPERATION_MODE_ADD,OPERATION_MODE_ROTOR,DATA_BITS_ONE);               // address : 11  operating Mode default : 3
    HAL_Delay(3);
  }
  else {
    Dxl_Write(&huart6,id,OPERATION_MODE_ADD,OPERATION_MODE_VELOCITY,DATA_BITS_ONE);               // address : 11  operating Mode default : 3
    HAL_Delay(3);
  }
  
  Dxl_Write(&huart6,id,STATUS_RETURN_LEVEL_ADD,STATUS_RETURN_LEVEL_READ,DATA_BITS_ONE);      // address : 68  상태 리턴 레벨 default : 2
  HAL_Delay(3);  
 
  Dxl_Write(&huart6,id,RETURN_DELAY_TIME_ADD,RETURN_DELAY_TIME * 10,DATA_BITS_ONE);      // address : 9  return delay time : 20 micro
  HAL_Delay(3);
  
  Dxl_Write(&huart6,id,TORQUE_ENABLE_ADD,TORQUE_ENABLE,DATA_BITS_ONE);                       // address : 64  토크 on/off default : 0
  HAL_Delay(3);
}

void Dxl_ping(UART_HandleTypeDef *huart, uint8_t id){
  uint8_t *Dxl_data = (uint8_t *)malloc(10);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = id;
  Dxl_data[5] = 0x03;
  Dxl_data[6] = 0x00;
  Dxl_data[7] = PING_INSTRUCTION_ADD;
  
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, 8);
  Dxl_data[8] = (Dxl_CRC & 0x00FF);
  Dxl_data[9] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  
  HAL_HalfDuplex_EnableTransmitter(huart);
  HAL_UART_Transmit(huart, Dxl_data, 10, 10);
  free(Dxl_data);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_FactoryReset(UART_HandleTypeDef *huart, uint8_t id, uint8_t option)
{
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  uint16_t length = 0x04;
  uint8_t Dxl_data[11] = { 0xFF,0xFF,0xFD,0x00,id,(length & 0x00FF),((length >> 8) & 0x00FF),0x06, option,0x00,0x00 };
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, 9);
  Dxl_data[9] = (Dxl_CRC & 0x00FF);
  Dxl_data[10] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  HAL_UART_Transmit(huart, Dxl_data, 11, 100);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_Action(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint32_t data)
{
  uint16_t length = 0x03;
  uint8_t Dxl_data[10] = { 0xFF,0xFF,0xFD,0x00,id,(length & 0x00FF),(length >> 8) & 0x00FF,0x05,0x00,0x00 };
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, 8);
  Dxl_data[8] = (Dxl_CRC & 0x00FF);
  Dxl_data[9] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  HAL_UART_Transmit(huart, Dxl_data, 10, 100);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

////////////////////////////////// write part //////////////////////////////////
int packet_sh[20];

void Dxl_Write(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, uint32_t data, uint16_t data_length)
{
  uint16_t packet_length = data_length + 5;
  uint8_t total_length = data_length + 12;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = id;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x03;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  for (int i = 0; i < data_length; i++)
  {
          Dxl_data[10 + i] = (data >> 8 * i) & 0xFF;
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  
  for(int i = 0; i <total_length ; i++)
    packet_sh[i] = Dxl_data[i];
  
  HAL_HalfDuplex_EnableTransmitter(huart);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);  
  HAL_UART_Transmit(huart, Dxl_data, total_length,10);
  free(Dxl_data);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_WriteMinus(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, int32_t data, uint16_t data_length)
{
  uint16_t packet_length = data_length + 5;
  uint8_t total_length = data_length + 12;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = id;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x03;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  for (int i = 0; i < data_length; i++)
  {
          Dxl_data[10 + i] = (data >> 8 * i) & 0xFF;
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  
  HAL_UART_Transmit(huart, Dxl_data, total_length, 20);
  free(Dxl_data);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_RegWrite(UART_HandleTypeDef *huart, uint8_t id, uint16_t address, uint32_t data, uint16_t data_length)
{ 
  uint16_t packet_length = data_length + 5;
  uint8_t total_length = data_length + 12;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = id;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x04;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  for (int i = 0; i < data_length; i++)
  {
          Dxl_data[10 + i] = (data >> 8 * i) & 0xFF;
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  HAL_UART_Transmit_DMA(huart, Dxl_data, total_length);
  free(Dxl_data);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_SyncWrite(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, uint32_t* data_arr, uint16_t data_length, uint16_t id_length)
{
  uint16_t packet_length = (data_length + 1)*id_length + 7;
  uint8_t total_length = packet_length + 7;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = 0xFE;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x83;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  Dxl_data[10] = data_length & 0xff;
  Dxl_data[11] = (data_length >> 8) & 0xff;
  for (int i = 0; i < id_length; i++)
  {
          Dxl_data[11 + (data_length + 1)*i + 1] = id_arr[i];
          for (int j = 0; j < data_length; j++)
          {
                  Dxl_data[11 + (data_length + 1)*i + 2 + j] = (data_arr[i] >> 8 * j) & 0xFF;
          }
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
 
  HAL_UART_Transmit_IT(huart, Dxl_data, total_length);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  free(Dxl_data);
}

void Dxl_SyncWriteCover(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, int32_t* data_arr, uint16_t data_length, uint16_t id_length)
{
  uint16_t packet_length = (data_length + 1)*id_length + 7;
  uint8_t total_length = packet_length + 7;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = 0xFE;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x83;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  Dxl_data[10] = data_length & 0xff;
  Dxl_data[11] = (data_length >> 8) & 0xff;
  for (int i = 0; i < id_length; i++)
  {
          Dxl_data[11 + (data_length + 1)*i + 1] = id_arr[i];
          for (int j = 0; j < data_length; j++)
          {
                  Dxl_data[11 + (data_length + 1)*i + 2 + j] = (data_arr[i] >> 8 * j) & 0xFF;
          }
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  
  HAL_HalfDuplex_EnableTransmitter(huart);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_UART_Transmit(huart, Dxl_data, total_length, 5);
  HAL_HalfDuplex_EnableReceiver(huart);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  free(Dxl_data);
}

////////////////////////////////// read part //////////////////////////////////
uint8_t packet_sort[15],qq;

void Dxl_Read(UART_HandleTypeDef *huart, uint8_t* buf,uint8_t* id, uint32_t* data){	
  
 
  ///// Status Packet  example
  ///// 0xFF 0xFF 0xFD 0x00 0x01         0x08	0x00	0x55	0x00	0xA6	0x00	0x00	0x00	0x8C	0xC0
  
  
  //while(HAL_UART_GetState(huart) != HAL_UART_STATE_READY);
  
  /*int header_index = 0 ,before_header_index = 0;
  for(int i = 0; i < 13; i++){
    if(buf[i] == 0xFF && buf[i+1] == 0xFF && buf[i+2] == 0xFD){ 
        header_index = i;
        break;
    }
  }// header check

  
  for(int i = header_index;  i < 15; i++){
    packet_sort[before_header_index++] = buf[i];
  }// header index를 찾게 되면 그 index부터 14번까지를 packet_sort에 저장
  
  for(int i =  0; i < header_index; i++){
    packet_sort[before_header_index++] = buf[i];
  }//sort된 다음 index부터 buf의 0 번부터 채워 넣기
  */
  uint32_t length = 0, pdata = 0;
  if(buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFD){ 
    
    length = (buf[5] + (buf[6] << 8));
  
    if (length ==8){
      pdata = 0;
      for (int i = 0; i<length - 4; i++){
        pdata +=  ( (buf[9 + i] << (8 * i)) );
      }
      uint32_t check_crc = update_crc(0, &buf[0], length + 5) & 0xFFFF;
      if (check_crc == (buf[0+length+5] + ( buf[0+length+6] << 8 )) ){
        *id = buf[4];
        *data = pdata;
      }
    }  
  }
  
  /*length = (packet_sort[5] + (packet_sort[6] << 8));
  
  if (length ==8){
    pdata = 0;
    for (int i = 0; i<length - 4; i++){
      pdata +=  ( (packet_sort[9 + i] << (8 * i)) );
    }
    uint32_t check_crc = update_crc(0, &packet_sort[0], length + 5) & 0xFFFF;
    if (check_crc == (packet_sort[0+length+5] + ( packet_sort[0+length+6] << 8 )) ){
      *id = packet_sort[4];
      *data = pdata;
    }
  }*/
  
  //while(HAL_UART_GetState(huart) != HAL_UART_STATE_READY);		
  
  
}

void Dxl_Read_DMA(uint8_t* buf, uint8_t size, uint8_t* id, int32_t* data){
  
  int32_t length = 0, pdata = 0; 
  uint8_t tmp_arr[15] = {0,};
  int tmp = 0;
    
  if(buf[0] != 0xFF && buf[1] != 0xFF && buf[2] != 0xFD){ 
    for(int i=0; i<15; i++){
      if(buf[i] == 0xFF){
       tmp = i;
       break;
      }
    }
    for(int i=0; i<15; i++){
      if(tmp< 15)
        tmp_arr[i] = buf[tmp];
      else{
        tmp = 0;
        tmp_arr[i] = buf[tmp];
      }
      tmp++;
    }
    for(int i=0; i<15; i++){
      buf[i] = tmp_arr[i];
    }
  } 

  if(buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFD){ 
    
    length = (buf[5] + (buf[6] << 8));
  
    if (length == 8){
      for (int i = 0; i<length - 4; i++){
        pdata +=  ( (buf[9 + i] << (8 * i)) );
      }
      uint32_t check_crc = update_crc(0, &buf[0], length + 5) & 0xFFFF;
      if (check_crc == (buf[0+length+5] + (buf[0+length+6] << 8 )) ){
        *id = buf[4];
        *(data + buf[4]) = pdata;
      }
    }  
  }
}

void Dxl_Read_Interface(uint8_t* buf, uint8_t size, uint8_t* id, int32_t* data){	

  int32_t length = 0, pdata = 0;  
  
  length = (buf[5] + (buf[6] << 8));
  if (length == 7){
    for (int i = 0; i<length - 5; i++){
      pdata +=  ( (buf[10 + i] << (8 * i)) );
      //show = pdata;
    }
    uint32_t check_crc = update_crc(0, &buf[0], 12) & 0xFFFF;
    //show2 = check_crc;
    if (check_crc == (buf[12] + (buf[13] << 8 )) ){
      *id = buf[4];
      *data = pdata;
    }
  }  
  
}

void Dxl_Read_Interface2(uint8_t* buf, uint8_t size, uint8_t* id, int32_t* data){	

  int header_index = 0;
  
  for(int i = 0; i <15; i++){
    if(buf[i] == 0xFF && buf[i+1] == 0xFF && buf[i+2] == 0xFD){
      header_index = i;
      break;
    }
    else
      header_index = -1;
  }
    
  uint32_t length = 0, pdata = 0;
  
  if(buf[header_index] == 0xFF && buf[header_index+1] == 0xFF && buf[header_index+2] == 0xFD){ 
    
      length = (buf[header_index+5] + (buf[header_index+6] << 8));                //7
  
      pdata = 0;
      for(int i = 0; i<length - 5; i++){
        pdata +=  ( (buf[header_index + 10 + i] << (8 * i)) );
      }
      uint32_t check_crc = update_crc(0, &buf[header_index], length + 5) & 0xFFFF;
      if (check_crc == (buf[header_index + 0+length+5] + ( buf[header_index +0+length+6] << 8 )) ){
        *id = buf[header_index + 4];
        *data = pdata;
      }
  }
}

uint8_t tq[14];

void Dxl_ReadInstruction(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint8_t data_length)
{
    while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
    uint16_t length = 0x07;
    uint8_t Dxl_data[14] = { 0xFF,0xFF,0xFD,0x00,id,(length & 0x00FF),(length >> 8) & 0x00FF,0x02, (address & 0x00FF), ((address >> 8) & 0x00FF), (data_length & 0x00FF), ((data_length >> 8) & 0x00FF),0x00,0x00 };
    uint16_t Dxl_CRC = update_crc(0, Dxl_data, 12);
    Dxl_data[12] = (Dxl_CRC & 0x00FF);
    Dxl_data[13] = ((Dxl_CRC >> 8) & 0x00FF);
    
    for(int i = 0; i<14;i++)
      tq[i] = Dxl_data[i];
      
    HAL_HalfDuplex_EnableTransmitter(huart);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_UART_Transmit(huart, tq, sizeof(tq),5);
    HAL_HalfDuplex_EnableReceiver(huart);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
    //HAL_UART_Transmit_IT(huart, hh, 14);
    while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
}

void Dxl_ReadInstruction2(UART_HandleTypeDef *huart, uint8_t id, uint8_t address, uint8_t data_length)
{
    uint8_t hh[14] = {0xff,0xff,0xfd,0x00,0x00,0x07,0x00,0x02,0x84,0x00,0x04,0x00,0x1b,0x05};
    HAL_UART_Transmit_IT(huart, hh, 14);
}

void Dxl_SyncReadInstruction(UART_HandleTypeDef *huart, uint8_t* id_arr, uint8_t address, uint8_t data_length, uint16_t id_length)
{
  while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY);
  uint16_t packet_length = id_length + 7;
  uint8_t total_length = packet_length + 7;
  uint8_t *Dxl_data = (uint8_t *)malloc(total_length);
  Dxl_data[0] = 0xFF;
  Dxl_data[1] = 0xFF;
  Dxl_data[2] = 0xFD;
  Dxl_data[3] = 0x00;
  Dxl_data[4] = 0xFE;
  Dxl_data[5] = packet_length & 0xff;
  Dxl_data[6] = (packet_length >> 8) & 0xff;
  Dxl_data[7] = 0x82;
  Dxl_data[8] = address & 0xff;
  Dxl_data[9] = (address >> 8) & 0xff;
  Dxl_data[10] = data_length & 0xff;
  Dxl_data[11] = (data_length >> 8) & 0xff;
  for (int i = 0; i < id_length; i++)
  {
          Dxl_data[12 + i] = id_arr[i];
  }
  uint16_t Dxl_CRC = update_crc(0, Dxl_data, total_length - 2);
  Dxl_data[total_length - 2] = (Dxl_CRC & 0x00FF);
  Dxl_data[total_length - 1] = ((Dxl_CRC >> 8) & 0x00FF);
  //HAL_UART_Transmit_DMA(huart, Dxl_data, total_length);
  HAL_UART_Transmit(huart, Dxl_data, total_length, 20);
  free(Dxl_data);
}

/////////////////////////////////////////////////////////

void Dxl_ChangeEEPROM(UART_HandleTypeDef *huart, uint8_t id, uint8_t address ,uint32_t data){
    Dxl_Write(huart,id,64,0,1);
    HAL_Delay(3);
    Dxl_Write(huart,id,address,data,1);
    HAL_Delay(3);    
    Dxl_Write(huart,id,64,1,1);	
}

void Dxl_SyncChangeEEPROM(UART_HandleTypeDef *huart, uint8_t* id, uint8_t address ,uint8_t* data, uint16_t id_length){
    
  for(int i = 0; i < id_length; i++){
    Dxl_Write(huart,id[i],64,0,1);
    HAL_Delay(3);        
    Dxl_Write(huart,id[i],address,data[i],1);
    HAL_Delay(3);
    Dxl_Write(huart,id[i],64,1,1);	
    HAL_Delay(3);
  }
}