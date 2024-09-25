// test_i2c.c

#include "header.h"

#define DEVICE_ADDR 0x68 // Example device address

int main(void) {
    uint8_t data;
    I2C_Init();

    // Write 0xAA to device
    I2C_Start();
    I2C_Write((DEVICE_ADDR << 1), 0xAA); // Write operation
    I2C_Stop();

    // Read from device
    I2C_Start();
    data = I2C_Read_NACK((DEVICE_ADDR << 1)); // Read operation with NACK
    I2C_Stop();

    // Do something with the data (in this example, it's unused)
    (void)data;

    // Loop forever
    while (1);
}
