#ifndef LSM6DS3_H_INC
#define LSM6DS3_H_INC

#include <stdbool.h>

#define LSM6DS3_SPI_BAUDRATE 1000000
#define SPI_RX_DMA_CHANNEL 0
#define SPI_TX_DMA_CHANNEL 1
#define SPI_BUFFER_SIZE 14


void initLSM6DS3(int spi_baudrate);

void writeRegister(uint8_t address, uint8_t value);

uint8_t readRegister(uint8_t address);
void readRegistersBlocking(uint8_t startAddress, uint8_t bytesCount, uint8_t * rxBuffer);

// Discard the first byte of the RX buffer
void startRegistersRead(uint8_t startAddress, uint8_t bytesToReadCount, uint8_t * junkByteAndThenRxBuffer);

bool imuCommunicationInProgress(void);

#endif /* LSM6DS3_H_INC */
