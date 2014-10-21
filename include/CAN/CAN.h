#ifndef CAN_H_
#define CAN_H_
#include <stdint.h>
#include "loggerConfig.h"

#define CAN_BAUD_50K			50000
#define CAN_BAUD_100K 			100000
#define CAN_BAUD_125K			125000
#define CAN_BAUD_250K			250000
#define CAN_BAUD_500K			500000
#define CAN_BAUD_1M				1000000

#define CAN_MSG_SIZE 8
typedef struct _CAN_msg
{
  int isExtendedAddress;
  unsigned int addressValue;
  unsigned char dataLength;
  unsigned char data[CAN_MSG_SIZE];
}  CAN_msg;


int CAN_init(LoggerConfig *loggerConfig);
int CAN_init_port(size_t port, uint32_t baud);
int CAN_set_filter(uint8_t id, uint8_t extended, uint32_t filter, uint32_t mask);
int CAN_tx_msg(uint8_t channel, CAN_msg *msg, unsigned int timeoutMs);
int CAN_rx_msg(uint8_t channel, CAN_msg *msg, unsigned int timeoutMs);


#endif /* CAN_H_ */
