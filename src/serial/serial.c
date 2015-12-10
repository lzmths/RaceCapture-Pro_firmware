/*
 * Race Capture Pro Firmware
 *
 * Copyright (C) 2015 Autosport Labs
 *
 * This file is part of the Race Capture Pro fimrware suite
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should
 * have received a copy of the GNU General Public License along with
 * this code. If not, see <http://www.gnu.org/licenses/>.
 */

#include "modp_numtoa.h"
#include "printk.h"
#include "serial.h"
#include "usart.h"
#include "usb_comm.h"

static Serial serial_ports[SERIAL_COUNT];

void init_serial(void)
{
        usart_init_serial(&serial_ports[SERIAL_GPS], UART_GPS);
        usart_init_serial(&serial_ports[SERIAL_TELEMETRY], UART_TELEMETRY);
        usart_init_serial(&serial_ports[SERIAL_WIRELESS], UART_WIRELESS);
        usart_init_serial(&serial_ports[SERIAL_AUX], UART_AUX);
        usb_init_serial(&serial_ports[SERIAL_USB]);
}

void configure_serial(serial_id_t port, uint8_t bits, uint8_t parity, uint8_t stopBits, uint32_t baud)
{
        uart_id_t uartPort;

        switch(port) {
        case SERIAL_GPS:
                uartPort = UART_GPS;
                break;
        case SERIAL_TELEMETRY:
                uartPort = UART_TELEMETRY;
                break;
        case SERIAL_WIRELESS:
                uartPort = UART_WIRELESS;
                break;
        case SERIAL_AUX:
                uartPort = UART_AUX;
                break;
        default:
                return;
        }

        /* If here, we have a real USART port to configure */
        usart_config(uartPort, bits, parity, stopBits, baud);
}

Serial* get_serial(serial_id_t port)
{
        return port < SERIAL_COUNT ? &serial_ports[port] : NULL;
}

static void do_rx_callback(const Serial *s, const char *data)
{
        if (s->rx_callback)
                s->rx_callback(data);
}

static void do_tx_callback(const Serial *s, const char *data)
{
        if (s->tx_callback)
                s->tx_callback(data);
}

static void do_rx_callback_char(const Serial *s, const char data)
{
        const char str[] = { data, '\0' };
        do_rx_callback(s, str);
}

static void do_tx_callback_char(const Serial *s, const char data)
{
        const char str[] = { data, '\0' };
        do_tx_callback(s, str);
}

char serial_get_c(const Serial *s)
{
        const char c = s->get_c();
        do_rx_callback_char(s, c);
        return c;
}

int serial_get_c_wait(const Serial *s, char *c, const size_t delay)
{
        const int rv = s->get_c_wait(c, delay);
        do_rx_callback_char(s, *c);
        return rv;
}

int serial_get_line(const Serial *s, char *l, const int len)
{
        const int rv = s->get_line(l, len);
        do_rx_callback(s, l);
        return rv;
}

int serial_get_line_wait(const Serial *s, char *l, const int len,
                         const size_t delay)
{
        const int rv = s->get_line_wait(l, len, delay);
        do_rx_callback(s, l);
        return rv;
}

void serial_put_c(const Serial *s, const char c)
{
        do_tx_callback_char(s, c);
        s->put_c(c);
}

void serial_put_s(const Serial *s, const char *l)
{
        do_tx_callback(s, l);
        s->put_s(l);
}

size_t serial_read_byte(const Serial *serial, uint8_t *b, size_t delay)
{
        return serial_get_c_wait(serial, (char*) b, delay);
}

void put_int(Serial *serial, int n)
{
        char buf[12];
        modp_itoa10(n, buf);
        serial_put_s(serial, buf);
}

void put_ll(Serial *serial, long long l)
{
        char buf[22];
        modp_ltoa10(l, buf);
        serial_put_s(serial, buf);
}

void put_float(Serial *serial, float f,int precision)
{
        char buf[20];
        modp_ftoa(f, buf, precision);
        serial_put_s(serial, buf);
}

void put_double(Serial *serial, double f, int precision)
{
        char buf[30];
        modp_dtoa(f,buf,precision);
        serial_put_s(serial, buf);
}

void put_hex(Serial *serial, int n)
{
        char buf[30];
        modp_itoaX(n,buf,16);
        serial_put_s(serial, buf);
}

void put_uint(Serial *serial, unsigned int n)
{
        char buf[20];
        modp_uitoa10(n,buf);
        serial_put_s(serial, buf);
}

void put_nameIndexUint(Serial *serial, const char *s, int i, unsigned int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        put_uint(serial, i);
        serial_put_s(serial, "=");
        put_uint(serial, n);
        serial_put_s(serial, ";");
}

void put_nameSuffixUint(Serial *serial, const char *s, const char *suf, unsigned int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        serial_put_s(serial, suf);
        serial_put_s(serial, "=");
        put_uint(serial, n);
        serial_put_s(serial, ";");
}

