#include <runtime.h>
#include <io.h>

#define BASE 0x3f8

void serial_init()
{
    out8(BASE+3, 0x80); // dlab
    out8(BASE+0, 0x0c); // 9600
    out8(BASE+1, 0x0); // divisor latch
    out8(BASE+3, 0x3); // 8n1
    out8(BASE+2, 0xc7); // fifo control
    out8(BASE+5, 0xc7); // dtr, rts, out2
}

static boolean is_transmit_empty() {
    return in8(BASE + 5) & 0x20;
}

void serial_out(char a)
{
    while (!is_transmit_empty());
    out8(BASE, a);
}

static char hex[]="0123456789abcdef";

void print_u64(u64 s)
{
    for (int x = 60; x >= 0; x -= 4)
        serial_out(hex[(s >> x)&0xf]);
}

void console(char *x)
{
    for (char *i = x; *i; i++) 
        serial_out(*i);
}

void debug(buffer b)
{
    foreach_character(i, b) serial_out(i);
}

