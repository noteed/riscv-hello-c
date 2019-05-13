// See LICENSE for license details.

#include <stdio.h>

enum {
    /* UART Registers */
    UART_REG_TXFIFO = 0,
};

static volatile int *uart = (int *)(void *)0x10013000;

int putchar(int ch)
{
    while (uart[UART_REG_TXFIFO] < 0);
    return uart[UART_REG_TXFIFO] = ch & 0xff;
}