void put_nameUint(Serial *serial, const char *s, unsigned int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=");
        put_uint(serial, n);
        serial_put_s(serial, ";");
}

void put_nameIndexInt(Serial *serial, const char *s, int i, int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        put_uint(serial, i);
        serial_put_s(serial, "=");
        put_int(serial, n);
        serial_put_s(serial, ";");
}

void put_nameSuffixInt(Serial *serial, const char *s, const char *suf, int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        serial_put_s(serial, suf);
        serial_put_s(serial, "=");
        put_int(serial, n);
        serial_put_s(serial, ";");
}

void put_nameInt(Serial *serial, const char *s, int n)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=");
        put_int(serial, n);
        serial_put_s(serial, ";");
}

void put_nameIndexDouble(Serial *serial, const char *s, int i, double n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        put_uint(serial, i);
        serial_put_s(serial, "=");
        put_double(serial, n,precision);
        serial_put_s(serial, ";");
}

void put_nameSuffixDouble(Serial *serial, const char *s, const char *suf, double n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        serial_put_s(serial, suf);
        serial_put_s(serial, "=");
        put_double(serial, n,precision);
        serial_put_s(serial, ";");
}

void put_nameDouble(Serial *serial, const char *s, double n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=");
        put_double(serial, n, precision);
        serial_put_s(serial, ";");
}

void put_nameIndexFloat(Serial *serial, const char *s, int i, float n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        put_uint(serial, i);
        serial_put_s(serial, "=");
        put_float(serial, n, precision);
        serial_put_s(serial, ";");
}

void put_nameSuffixFloat(Serial *serial, const char *s, const char *suf, float n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        serial_put_s(serial, suf);
        serial_put_s(serial, "=");
        put_float(serial, n, precision);
        serial_put_s(serial, ";");
}

void put_nameFloat(Serial *serial, const char *s, float n, int precision)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=");
        put_float(serial, n, precision);
        serial_put_s(serial, ";");
}

void put_nameString(Serial *serial, const char *s, const char *v)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=\"");
        serial_put_s(serial, v);
        serial_put_s(serial, "\";");
}

void put_nameSuffixString(Serial *serial, const char *s, const char *suf, const char *v)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        serial_put_s(serial, suf);
        serial_put_s(serial, "=\"");
        serial_put_s(serial, v);
        serial_put_s(serial, "\";");
}

void put_nameIndexString(Serial *serial, const char *s, int i, const char *v)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "_");
        put_uint(serial, i);
        serial_put_s(serial, "=\"");
        serial_put_s(serial, v);
        serial_put_s(serial, "\";");
}

void put_escapedString(Serial * serial, const char *v, int length)
{
        const char *value = v;
        while (value - v < length) {
                switch(*value) {
                case '\n':
                        serial_put_s(serial, "\\n");
                        break;
                case '\r':
                        serial_put_s(serial, "\\r");
                        break;
                case '"':
                        serial_put_s(serial, "\\\"");
                        break;
                default:
                        serial_put_c(serial, *value);
                        break;
                }
                value++;
        }
}

void put_nameEscapedString(Serial *serial, const char *s, const char *v, int length)
{
        serial_put_s(serial, s);
        serial_put_s(serial, "=\"");
        const char *value = v;
        while (value - v < length) {
                switch(*value) {
                case ' ':
                        serial_put_s(serial, "\\_");
                        break;
                case '\n':
                        serial_put_s(serial, "\\n");
                        break;
                case '\r':
                        serial_put_s(serial, "\\r");
                        break;
                case '"':
                        serial_put_s(serial, "\\\"");
                        break;
                default:
                        serial_put_c(serial, *value);
                        break;
                }
                value++;
        }
        serial_put_s(serial, "\";");
}


void put_bytes(Serial *serial, char *data, unsigned int length)
{
        while (length > 0) {
                serial_put_c(serial, *data);
                data++;
                length--;
        }
}

void put_crlf(const Serial *serial)
{
        serial_put_s(serial, "\r\n");
}

void read_line(Serial *serial, char *buffer, size_t bufferSize)
{
        size_t bufIndex = 0;
        char c;
        while(bufIndex < bufferSize - 1) {
                c = serial_get_c(serial);
                if (c) {
                        if ('\r' == c) {
                                break;
                        } else {
                                buffer[bufIndex++] = c;
                        }
                }
        }
        buffer[bufIndex]='\0';
}

void interactive_read_line(Serial *serial, char * buffer, size_t bufferSize)
{
        size_t bufIndex = 0;
        char c;
        while(bufIndex < bufferSize - 1) {
                c = serial_get_c(serial);
                if (c) {
                        if ('\r' == c) {
                                break;
                        } else if ('\b' == c) {
                                if (bufIndex > 0) {
                                        bufIndex--;
                                        serial_put_c(serial, c);
                                }
                        } else {
                                serial_put_c(serial, c);
                                buffer[bufIndex++] = c;
                        }
                }
        }
        serial_put_s(serial, "\r\n");
        buffer[bufIndex]='\0';
}
