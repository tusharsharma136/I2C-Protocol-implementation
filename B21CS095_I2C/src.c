// i2c.c

#include "header.h"
#include <stm32f4xx.h>

// Private function prototypes
static void delay(void);

void I2C_Init(void) {
    // Enable the clock for GPIOB and I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Configure PB6 and PB7 as alternate function (AF4 - I2C1)
    GPIOB->MODER |= (2U << GPIO_MODER_MODER6_Pos) | (2U << GPIO_MODER_MODER7_Pos);
    GPIOB->AFR[0] |= (4U << GPIO_AFRL_AFSEL6_Pos) | (4U << GPIO_AFRL_AFSEL7_Pos);

    // Configure I2C1
    I2C1->CR1 &= ~I2C_CR1_PE; // Disable I2C1
    I2C1->CR2 |= 16;           // Set peripheral clock to 16 MHz
    I2C1->CCR = 80;            // Set clock control register (100 kHz, assuming APB1 at 16 MHz)
    I2C1->TRISE = 17;          // Set maximum rise time
    I2C1->CR1 |= I2C_CR1_PE;   // Enable I2C1
}

void I2C_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;           // Generate START condition
    while (!(I2C1->SR1 & I2C_SR1_SB));    // Wait for start bit to be set
}

void I2C_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;            // Generate STOP condition
    while (I2C1->CR1 & I2C_CR1_STOP);     // Wait for STOP condition to clear
}

void I2C_Write(uint8_t address, uint8_t data) {
    // Send address with write intent
    I2C1->DR = address & 0xFE;            // Mask the read/write bit
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // Wait for address acknowledgment
    (void)I2C1->SR1;                      // Clear ADDR flag by reading SR1 and SR2
    (void)I2C1->SR2;

    // Send data
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));   // Wait for byte transfer to finish
}

uint8_t I2C_Read_ACK(void) {
    I2C1->CR1 |= I2C_CR1_ACK;             // Enable acknowledgment
    while (!(I2C1->SR1 & I2C_SR1_RXNE));  // Wait for RX buffer to be full
    return I2C1->DR;
}

uint8_t I2C_Read_NACK(void) {
    I2C1->CR1 &= ~I2C_CR1_ACK;            // Disable acknowledgment
    while (!(I2C1->SR1 & I2C_SR1_RXNE));  // Wait for RX buffer to be full
    return I2C1->DR;
}

static void delay(void) {
    for (volatile uint32_t i = 0; i < 1000; i++) {
        __asm__("nop");
    }
}
