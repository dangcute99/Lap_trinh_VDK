/*******************************************************
This program was created by the
CodeWizardAVR V3.14 Advanced
Automatic Program Generator
? Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project :
Version :
Date    : 27/09/2023
Author  :Nguyendang
Company :
Comments:


Chip type               : ATmega128
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 1024
*******************************************************/

#include <mega128.h>

#include <delay.h>

// I2C Bus functions
#include <i2c.h>
#include <string.h>
#include <stdio.h>
// DS1307 Real Time Clock functions
#include <ds1307.h>

// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
char hienthi[] = "chung";
char lcd[20];
char enthernet[100];
char AC1[4];
char IP[20] = "172.20.10.20";
char sdt_sms[11] = "0942467684";
char sdt_call[11] = "0942467684";
int sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9;
int sdt_sms1, sdt_sms2, sdt_sms3, sdt_sms4, sdt_sms5, sdt_sms6, sdt_sms7, sdt_sms8, sdt_sms9;
char tram[] = "Phong Lab TTBD";
char sim[100];
char sms[100];
int i, hang = -1;
int cot = 1;
unsigned int DC1;
unsigned int DC2;
int dc11, dc12;
int dht = 0;
int dc21, dc22;
int dc_low_1 = 0, dc_low_2 = 0, dc_high = 0;
int dc_low_11 = 0, dc_low_21 = 0, dc_high1 = 0;
unsigned char hour, min, sec, day, month, year, week_day;
unsigned int h11 = 0, h12 = 0, hour1, m11 = 0, m12 = 0, min1, d1, d2, day1 = 0, m1, m2, month1 = 0, y1, y2, year1 = 0;
bit bit_phim = 1;
bit bit_temp, bit_humi;
bit bit_canh_bao = 0;
bit bit_coi = 1;
bit bit_ac1, bit_ac2;
bit bit_as;
bit bit_dc1_cb1 = 0, bit_dc1_cb2 = 0;
bit bit_dc2_cb1 = 0, bit_dc2_cb2 = 0;
bit bit_enthernet = 1;
bit dht_bit = 1;
int doam1, doam2, nhietdo1, nhietdo2;
int doam11, doam22, nhietdo11, nhietdo22;
int dht_nhiet_do, dht_do_am;
//
#define Mode_pin PINE .4
#define Up_pin PINE .5
#define Down_pin PINE .6
#define Shift_pin PINE .3
#define Ent_pin PINE .7
#define KTAC1 PINB .2
#define KTAC2 PINB .3
#define AS PINB .4
#define KHOI PINF .3
#define led_xanh PORTD .7
#define led_vang PORTD .6
#define led_do PORTD .5
#define coi PORTC .4

// External Interrupt 7 service routine
int DHT22_getdata(int *tem, int *humi)
{

   int buffer[5] = {0, 0, 0, 0, 0};
   int ii, i, checksum;
   // request data

   DDRF = (1 << DDF6);
   PORTF = (1 << PORTF6);
   delay_us(50);
   PORTF = (0 << PORTF6);
   delay_ms(25);
   PORTF = (1 << PORTF6);
   DDRF = (0 << DDF6);
   // response
   dht = 0;
   while (PINF .6 == 1)
   {
      if (dht > 40)
      {
         PORTB .6 = 1;
         dht_bit = 0;
         break;
      }
   }
   while (PINF .6 == 0)
      if (dht_bit == 0)
         break;
   dht = 0;
   while (PINF .6 == 1)

      if (dht > 40)
      {
         dht_bit = 0;
         break;
      }
   while (PINF .6 == 0)
      if (dht_bit == 0)
         break;

   // received data
   if (dht_bit == 1)
   {
      for (i = 0; i < 5; i++)
      {
         for (ii = 0; ii < 8; ii++)
         {
            while (PINF .6 == 0)
               ; // doi data len 1
            delay_us(50);
            if (PINF .6 == 1)
               buffer[i] = (buffer[i] << 1) | (0x01);
            else
               buffer[i] = (buffer[i] << 1);
            dht = 0;
            while (PINF .6 == 1)

               if (dht > 40)
                  break;
            // doi data xuong 0
         }
      }
      // check sum
      checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
      // kiem tra
      if ((checksum) != buffer[4])
         return 0;
      // lay du lieu
      *tem = buffer[2] * 256 + buffer[3];
      *humi = buffer[0] * 256 + buffer[1];
      dht_bit = 1;
      return 1;
   }
}

interrupt[EXT_INT7] void ext_int7_isr(void)
{
   // Place your code here
   if ((bit_canh_bao == 1) & (bit_coi == 1))
   {
      bit_coi = 0;
   }
}

#define DATA_REGISTER_EMPTY (1 << UDRE0)
#define RX_COMPLETE (1 << RXC0)
#define FRAMING_ERROR (1 << FE0)
#define PARITY_ERROR (1 << UPE0)
#define DATA_OVERRUN (1 << DOR0)

// Get a character from the USART1 Receiver
#pragma used +
char getchar1(void)
{
   unsigned char status;
   char data;
   while (1)
   {
      while (((status = UCSR1A) & RX_COMPLETE) == 0)
         ;
      data = UDR1;
      if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0)
         return data;
   }
}
#pragma used -

// Write a character to the USART1 Transmitter
#pragma used +
void putchar1(char c)
{
   while ((UCSR1A & DATA_REGISTER_EMPTY) == 0)
      ;
   UDR1 = c;
}
#pragma used -

// Standard Input/Output functions
#include <stdio.h>

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0 << REFS1) | (0 << REFS0) | (0 << ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
   ADMUX = adc_input | ADC_VREF_TYPE;
   // Delay needed for the stabilization of the ADC input voltage
   delay_us(10);
   // Start the AD conversion
   ADCSRA |= (1 << ADSC);
   // Wait for the AD conversion to complete
   while ((ADCSRA & (1 << ADIF)) == 0)
      ;
   ADCSRA |= (1 << ADIF);
   return ADCW;
}
#pragma used +
void puts1(unsigned char chuoi[])
{
   for (i = 0; chuoi[i] != '\0'; i++)
   {
      while ((UCSR1A & DATA_REGISTER_EMPTY) == 0)
         ;
      UDR1 = chuoi[i];
   }
}
#pragma used -

void khoitaosim()
{
   lcd_gotoxy(0, 0);
   lcd_puts("khoi tao sim...");
   delay_ms(50);
   puts("AT");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("ATE0");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("AT+IPR=9600");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("AT&W");
   putchar(13);
   putchar(10);
   delay_ms(200); // LUU LAI CAC CAI DAT DA CAI
   puts("ATX4");
   putchar(13);
   putchar(10);
   delay_ms(200); // LENH CHO CUOC GOI
   puts("AT&W");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("AT+CSCS=\"GSM\"");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("AT+CMGF=1");
   putchar(13);
   putchar(10);
   delay_ms(200); // DUA TIN NHAN TRA VE VE DANG TEXT
   puts("AT+CNMI=2,1,0,0,0");
   putchar(13);
   putchar(10);
   delay_ms(200);
   puts("AT+CSAS");
   putchar(13);
   putchar(10);
   delay_ms(200); // luu cac cai dat
   puts("AT+CNMI=2,2");
   putchar(13);
   putchar(10);
   delay_ms(300);
   lcd_clear();
}
void call(char sdt[])
{
   sprintf(sim, "ATD;%s%c%c", sdt, 13, 10);
   puts(sim);
   delay_ms(500);
}
void send_sms(char sdt[], char text[])
{
   lcd_clear();
   delay_ms(500);
   sprintf(sim, "AT+CMGS=%c%s%c%c%c", '"', sdt, '"', 13, 10);
   puts(sim);
   delay_ms(500);
   sprintf(sim, "%s %c%c", text, 13, 10);
   puts(sim);
   delay_ms(200);
   putchar(26);
}
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
   /* Wait for completion of previous write */
   while (EECR & (1 << EEWE))
      ;
   /* Set up address and data registers */
   EEAR = uiAddress;
   EEDR = ucData;
   /* Write logical one to EEMWE */
   EECR |= (1 << EEMWE);
   /* Start eeprom write by setting EEWE */
   EECR |= (1 << EEWE);
}
unsigned char EEPROM_read(unsigned int uiAddress)
{
   /* Wait for completion of previous write */
   while (EECR & (1 << EEWE))
      ;
   /* Set up address register */
   EEAR = uiAddress;
   /* Start eeprom read by writing EERE */
   EECR |= (1 << EERE);
   /* Return data from data register */
   return EEDR;
}
void trohang()
{
   for (i = 0; i < 4; i++)
      if (i == hang)
      {
         lcd_gotoxy(19, i);
         lcd_putchar('<');
      }
      else
      {
         lcd_gotoxy(19, i);
         lcd_putchar(' ');
      }
}
// int DHT22_getdata(int *tem, int *humi)
// {

//    int buffer[5] = {0, 0, 0, 0, 0};
//    int ii, i, checksum;
//    // request data
//    DDRF = (1 << DDF6);
//    PORTF = (1 << PORTF6);
//    delay_us(50);
//    PORTF = (0 << PORTF6);
//    delay_ms(25);
//    PORTF = (1 << PORTF6);
//    DDRF = (0 << DDF6);
//    // response
//    while (PINF .6 == 1)
//       ;
//    while (PINF .6 == 0)
//       ;
//    while (PINF .6 == 1)
//       ;
//    while (PINF .6 == 0)
//       ;
//    // received data
//    for (i = 0; i < 5; i++)
//    {
//       for (ii = 0; ii < 8; ii++)
//       {
//          while (PINF .6 == 0)
//             ; // doi data len 1
//          delay_us(50);
//          if (PINF .6 == 1)
//             buffer[i] = (buffer[i] << 1) | (0x01);
//          else
//             buffer[i] = (buffer[i] << 1);
//          while (PINF .6 == 1)
//             ; // doi data xuong 0
//       }
//    }
//    // check sum
//    checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
//    // kiem tra
//    if ((checksum) != buffer[4])
//       return 0;
//    // lay du lieu
//    *tem = buffer[2] * 256 + buffer[3];
//    *humi = buffer[0] * 256 + buffer[1];
//    return 1;
// }

void ct_hienthichung(void)
{
   delay_ms(1);
   rtc_get_date(&week_day, &day, &month, &year);
   delay_ms(1);
   rtc_get_time(&hour, &min, &sec);
   delay_ms(1);
   // DHT22_getdata(&dht_nhiet_do, &dht_do_am);
   delay_ms(1);
   if (strcmp(hienthi, "chung") == 0)
   {
      doam11 = EEPROM_read(0);
      doam22 = EEPROM_read(1);
      nhietdo11 = EEPROM_read(2);
      nhietdo22 = EEPROM_read(3);
      h11 = hour / 10;
      h12 = hour % 10;
      m11 = min / 10;
      m12 = min % 10;
      d1 = day / 10;
      d2 = day % 10;
      m1 = month / 10;
      m2 = month % 10;
      y1 = year / 10;
      y2 = year % 10;
      if (doam11 > 99)
      {
         doam11 = 40;
         EEPROM_write(0, doam11);
      }
      if (doam22 > 99)
      {
         doam22 = 70;
         EEPROM_write(1, doam22);
      }
      if (nhietdo11 > 99)
      {
         nhietdo11 = 20;
         EEPROM_write(2, nhietdo11);
      }
      if (nhietdo22 > 99)
      {
         nhietdo22 = 25;
         EEPROM_write(3, nhietdo22);
      }
      lcd_gotoxy(0, 0);
      sprintf(lcd, "DC1:%2d%c%dv   %1d%1d:%1d%1d:%1d%1d", dc11, ',', dc12, hour / 10, hour % 10, min / 10, min % 10, sec / 10, sec % 10);
      lcd_puts(lcd);
      delay_us(50);
      lcd_gotoxy(0, 1);
      sprintf(lcd, "DC2:%2d%c%dv   %1d%1d/%1d%1d/%2d", dc21, ',', dc22, day / 10, day % 10, month / 10, month % 10, year);
      lcd_puts(lcd);
      delay_us(50);
      lcd_gotoxy(0, 2);
      if (KTAC1 == 0)
      {
         strcpy(AC1, "OFF");
      }
      else
      {
         strcpy(AC1, "ON");
      }
      // DHT22_getdata(&dht_nhiet_do, &dht_do_am);
      sprintf(lcd, "Humi:%2d%c%d%%   AC1:%3s", dht_do_am / 10, ',', dht_do_am % 10, AC1);
      lcd_puts(lcd);
      if (KTAC2 == 0)
      {
         strcpy(AC1, "OFF");
      }
      else
      {
         strcpy(AC1, "ON");
      }
      delay_us(50);
      lcd_gotoxy(0, 3);
      sprintf(lcd, "Temp:%2d%c%d%cC  AC2:%3s", dht_nhiet_do / 10, ',', dht_nhiet_do % 10, 0xdf, AC1);
      lcd_puts(lcd);
      delay_us(50);
   }
   if (strcmp(hienthi, "Mode") == 0)
   {
      dc_low_1 = EEPROM_read(31) * 10 + EEPROM_read(32);
      dc_low_2 = EEPROM_read(33) * 10 + EEPROM_read(34);
      dc_high = EEPROM_read(35) * 10 + EEPROM_read(36);
      if ((dc_low_1 > 999) | (dc_low_2 > 999) | (dc_high > 999))
      {
         dc_low_1 = 470;
         dc_low_2 = 480;
         dc_high = 585;
         EEPROM_write(31, dc_low_1 / 10);
         EEPROM_write(32, dc_low_1 % 10);
         EEPROM_write(33, dc_low_2 / 10);
         EEPROM_write(34, dc_low_2 % 10);
         EEPROM_write(35, dc_high / 10);
         EEPROM_write(36, dc_high % 10);
      }
      trohang();
      lcd_gotoxy(0, 0);
      sprintf(lcd, "time:    %1d%1d:%1d%1d", h11, h12, m11, m12);
      lcd_puts(lcd);
      lcd_gotoxy(0, 1);
      sprintf(lcd, "date:    %1d%1d/%1d%1d/%2d", d1, d2, m1, m2, y1 * 10 + y2);
      lcd_puts(lcd);
      lcd_gotoxy(0, 2);
      sprintf(lcd, "humi: %1d%1d to %1d%1d%%", doam11 / 10, doam11 % 10, doam22 / 10, doam22 % 10);
      lcd_puts(lcd);
      lcd_gotoxy(0, 3);
      sprintf(lcd, "temp: %1d%1d to %1d%1d%cC", nhietdo11 / 10, nhietdo11 % 10, nhietdo22 / 10, nhietdo22 % 10, 0xdf);
      lcd_puts(lcd);
      // hang 1
      if (hang == 0)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(9, 0);
            delay_ms(200);
            lcd_putchar('_');
            // phim up

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               h11++;
               if (h11 > 2)
                  h11 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               h11--;
               if (h11 == -1)
                  h11 = 2;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(10, 0);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               h12++;
               if (h12 > 9)
                  h12 = 0;
               if (h11 == 2)
                  if (h12 > 4)
                     h12 = 0;

               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               h12--;
               if (h11 == 2)
                  if (h12 == -1)
                     h12 = 4;
               if (h12 == -1)
                  h12 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(12, 0);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               m11++;
               if (m11 > 5)
                  m11 = 0;

               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               m11--;
               if (m11 == -1)
                  m11 = 5;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 4:

            lcd_gotoxy(13, 0);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               m12++;
               if (m12 > 9)
                  m12 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               m12--;
               if (m12 == -1)
                  m12 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(50);
            }
            break;
         default:
            cot = 1;
            delay_ms(25);
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            hour1 = h11 * 10 + h12;
            min1 = m11 * 10 + m12;
            if (hour1 > 24)
            {
               lcd_clear();
               lcd_gotoxy(1, 1);
               lcd_puts("!vui long nhap lai gio!");
               delay_ms(500);
               lcd_clear();
            }
            else
            {
               rtc_set_time(hour1, min1, 0);
               cot = 1;
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("luu thanh cong!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;

            delay_ms(25);
         }
      };
      // hang 2
      if (hang == 1)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(9, 1);
            delay_ms(200);
            lcd_putchar('_');
            // phim up

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               d1++;
               if (d1 > 3)
                  d1 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               d1--;
               if (d1 == -1)
                  d1 = 3;
               led_do = ~led_do;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(10, 1);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               d2++;
               if (d2 > 9)
                  d2 = 0;
               if (d1 == 3)
                  if (d2 > 1)
                     d2 = 0;

               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               d2--;
               if (d1 == 3)
                  if (d2 == -1)
                     d2 = 1;
               if (d2 == -1)
                  d2 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(12, 1);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               m1++;
               if (m1 > 1)
                  m1 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               m1--;
               if (m1 == -1)
                  m1 = 1;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 4:

            lcd_gotoxy(13, 1);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               m2++;
               if (m1 == 1)
               {
                  if (m2 > 2)
                     m2 = 0;
               }
               if (m2 > 9)
                  m2 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               m2--;
               if (m1 == 1)
                  if (m2 == -1)
                     m2 = 2;
               if (m2 == -1)
                  m2 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 5:

            lcd_gotoxy(15, 1);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               y1++;
               if (y1 > 9)
                  y1 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               y1--;
               if (y1 == -1)
                  y1 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 6:

            lcd_gotoxy(16, 1);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               y2++;
               if (y2 > 9)
                  y2 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               y2--;
               if (y2 == -1)
                  y2 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            day1 = d1 * 10 + d2;
            month1 = m1 * 10 + m2;
            year1 = y1 * 10 + y2;
            if ((day1 > 31) | (month1 > 12) | (day1 == 0) | (month1 == 0))
            {
               lcd_clear();
               lcd_gotoxy(1, 1);
               lcd_puts("nhap lai ngay thang!");
               delay_ms(500);
               lcd_clear();
            }
            else
            {
               rtc_set_date(2, day1, month1, year1);
               cot = 1;
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("luu thanh cong!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;
            delay_ms(25);
         }
      };
      // hang 3
      if (hang == 2)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(6, 2);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               doam11 = doam11 + 10;
               if (doam11 > 99)
                  doam11 = doam11 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               doam11 = doam11 - 10;
               if (doam11 < 0)
                  doam11 = doam11 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:
            lcd_gotoxy(7, 2);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               doam11++;
               if ((doam11 - 1) % 10 == 9)
                  doam11 = doam11 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               doam11--;
               if ((doam11 + 1) % 10 == 0)
                  doam11 = doam11 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(12, 2);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               doam22 = doam22 + 10;
               if (doam22 > 99)
                  doam22 = doam22 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               doam22 = doam22 - 10;
               if (doam22 < 0)
                  doam22 = doam22 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 4:
            lcd_gotoxy(13, 2);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               doam22++;
               if ((doam22 - 1) % 10 == 9)
                  doam22 = doam22 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               doam22--;
               if ((doam22 + 1) % 10 == 0)
                  doam22 = doam22 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;

         default:
            cot = 1;
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if (doam11 < doam22)
            {
               EEPROM_write(0, doam11);
               EEPROM_write(1, doam22);
               cot = 1;
               sprintf(enthernet, "%s,doam_low,%d %c%c%c", IP, doam11, 0, 13, 10);
               puts1(enthernet);
               delay_ms(100);
               sprintf(enthernet, "%s,doam_high,%d %c%c%c", IP, doam22, 0, 13, 10);
               puts1(enthernet);
               delay_ms(100);
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("luu thanh cong!");
               delay_ms(500);
               lcd_clear();
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 0;
               lcd_gotoxy(2, 2);
               lcd_clear();
               sprintf(lcd, "!!nhap lai nguong!!");
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
            }
         }
         else
         {
            bit_phim = 1;
            delay_ms(25);
         }
      };
      // hang 4
      if (hang == 3)
      {

         switch (cot)
         {
         case 1:
            lcd_gotoxy(6, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               nhietdo11 = nhietdo11 + 10;
               if (nhietdo11 > 99)
                  nhietdo11 = nhietdo11 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               nhietdo11 = nhietdo11 - 10;
               if (nhietdo11 < 0)
                  nhietdo11 = nhietdo11 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }

            break;
         case 2:

            lcd_gotoxy(7, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               nhietdo11++;
               if ((nhietdo11 - 1) % 10 == 9)
                  nhietdo11 = nhietdo11 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               nhietdo11--;
               if ((nhietdo11 + 1) % 10 == 0)
                  nhietdo11 = nhietdo11 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(12, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               nhietdo22 = nhietdo22 + 10;
               if (nhietdo22 > 99)
                  nhietdo22 = nhietdo22 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               nhietdo22 = nhietdo22 - 10;
               if (nhietdo22 < 0)
                  nhietdo22 = nhietdo22 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 4:
            lcd_gotoxy(13, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               nhietdo22++;
               if ((nhietdo22 - 1) % 10 == 9)
                  nhietdo22 = nhietdo22 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               nhietdo22--;
               if ((nhietdo22 + 1) % 10 == 0)
                  nhietdo22 = nhietdo22 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if (nhietdo11 < nhietdo22)
            {
               EEPROM_write(2, nhietdo11);
               EEPROM_write(3, nhietdo22);
               cot = 1;
               sprintf(enthernet, "%s,nhietdo_lower,%d %c%c%c", IP, nhietdo11, 0, 13, 10);
               puts1(enthernet);
               delay_ms(100);
               sprintf(enthernet, "%s,nhietdo_high,%d %c%c%c", IP, nhietdo22, 0, 13, 10);
               puts1(enthernet);
               delay_ms(100);
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("luu thanh cong!");
               delay_ms(500);
               lcd_clear();
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 0;
               lcd_gotoxy(2, 2);
               lcd_clear();
               sprintf(lcd, "!!nhap lai nguong!!");
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
            }
         }
         else
         {
            bit_phim = 1;
            delay_ms(25);
         }
      };
   }
   if (strcmp(hienthi, "setDC") == 0)
   {
      sdt_call1 = EEPROM_read(11);
      sdt_call2 = EEPROM_read(12);
      sdt_call3 = EEPROM_read(13);
      sdt_call4 = EEPROM_read(14);
      sdt_call5 = EEPROM_read(15);
      sdt_call6 = EEPROM_read(16);
      sdt_call7 = EEPROM_read(17);
      sdt_call8 = EEPROM_read(18);
      sdt_call9 = EEPROM_read(19);
      sdt_sms1 = EEPROM_read(21);
      sdt_sms2 = EEPROM_read(22);
      sdt_sms3 = EEPROM_read(23);
      sdt_sms4 = EEPROM_read(24);
      sdt_sms5 = EEPROM_read(25);
      sdt_sms6 = EEPROM_read(26);
      sdt_sms7 = EEPROM_read(27);
      sdt_sms8 = EEPROM_read(28);
      sdt_sms9 = EEPROM_read(29);
      // if ((sdt_sms1 > 9) | (sdt_sms2 > 9) | (sdt_sms3 > 9) | (sdt_sms4 > 9) | (sdt_sms5 > 9) | (sdt_sms6 > 9) | (sdt_sms7 > 9) | (sdt_sms8 > 9) | (sdt_sms9 > 9))
      // {
      //    sdt_sms1 = sdt_sms2 = sdt_sms3 = sdt_sms4 = sdt_sms5 = sdt_sms6 = sdt_sms7 = sdt_sms8 = sdt_sms9 = 0;
      //    EEPROM_write(21, sdt_sms1);
      //    EEPROM_write(22, sdt_sms2);
      //    EEPROM_write(23, sdt_sms3);
      //    EEPROM_write(24, sdt_sms4);
      //    EEPROM_write(25, sdt_sms5);
      //    EEPROM_write(26, sdt_sms6);
      //    EEPROM_write(27, sdt_sms7);
      //    EEPROM_write(28, sdt_sms8);
      //    EEPROM_write(29, sdt_sms9);
      // }
      // if ((sdt_call1 > 9) | (sdt_call2 > 9) | (sdt_call3 > 9) | (sdt_call4 > 9) | (sdt_call5 > 9) | (sdt_call6 > 9) | (sdt_call7 > 9) | (sdt_call8 > 9) | (sdt_call9 > 9))
      // {
      //    sdt_call1 = sdt_call2 = sdt_call3 = sdt_call4 = sdt_call5 = sdt_call6 = sdt_call7 = sdt_call8 = sdt_call9 = 0;
      //    EEPROM_write(11, sdt_call1);
      //    EEPROM_write(12, sdt_call2);
      //    EEPROM_write(13, sdt_call3);
      //    EEPROM_write(14, sdt_call4);
      //    EEPROM_write(15, sdt_call5);
      //    EEPROM_write(16, sdt_call6);
      //    EEPROM_write(17, sdt_call7);
      //    EEPROM_write(18, sdt_call8);
      //    EEPROM_write(19, sdt_call9);
      // }
      ////////////////////////////////
      for (i = 1; i < 4; i++)
         if (i == hang - 3)
         {
            lcd_gotoxy(19, i);
            lcd_putchar('<');
         }
         else
         {
            lcd_gotoxy(19, i);
            lcd_putchar(' ');
         }
      lcd_gotoxy(3, 0);
      sprintf(lcd, "Cai dat muc DC");
      lcd_puts(lcd);
      lcd_gotoxy(0, 1);
      sprintf(lcd, "DC_low_1: %1d%1d,%1d", dc_low_1 / 100, (dc_low_1 % 100) / 10, dc_low_1 % 10);
      lcd_puts(lcd);
      lcd_gotoxy(0, 2);
      sprintf(lcd, "DC_low_2: %1d%1d,%1d", dc_low_2 / 100, (dc_low_2 % 100) / 10, dc_low_2 % 10);
      lcd_puts(lcd);
      lcd_gotoxy(0, 3);
      sprintf(lcd, "DC_high : %1d%1d,%1d", dc_high / 100, (dc_high % 100) / 10, dc_high % 10);
      lcd_puts(lcd);
      //
      // hang1
      if (hang - 4 == 0)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(10, 1);
            delay_ms(200);
            lcd_putchar('_');
            // phim up
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_1 = dc_low_1 + 100;
               if (dc_low_1 >= 1000)
                  dc_low_1 = dc_low_1 - 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_1 = dc_low_1 - 100;
               if (dc_low_1 < 0)
                  dc_low_1 = dc_low_1 + 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(11, 1);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_1 = dc_low_1 + 10;
               if (((dc_low_1 - 10) / 10) % 10 == 9)
                  dc_low_1 = dc_low_1 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_1 = dc_low_1 - 10;
               if (((dc_low_1 + 10) / 10) % 10 == 0)
                  dc_low_1 = dc_low_1 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(13, 1);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_1++;
               if (((dc_low_1 - 1) % 10) == 9)
                  dc_low_1 = dc_low_1 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_1--;
               if (((dc_low_1 + 1) % 10) == 0)
                  dc_low_1 = dc_low_1 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            delay_ms(25);
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if ((dc_low_1 != 0) & (dc_low_1 < EEPROM_read(33) * 10 + EEPROM_read(34)))
            {
               lcd_clear();
               EEPROM_write(31, dc_low_1 / 10);
               EEPROM_write(32, dc_low_1 % 10);
               cot = 1;
               lcd_gotoxy(3, 1);
               sprintf(enthernet, "%s,dienap_low,%d.%d", IP, dc_low_1 / 10, dc_low_1 % 10);
               puts1(enthernet);
               sprintf(lcd, "Da luu nguong");
               lcd_puts(lcd);
               lcd_gotoxy(3, 2);
               sprintf(lcd, "DC_low_1:%d,%dV", dc_low_1 / 10, dc_low_1 % 10);
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
            }
            else
            {
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("Kiem tra lai muc DC_low_1!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;

            delay_ms(25);
         }
      };
      // hang 2
      if (hang - 4 == 1)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(10, 2);
            delay_ms(200);
            lcd_putchar('_');
            // phim up

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_2 = dc_low_2 + 100;
               if (dc_low_2 >= 1000)
                  dc_low_2 = dc_low_2 - 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_2 = dc_low_2 - 100;
               if (dc_low_2 < 0)
                  dc_low_2 = dc_low_2 + 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(11, 2);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_2 = dc_low_2 + 10;
               if (((dc_low_2 - 10) / 10) % 10 == 9)
                  dc_low_2 = dc_low_2 - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_2 = dc_low_2 - 10;
               if (((dc_low_2 + 10) / 10) % 10 == 0)
                  dc_low_2 = dc_low_2 + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(13, 2);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_low_2++;
               if (((dc_low_2 - 1) % 10) == 9)
                  dc_low_2 = dc_low_2 - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_low_2--;
               if (((dc_low_2 + 1) % 10) == 0)
                  dc_low_2 = dc_low_2 + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            delay_ms(25);
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if ((dc_low_2 != 0) & (dc_low_2 > EEPROM_read(31) * 10 + EEPROM_read(32)) & (dc_low_2 < EEPROM_read(35) * 10 + EEPROM_read(36)))
            {
               lcd_clear();
               EEPROM_write(33, dc_low_2 / 10);
               EEPROM_write(34, dc_low_2 % 10);
               cot = 1;
               lcd_gotoxy(3, 1);
               sprintf(enthernet, "%s,dienap_low1,%d.%d", IP, dc_low_2 / 10, dc_low_2 % 10);
               puts1(enthernet);
               sprintf(lcd, "Da luu nguong");
               lcd_puts(lcd);
               lcd_gotoxy(3, 2);
               sprintf(lcd, "Dc_low_2:%d,%dV", dc_low_2 / 10, dc_low_2 % 10);
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
            }
            else
            {
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("Kiem tra lai muc Dc_low_2!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;

            delay_ms(25);
         }
      };
      if (hang - 4 == 2)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(10, 3);
            delay_ms(200);
            lcd_putchar('_');
            // phim up

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_high = dc_high + 100;
               if (dc_high >= 1000)
                  dc_high = dc_high - 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_high = dc_high - 100;
               if (dc_high < 0)
                  dc_high = dc_high + 1000;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(11, 3);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_high = dc_high + 10;
               if (((dc_high - 10) / 10) % 10 == 9)
                  dc_high = dc_high - 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_high = dc_high - 10;
               if (((dc_high + 10) / 10) % 10 == 0)
                  dc_high = dc_high + 100;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(13, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               dc_high++;
               if (((dc_high - 1) % 10) == 9)
                  dc_high = dc_high - 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               dc_high--;
               if (((dc_high + 1) % 10) == 0)
                  dc_high = dc_high + 10;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            delay_ms(25);
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if ((dc_high != 0) & (dc_high > EEPROM_read(33) * 10 + EEPROM_read(34)))
            {
               lcd_clear();
               EEPROM_write(35, dc_high / 10);
               EEPROM_write(36, dc_high % 10);
               cot = 1;
               lcd_gotoxy(3, 1);
               sprintf(enthernet, "%s,dienap_high,%d.%d", IP, dc_high / 10, dc_high % 10);
               puts1(enthernet);
               sprintf(lcd, "Da luu nguong");
               lcd_puts(lcd);
               lcd_gotoxy(3, 2);
               sprintf(lcd, "Dc_high:%d,%dV", dc_high / 10, dc_high % 10);
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
            }
            else
            {
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("Kiem tra lai muc Dc_high!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;

            delay_ms(25);
         }
      };
   }
   if (strcmp(hienthi, "sdt") == 0)
   {
      for (i = 2; i < 4; i++)
         if (i == hang - 5)
         {
            lcd_gotoxy(19, i);
            lcd_putchar('<');
         }
         else
         {
            lcd_gotoxy(19, i);
            lcd_putchar(' ');
         }
      lcd_gotoxy(0, 0);
      sprintf(lcd, "Cai dat SDT canh bao");
      lcd_puts(lcd);
      lcd_gotoxy(2, 1);
      sprintf(lcd, "%s", tram);
      lcd_puts(lcd);
      // lcd_gotoxy(0, 2);
      // sprintf(lcd, "Call: 0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9);
      // // sprintf(lcd, "Sms : 0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9);
      // lcd_puts(lcd);
      lcd_gotoxy(0, 3);
      sprintf(lcd, "Sms : 0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_sms1, sdt_sms2, sdt_sms3, sdt_sms4, sdt_sms5, sdt_sms6, sdt_sms7, sdt_sms8, sdt_sms9);
      lcd_puts(lcd);
      // hang 1
      // if (hang - 7 == 0)
      // {

      //    switch (cot)
      //    {
      //    case 1:

      //       lcd_gotoxy(7, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       // phim up

      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call1++;
      //          if (sdt_call1 > 9)
      //             sdt_call1 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call1--;
      //          if (sdt_call1 == -1)
      //             sdt_call1 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 2:

      //       lcd_gotoxy(8, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');

      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call2++;
      //          if (sdt_call2 > 9)
      //             sdt_call2 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call2--;
      //          if (sdt_call2 == -1)
      //             sdt_call2 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 3:

      //       lcd_gotoxy(9, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call3++;
      //          if (sdt_call3 > 9)
      //             sdt_call3 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call3--;
      //          if (sdt_call3 == -1)
      //             sdt_call3 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 4:

      //       lcd_gotoxy(10, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call4++;
      //          if (sdt_call4 > 9)
      //             sdt_call4 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call4--;
      //          if (sdt_call4 == -1)
      //             sdt_call4 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 5:

      //       lcd_gotoxy(11, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call5++;
      //          if (sdt_call5 > 9)
      //             sdt_call5 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call5--;
      //          if (sdt_call5 == -1)
      //             sdt_call5 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 6:

      //       lcd_gotoxy(12, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call6++;
      //          if (sdt_call6 > 9)
      //             sdt_call6 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call6--;
      //          if (sdt_call6 == -1)
      //             sdt_call6 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 7:

      //       lcd_gotoxy(13, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call7++;
      //          if (sdt_call7 > 9)
      //             sdt_call7 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call7--;
      //          if (sdt_call7 == -1)
      //             sdt_call7 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 8:

      //       lcd_gotoxy(14, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call8++;
      //          if (sdt_call8 > 9)
      //             sdt_call8 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call8--;
      //          if (sdt_call8 == -1)
      //             sdt_call8 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    case 9:

      //       lcd_gotoxy(15, 2);
      //       delay_ms(200);
      //       lcd_putchar('_');
      //       if ((Up_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call9++;
      //          if (sdt_call9 > 9)
      //             sdt_call9 = 0;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       if ((Down_pin == 0) && (bit_phim == 1))
      //       {
      //          sdt_call9--;
      //          if (sdt_call9 == -1)
      //             sdt_call9 = 9;
      //          bit_phim = 0;
      //          delay_ms(50);
      //       }
      //       else
      //       {
      //          bit_phim = 1;
      //          delay_ms(25);
      //       }
      //       break;
      //    default:
      //       cot = 1;
      //       delay_ms(25);
      //       break;
      //    }
      //    // luu
      //    if ((Ent_pin == 0) && (bit_phim == 1))
      //    {
      //       bit_phim = 0;
      //       delay_ms(50);
      //       if (sdt_call1 != 0)
      //       {
      //          lcd_clear();
      //          EEPROM_write(11, sdt_call1);
      //          EEPROM_write(12, sdt_call2);
      //          EEPROM_write(13, sdt_call3);
      //          EEPROM_write(14, sdt_call4);
      //          EEPROM_write(15, sdt_call5);
      //          EEPROM_write(16, sdt_call6);
      //          EEPROM_write(17, sdt_call7);
      //          EEPROM_write(18, sdt_call8);
      //          EEPROM_write(19, sdt_call9);
      //          cot = 1;
      //          lcd_gotoxy(3, 1);
      //          sprintf(sdt_call, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9);
      //          sprintf(lcd, "da luu sdt sms:");
      //          lcd_puts(lcd);
      //          lcd_gotoxy(5, 2);
      //          sprintf(lcd, "%s", sdt_call);
      //          lcd_puts(lcd);
      //          delay_ms(500);
      //          lcd_clear();
      //       }
      //       else
      //       {
      //          lcd_clear();
      //          lcd_gotoxy(2, 1);
      //          lcd_puts("Kiem tra lai SDT!");
      //          delay_ms(500);
      //          lcd_clear();
      //       }
      //    }
      //    else
      //    {
      //       bit_phim = 1;

      //       delay_ms(25);
      //    }
      // };
      // // hang 2
      if (hang - 7 == 0)
      {

         switch (cot)
         {
         case 1:

            lcd_gotoxy(7, 3);
            delay_ms(200);
            lcd_putchar('_');
            // phim up

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms1++;
               if (sdt_sms1 > 9)
                  sdt_sms1 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms1--;
               if (sdt_sms1 == -1)
                  sdt_sms1 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 2:

            lcd_gotoxy(8, 3);
            delay_ms(200);
            lcd_putchar('_');

            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms2++;
               if (sdt_sms2 > 9)
                  sdt_sms2 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms2--;
               if (sdt_sms2 == -1)
                  sdt_sms2 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 3:

            lcd_gotoxy(9, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms3++;
               if (sdt_sms3 > 9)
                  sdt_sms3 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms3--;
               if (sdt_sms3 == -1)
                  sdt_sms3 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 4:

            lcd_gotoxy(10, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms4++;
               if (sdt_sms4 > 9)
                  sdt_sms4 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms4--;
               if (sdt_sms4 == -1)
                  sdt_sms4 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 5:

            lcd_gotoxy(11, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms5++;
               if (sdt_sms5 > 9)
                  sdt_sms5 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms5--;
               if (sdt_sms5 == -1)
                  sdt_sms5 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 6:

            lcd_gotoxy(12, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms6++;
               if (sdt_sms6 > 9)
                  sdt_sms6 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms6--;
               if (sdt_sms6 == -1)
                  sdt_sms6 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 7:

            lcd_gotoxy(13, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms7++;
               if (sdt_sms7 > 9)
                  sdt_sms7 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms7--;
               if (sdt_sms7 == -1)
                  sdt_sms7 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 8:

            lcd_gotoxy(14, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms8++;
               if (sdt_sms8 > 9)
                  sdt_sms8 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms8--;
               if (sdt_sms8 == -1)
                  sdt_sms8 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         case 9:

            lcd_gotoxy(15, 3);
            delay_ms(200);
            lcd_putchar('_');
            if ((Up_pin == 0) && (bit_phim == 1))
            {
               sdt_sms9++;
               if (sdt_sms9 > 9)
                  sdt_sms9 = 0;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            if ((Down_pin == 0) && (bit_phim == 1))
            {
               sdt_sms9--;
               if (sdt_sms9 == -1)
                  sdt_sms9 = 9;
               bit_phim = 0;
               delay_ms(50);
            }
            else
            {
               bit_phim = 1;
               delay_ms(25);
            }
            break;
         default:
            cot = 1;
            delay_ms(25);
            break;
         }
         // luu
         if ((Ent_pin == 0) && (bit_phim == 1))
         {
            if (sdt_sms1 != 0)
            {
               lcd_clear();
               cot = 1;
               lcd_gotoxy(3, 1);
               sprintf(sdt_sms, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_sms1, sdt_sms2, sdt_sms3, sdt_sms4, sdt_sms5, sdt_sms6, sdt_sms7, sdt_sms8, sdt_sms9);
               sprintf(lcd, "da luu sdt sms:");
               lcd_puts(lcd);
               lcd_gotoxy(5, 2);
               sprintf(lcd, "%s", sdt_sms);
               lcd_puts(lcd);
               delay_ms(500);
               lcd_clear();
               EEPROM_write(21, sdt_sms1);
               EEPROM_write(22, sdt_sms2);
               EEPROM_write(23, sdt_sms3);
               EEPROM_write(24, sdt_sms4);
               EEPROM_write(25, sdt_sms5);
               EEPROM_write(26, sdt_sms6);
               EEPROM_write(27, sdt_sms7);
               EEPROM_write(28, sdt_sms8);
               EEPROM_write(29, sdt_sms9);
            }
            else
            {
               lcd_clear();
               lcd_gotoxy(2, 1);
               lcd_puts("kiem tra lai SDT!");
               delay_ms(500);
               lcd_clear();
            }
            bit_phim = 0;
            delay_ms(50);
         }
         else
         {
            bit_phim = 1;
            delay_ms(25);
         }
      };
   }

   ////////////////////////////////////////////////////////////////
   if ((Mode_pin == 0) && (bit_phim == 1))
   {
      hang++;

      if (hang == 0)
      {
         lcd_clear();
         cot = 1;
         strcpy(hienthi, "Mode");
      }
      if (hang == 4)
      {
         lcd_clear();
         cot = 1;
         strcpy(hienthi, "setDC");
      }
      if (hang == 7)
      {
         lcd_clear();
         cot = 1;
         strcpy(hienthi, "sdt");
      }
      if (hang == 8)
      {
         hang = -1;
         lcd_clear();
         strcpy(hienthi, "chung");
         delay_ms(200);
         cot = 0;
      }
      bit_phim = 0;
      delay_ms(50);
   }
   else
   {
      bit_phim = 1;
      delay_ms(25);
   }
   // phim shift
   if ((Shift_pin == 0) && (bit_phim == 1) && ((strcmp(hienthi, "sdt") == 0) | (strcmp(hienthi, "Mode") == 0) | (strcmp(hienthi, "setDC") == 0)))
   {
      cot++;
      bit_phim = 0;
      delay_ms(50);
   }
   else
   {
      bit_phim = 1;
      delay_ms(25);
   }
}

// Timer 0 overflow interrupt service routine
interrupt[TIM0_OVF] void timer0_ovf_isr(void)
{
   // Reinitialize Timer 0 value
   TCNT0 = 0xF0;
   // Place your code here
}

// Timer 0 output compare interrupt service routine
interrupt[TIM0_COMP] void timer0_comp_isr(void)
{
   // Place your code here
   dht++;
}

// Timer1 overflow interrupt service routine
interrupt[TIM1_OVF] void timer1_ovf_isr(void)
{
   // Place your code here
   DHT22_getdata(&dht_nhiet_do, &dht_do_am);
}
void main(void)
{
   // Declare your local variables here

   // Input/Output Ports initialization
   // Port A initialization
   // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRA = (0 << DDA7) | (0 << DDA6) | (0 << DDA5) | (0 << DDA4) | (0 << DDA3) | (0 << DDA2) | (0 << DDA1) | (0 << DDA0);
   // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
   PORTA = (0 << PORTA7) | (0 << PORTA6) | (0 << PORTA5) | (0 << PORTA4) | (0 << PORTA3) | (0 << PORTA2) | (0 << PORTA1) | (0 << PORTA0);

   // Port B initialization
   // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRB = (0 << DDB7) | (0 << DDB6) | (0 << DDB5) | (0 << DDB4) | (0 << DDB3) | (0 << DDB2) | (0 << DDB1) | (0 << DDB0);
   // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=P Bit2=P Bit1=T Bit0=T
   PORTB = (0 << PORTB7) | (0 << PORTB6) | (0 << PORTB5) | (1 << PORTB4) | (1 << PORTB3) | (1 << PORTB2) | (0 << PORTB1) | (0 << PORTB0);

   // Port C initialization
   // Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=In Bit2=In Bit1=In Bit0=In
   DDRC = (0 << DDC7) | (0 << DDC6) | (0 << DDC5) | (1 << DDC4) | (0 << DDC3) | (0 << DDC2) | (0 << DDC1) | (0 << DDC0);
   // State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=T Bit2=T Bit1=T Bit0=T
   PORTC = (0 << PORTC7) | (0 << PORTC6) | (0 << PORTC5) | (0 << PORTC4) | (0 << PORTC3) | (0 << PORTC2) | (0 << PORTC1) | (0 << PORTC0);

   // Port D initialization
   // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRD = (1 << DDD7) | (1 << DDD6) | (1 << DDD5) | (0 << DDD4) | (0 << DDD3) | (0 << DDD2) | (0 << DDD1) | (0 << DDD0);
   // State: Bit7=0 Bit6=0 Bit5=0 Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
   PORTD = (0 << PORTD7) | (0 << PORTD6) | (0 << PORTD5) | (0 << PORTD4) | (0 << PORTD3) | (0 << PORTD2) | (0 << PORTD1) | (0 << PORTD0);

   // Port E initialization
   // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRE = (0 << DDE7) | (0 << DDE6) | (0 << DDE5) | (0 << DDE4) | (0 << DDE3) | (0 << DDE2) | (0 << DDE1) | (0 << DDE0);
   // State: Bit7=P Bit6=P Bit5=P Bit4=P Bit3=P Bit2=T Bit1=T Bit0=T
   PORTE = (1 << PORTE7) | (1 << PORTE6) | (1 << PORTE5) | (1 << PORTE4) | (1 << PORTE3) | (0 << PORTE2) | (0 << PORTE1) | (0 << PORTE0);

   // Port F initialization
   // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRF = (0 << DDF7) | (0 << DDF6) | (0 << DDF5) | (0 << DDF4) | (0 << DDF3) | (0 << DDF2) | (0 << DDF1) | (0 << DDF0);
   // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
   PORTF = (0 << PORTF7) | (0 << PORTF6) | (0 << PORTF5) | (0 << PORTF4) | (1 << PORTF3) | (0 << PORTF2) | (0 << PORTF1) | (0 << PORTF0);

   // Port G initialization
   // Function: Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
   DDRG = (0 << DDG4) | (0 << DDG3) | (0 << DDG2) | (0 << DDG1) | (0 << DDG0);
   // State: Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
   PORTG = (0 << PORTG4) | (0 << PORTG3) | (0 << PORTG2) | (0 << PORTG1) | (0 << PORTG0);

   // Timer/Counter 0 initialization
   // Clock source: System Clock
   // Clock value: 8000,000 kHz
   // Mode: Normal top=0xFF
   // OC0 output: Disconnected
   // Timer Period: 2 us
   ASSR = 0 << AS0;
   TCCR0 = (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (0 << CS02) | (0 << CS01) | (1 << CS00);
   TCNT0 = 0xF0;
   OCR0 = 0x00;

   // Timer/Counter 1 initialization
   // Clock source: System Clock
   // Clock value: 31,250 kHz
   // Mode: Normal top=0xFFFF
   // OC1A output: Disconnected
   // OC1B output: Disconnected
   // OC1C output: Disconnected
   // Noise Canceler: Off
   // Input Capture on Rising Edge
   // Timer Period: 2,0972 s
   // Timer1 Overflow Interrupt: On
   // Input Capture Interrupt: Off
   // Compare A Match Interrupt: Off
   // Compare B Match Interrupt: Off
   // Compare C Match Interrupt: Off
   TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << COM1C1) | (0 << COM1C0) | (0 << WGM11) | (0 << WGM10);
   TCCR1B = (0 << ICNC1) | (1 << ICES1) | (0 << WGM13) | (0 << WGM12) | (1 << CS12) | (0 << CS11) | (0 << CS10);
   TCNT1H = 0xFF;
   TCNT1L = 0xFE;
   ICR1H = 0x00;
   ICR1L = 0x00;
   OCR1AH = 0x00;
   OCR1AL = 0x00;
   OCR1BH = 0x00;
   OCR1BL = 0x00;
   OCR1CH = 0x00;
   OCR1CL = 0x00;

   // Timer/Counter 2 initialization
   // Clock source: System Clock
   // Clock value: Timer2 Stopped
   // Mode: Normal top=0xFF
   // OC2 output: Disconnected
   TCCR2 = (0 << WGM20) | (0 << COM21) | (0 << COM20) | (0 << WGM21) | (0 << CS22) | (0 << CS21) | (0 << CS20);
   TCNT2 = 0x00;
   OCR2 = 0x00;

   // Timer/Counter 3 initialization
   // Clock source: System Clock
   // Clock value: Timer3 Stopped
   // Mode: Normal top=0xFFFF
   // OC3A output: Disconnected
   // OC3B output: Disconnected
   // OC3C output: Disconnected
   // Noise Canceler: Off
   // Input Capture on Falling Edge
   // Timer3 Overflow Interrupt: Off
   // Input Capture Interrupt: Off
   // Compare A Match Interrupt: Off
   // Compare B Match Interrupt: Off
   // Compare C Match Interrupt: Off
   TCCR3A = (0 << COM3A1) | (0 << COM3A0) | (0 << COM3B1) | (0 << COM3B0) | (0 << COM3C1) | (0 << COM3C0) | (0 << WGM31) | (0 << WGM30);
   TCCR3B = (0 << ICNC3) | (0 << ICES3) | (0 << WGM33) | (0 << WGM32) | (0 << CS32) | (0 << CS31) | (0 << CS30);
   TCNT3H = 0x00;
   TCNT3L = 0x00;
   ICR3H = 0x00;
   ICR3L = 0x00;
   OCR3AH = 0x00;
   OCR3AL = 0x00;
   OCR3BH = 0x00;
   OCR3BL = 0x00;
   OCR3CH = 0x00;
   OCR3CL = 0x00;

   // Timer(s)/Counter(s) Interrupt(s) initialization
   TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (1 << TOIE1) | (0 << OCIE0) | (0 << TOIE0);
   ETIMSK = (0 << TICIE3) | (0 << OCIE3A) | (0 << OCIE3B) | (0 << TOIE3) | (0 << OCIE3C) | (0 << OCIE1C);

   // External Interrupt(s) initialization
   // INT0: Off
   // INT1: Off
   // INT2: Off
   // INT3: Off
   // INT4: Off
   // INT5: Off
   // INT6: Off
   // INT7: On
   // INT7 Mode: Falling Edge
   EICRA = (0 << ISC31) | (0 << ISC30) | (0 << ISC21) | (0 << ISC20) | (0 << ISC11) | (0 << ISC10) | (0 << ISC01) | (0 << ISC00);
   EICRB = (1 << ISC71) | (0 << ISC70) | (0 << ISC61) | (0 << ISC60) | (0 << ISC51) | (0 << ISC50) | (0 << ISC41) | (0 << ISC40);
   EIMSK = (1 << INT7) | (0 << INT6) | (0 << INT5) | (0 << INT4) | (0 << INT3) | (0 << INT2) | (0 << INT1) | (0 << INT0);
   EIFR = (1 << INTF7) | (0 << INTF6) | (0 << INTF5) | (0 << INTF4) | (0 << INTF3) | (0 << INTF2) | (0 << INTF1) | (0 << INTF0);

   // USART0 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART0 Receiver: On
   // USART0 Transmitter: On
   // USART0 Mode: Asynchronous
   // USART0 Baud Rate: 9600
   UCSR0A = (0 << RXC0) | (0 << TXC0) | (0 << UDRE0) | (0 << FE0) | (0 << DOR0) | (0 << UPE0) | (0 << U2X0) | (0 << MPCM0);
   UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02) | (0 << RXB80) | (0 << TXB80);
   UCSR0C = (0 << UMSEL0) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);
   UBRR0H = 0x00;
   UBRR0L = 0x33;

   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART1 Baud Rate: 9600
   UCSR1A = (0 << RXC1) | (0 << TXC1) | (0 << UDRE1) | (0 << FE1) | (0 << DOR1) | (0 << UPE1) | (0 << U2X1) | (0 << MPCM1);
   UCSR1B = (0 << RXCIE1) | (0 << TXCIE1) | (0 << UDRIE1) | (1 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12) | (0 << RXB81) | (0 << TXB81);
   UCSR1C = (0 << UMSEL1) | (0 << UPM11) | (0 << UPM10) | (0 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10) | (0 << UCPOL1);
   UBRR1H = 0x00;
   UBRR1L = 0x33;

   // Analog Comparator initialization
   // Analog Comparator: Off
   // The Analog Comparator's positive input is
   // connected to the AIN0 pin
   // The Analog Comparator's negative input is
   // connected to the AIN1 pin
   ACSR = (1 << ACD) | (0 << ACBG) | (0 << ACO) | (0 << ACI) | (0 << ACIE) | (0 << ACIC) | (0 << ACIS1) | (0 << ACIS0);

   // ADC initialization
   // ADC Clock frequency: 1000,000 kHz
   // ADC Voltage Reference: AREF pin
   ADMUX = ADC_VREF_TYPE;
   ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADFR) | (0 << ADIF) | (0 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   SFIOR = (0 << ACME);

   // SPI initialization
   // SPI disabled
   SPCR = (0 << SPIE) | (0 << SPE) | (0 << DORD) | (0 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (0 << SPR0);

   // TWI initialization
   // TWI disabled
   TWCR = (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWEN) | (0 << TWIE);

   // Bit-Banged I2C Bus initialization
   // I2C Port: PORTD
   // I2C SDA bit: 1
   // I2C SCL bit: 0
   // Bit Rate: 100 kHz
   // Note: I2C settings are specified in the
   // Project|Configure|C Compiler|Libraries|I2C menu.
   i2c_init();

   // DS1307 Real Time Clock initialization
   // Square wave output on pin SQW/OUT: Off
   // SQW/OUT pin state: 0
   rtc_init(0, 0, 0);

   // Alphanumeric LCD initialization
   // Connections are specified in the
   // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
   // RS - PORTA Bit 0
   // RD - PORTA Bit 1
   // EN - PORTA Bit 2
   // D4 - PORTA Bit 4
   // D5 - PORTA Bit 5
   // D6 - PORTA Bit 6
   // D7 - PORTA Bit 7
   // Characters/line: 20
   lcd_init(20);

// Global enable interrupts
#asm("sei")
   coi = 0;
   delay_ms(10000);
   khoitaosim();
   lcd_clear();
   // "0334271185"
   sdt_sms1 = EEPROM_read(21);
   sdt_sms2 = EEPROM_read(22);
   sdt_sms3 = EEPROM_read(23);
   sdt_sms4 = EEPROM_read(24);
   sdt_sms5 = EEPROM_read(25);
   sdt_sms6 = EEPROM_read(26);
   sdt_sms7 = EEPROM_read(27);
   sdt_sms8 = EEPROM_read(28);
   sdt_sms9 = EEPROM_read(29);
   if (!((sdt_sms1 >= 0) & (sdt_sms2 >= 0) & (sdt_sms3 >= 0) & (sdt_sms4 >= 0) & (sdt_sms5 >= 0) & (sdt_sms6 >= 0) & (sdt_sms7 >= 0) & (sdt_sms8 >= 0) & (sdt_sms9 >= 0) & (sdt_sms1 <= 9) & (sdt_sms2 <= 9) & (sdt_sms3 <= 9) & (sdt_sms4 <= 9) & (sdt_sms5 <= 9) & (sdt_sms6 <= 9) & (sdt_sms7 <= 9) & (sdt_sms8 <= 9) & (sdt_sms9 <= 9)))
   {
      // sdt_sms1 = sdt_sms2 = sdt_sms3 = sdt_sms4 = sdt_sms5 = sdt_sms6 = sdt_sms7 = sdt_sms8 = sdt_sms9 = 0;
      sdt_sms1 = 0; // 5
      sdt_sms2 = 3; // 5
      sdt_sms3 = 4; // 9
      sdt_sms4 = 2; // 6
      sdt_sms5 = 7; // 9
      sdt_sms6 = 1; // 6
      sdt_sms7 = 1; // 9
      sdt_sms8 = 8; // 1
      sdt_sms9 = 5; // 5
      EEPROM_write(21, sdt_sms1);
      EEPROM_write(22, sdt_sms2);
      EEPROM_write(23, sdt_sms3);
      EEPROM_write(24, sdt_sms4);
      EEPROM_write(25, sdt_sms5);
      EEPROM_write(26, sdt_sms6);
      EEPROM_write(27, sdt_sms7);
      EEPROM_write(28, sdt_sms8);
      EEPROM_write(29, sdt_sms9);
      sprintf(sdt_sms, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_sms1, sdt_sms2, sdt_sms3, sdt_sms4, sdt_sms5, sdt_sms6, sdt_sms7, sdt_sms8, sdt_sms9);
   }
   else
      sprintf(sdt_sms, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_sms1, sdt_sms2, sdt_sms3, sdt_sms4, sdt_sms5, sdt_sms6, sdt_sms7, sdt_sms8, sdt_sms9);
   sdt_call1 = EEPROM_read(11);
   sdt_call2 = EEPROM_read(12);
   sdt_call3 = EEPROM_read(13);
   sdt_call4 = EEPROM_read(14);
   sdt_call5 = EEPROM_read(15);
   sdt_call6 = EEPROM_read(16);
   sdt_call7 = EEPROM_read(17);
   sdt_call8 = EEPROM_read(18);
   sdt_call9 = EEPROM_read(19);
   if (!((sdt_call1 >= 0) & (sdt_call2 >= 0) & (sdt_call3 >= 0) & (sdt_call4 >= 0) & (sdt_call5 >= 0) & (sdt_call6 >= 0) & (sdt_call7 >= 0) & (sdt_call8 >= 0) & (sdt_call9 >= 0) & (sdt_call1 <= 9) & (sdt_call2 <= 9) & (sdt_call3 <= 9) & (sdt_call4 <= 9) & (sdt_call5 <= 9) & (sdt_call6 <= 9) & (sdt_call7 <= 9) & (sdt_call8 <= 9) & (sdt_call9 <= 9)))
   {
      // sdt_call1 = sdt_call2 = sdt_call3 = sdt_call4 = sdt_call5 = sdt_call6 = sdt_call7 = sdt_call8 = sdt_call9 = 0;
      sdt_call1 = 0; // 5 0
      sdt_call2 = 3; // 5 3
      sdt_call3 = 4; // 9 4
      sdt_call4 = 2; // 6 2
      sdt_call5 = 7; // 9 7
      sdt_call6 = 1; // 6 1
      sdt_call7 = 1; // 9 1
      sdt_call8 = 8; // 1 8
      sdt_call9 = 5; // 5 5
      EEPROM_write(11, sdt_call1);
      EEPROM_write(12, sdt_call2);
      EEPROM_write(13, sdt_call3);
      EEPROM_write(14, sdt_call4);
      EEPROM_write(15, sdt_call5);
      EEPROM_write(16, sdt_call6);
      EEPROM_write(17, sdt_call7);
      EEPROM_write(18, sdt_call8);
      EEPROM_write(19, sdt_call9);
      sprintf(sdt_call, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9);
   }
   else
      sprintf(sdt_call, "0%1d%1d%1d%1d%1d%1d%1d%1d%1d", sdt_call1, sdt_call2, sdt_call3, sdt_call4, sdt_call5, sdt_call6, sdt_call7, sdt_call8, sdt_call9);
   dc_low_1 = EEPROM_read(31) * 10 + EEPROM_read(32);
   dc_low_2 = EEPROM_read(33) * 10 + EEPROM_read(34);
   dc_high = EEPROM_read(35) * 10 + EEPROM_read(36);
   if ((dc_low_1 > 999) | (dc_low_2 > 999) | (dc_high > 999))
   {
      dc_low_1 = 470;
      dc_low_2 = 480;
      dc_high = 585;
      EEPROM_write(31, dc_low_1 / 10);
      EEPROM_write(32, dc_low_1 % 10);
      EEPROM_write(33, dc_low_2 / 10);
      EEPROM_write(34, dc_low_2 % 10);
      EEPROM_write(35, dc_high / 10);
      EEPROM_write(36, dc_high % 10);
   }
   if (KTAC1 == 0)
      bit_ac1 == 0;
   else
      bit_ac1 == 1;
   if (KTAC2 == 0)
      bit_ac2 == 0;
   else
      bit_ac2 == 1;
   bit_temp = bit_humi = 0;
   if (AS == 0)
      bit_as = 0;
   else
      bit_as = 1;

   while (1)
   {
      ct_hienthichung();
      ////////////////////////////////////////////////////////////////////////////////////////////////
      DC1 = (read_adc(1) * 5 * 10 ^ 1 / 1023);
      dc11 = DC1 / 610;
      dc12 = DC1 % 610 / 100;
      // dc 2
      DC2 = (read_adc(0) * 5 * 10 ^ 1 / 1023);
      dc21 = DC2 / 615;
      dc22 = DC2 % 615 / 100;
      // // ////////////////////////////////////////////////////////////////////////////////////////////////
      doam1 = EEPROM_read(0);
      doam2 = EEPROM_read(1);
      // DHT22_getdata(&dht_nhiet_do, &dht_do_am);
      if ((dht_do_am >= doam1 * 10) & (dht_do_am <= doam2 * 10) & (bit_humi == 1))
      {
         bit_humi = 0;
         sprintf(enthernet, "%s,humidity,%d.%d", IP, dht_do_am / 10, dht_do_am % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "do am tram %s hien tai la %d,%d da ve nguong binh thuong  ", tram, dht_do_am / 10, dht_do_am % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "do am tram %s hien tai la %d,%d da ve nguong binh thuong  ", tram, dht_do_am / 10, dht_do_am % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         lcd_clear();
      }
      if ((dht_do_am < doam1 * 10) & (bit_humi == 0))
      {
         bit_humi = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,humidity,%d.%d", IP, dht_do_am / 10, dht_do_am % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "do am tram %s hien tai la %d,%d o duoi nguong  ", tram, dht_do_am / 10, dht_do_am % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "do am tram %s hien tai la %d,%d o duoi nguong  ", tram, dht_do_am / 10, dht_do_am % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((dht_do_am > doam2 * 10) & (bit_humi == 0))
      {
         bit_coi = 1;
         bit_humi = 1;
         sprintf(enthernet, "%s,humidity,%d.%d", IP, dht_do_am / 10, dht_do_am % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "do am tram %s hien tai la %d,%d o tren nguong  ", tram, dht_do_am / 10, dht_do_am % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "do am tram %s hien tai la %d,%d o tren nguong  ", tram, dht_do_am / 10, dht_do_am % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////
      nhietdo1 = EEPROM_read(2);
      nhietdo2 = EEPROM_read(3);
      // DHT22_getdata(&dht_nhiet_do, &dht_do_am);
      if ((dht_nhiet_do >= nhietdo1 * 10) & (dht_nhiet_do <= nhietdo2 * 10) & (bit_temp == 1))
      {
         bit_temp = 0;
         sprintf(enthernet, "%s,temperature,%d.%d", IP, dht_nhiet_do / 10, dht_nhiet_do % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "nhiet do tram %s hien tai la %d,%d da tro ve nguong binh thuong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "nhiet do tram %s hien tai la %d,%d da tro ve nguong binh thuong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((dht_nhiet_do < nhietdo1 * 10) & (bit_temp == 0))
      {
         bit_temp = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,temperature,%d.%d", IP, dht_nhiet_do / 10, dht_nhiet_do % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "nhiet do tram %s hien tai la %d,%d o duoi  nguong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "nhiet do tram %s hien tai la %d,%d o duoi  nguong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((dht_nhiet_do > nhietdo2 * 10) & (bit_temp == 0))
      {
         bit_coi = 1;
         bit_temp = 1;
         sprintf(enthernet, "%s,temperature,%d.%d", IP, dht_nhiet_do / 10, dht_nhiet_do % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "nhiet do tram %s hien tai la %d,%d o tren  nguong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "nhiet do tram %s hien tai la %d,%d o tren  nguong  ", tram, dht_nhiet_do / 10, dht_nhiet_do % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      ////////////////////////////////
      if ((AS == 0) & (bit_as == 0))
      {
         bit_as = 1;
         sprintf(enthernet, "%s,light,2", IP);
         puts1(enthernet);
         delay_ms(100);
         lcd_gotoxy(0, 0);
         lcd_clear();
         sprintf(lcd, "mat anh sang ");
         lcd_puts(lcd);
         delay_ms(1000);
         lcd_clear();
      }
      if ((AS == 1) & (bit_as == 1))
      {
         bit_as = 0;
         sprintf(enthernet, "%s,light,1", IP);
         puts1(enthernet);
         delay_ms(100);
         lcd_gotoxy(0, 0);
         lcd_clear();
         sprintf(lcd, "co anh sang ");
         lcd_puts(lcd);
         delay_ms(500);
         lcd_clear();
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////
      if ((KTAC1 == 0) & (bit_ac1 == 0))
      {
         bit_ac1 = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,voltage_ac,1", IP);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "canh bao mat dien ap AC1 tai tram %s", tram);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "canh bao mat dien ap AC1 tai tram %s", tram);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((KTAC1 == 1) & (bit_ac1 == 1))
      {
         bit_ac1 = 0;
         sprintf(enthernet, "%s,voltage_ac,220", IP);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "da co dien ap AC1 tai tram %s", tram);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "da co dien ap AC1 tai tram %s", tram);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      ////////////////////////////////////////////////////////////////
      if ((KTAC2 == 0) & (bit_ac2 == 0))
      {
         bit_ac2 = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,voltage_ac2,1", IP);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Canh bao mat dien ap AC2 tai tram %s", tram);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Canh bao mat dien ap AC2 tai tram %s", tram);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((KTAC2 == 1) & (bit_ac2 == 1))
      {
         bit_ac2 = 0;
         sprintf(enthernet, "%s,voltage_ac2,220", IP);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Da co dien ap AC2 tai tram %s", tram);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Da co dien ap AC2 tai tram %s", tram);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
      }
      if ((min % 5 == 0) & (sec == 0) & (bit_enthernet == 1))
      {
         bit_enthernet = 0;
         lcd_clear();
         lcd_puts("truyen enthernet");
         sprintf(enthernet, "%s,temperature,%d.%d", IP, dht_nhiet_do / 10, dht_nhiet_do % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(enthernet, "%s,humidity,%d.%d", IP, dht_do_am / 10, dht_do_am % 10);
         puts1(enthernet);
         delay_ms(100);
         sprintf(enthernet, "%s,voltage_dc1,%d.%d", IP, dc11, dc12);
         puts1(enthernet);
         delay_ms(100);
         sprintf(enthernet, "%s,voltage_dc2,%d.%d", IP, dc21, dc22);
         puts1(enthernet);
         delay_ms(100);
         if (KTAC1 == 1)
         {
            sprintf(enthernet, "%s,voltage_ac,220", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         else
         {
            sprintf(enthernet, "%s,voltage_ac,1", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         if (KTAC2 == 1)
         {
            sprintf(enthernet, "%s,voltage_ac2,220", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         else
         {
            sprintf(enthernet, "%s,voltage_ac2,1", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         if (AS == 1)
         {
            sprintf(enthernet, "%s,light,1", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         else
         {
            sprintf(enthernet, "%s,light,2", IP);
            puts1(enthernet);
            delay_ms(100);
         }
         lcd_puts("truyen xong");
         lcd_clear();
      }
      else
      {
         bit_enthernet = 1;
         delay_ms(100);
      }
      //////////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////
      dc_low_11 = EEPROM_read(31) * 10 + EEPROM_read(32);
      dc_low_21 = EEPROM_read(33) * 10 + EEPROM_read(34);
      dc_high1 = EEPROM_read(35) * 10 + EEPROM_read(36);
      if ((dc11 * 10 + dc22 < dc_low_11) & (bit_dc1_cb2 == 0))
      {
         bit_coi = 1;
         bit_dc1_cb2 = 1;
         bit_dc1_cb1 = 0;
         led_vang = 0;
         sprintf(enthernet, "%s,voltage_dc1,%d.%d", IP, dc11, dc12);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV thap hon muc %d,%dV", tram, dc11, dc12, dc_low_11 / 10, dc_low_11 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV thap hon muc %d,%dV", tram, dc11, dc12, dc_low_11 / 10, dc_low_11 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);

         // nho hon nguong
      }
      dc_low_21 = EEPROM_read(33) * 10 + EEPROM_read(34);
      if ((dc11 * 10 + dc22 >= dc_low_11) & (dc11 * 10 + dc22 <= dc_low_21) & (bit_dc1_cb1 == 0))
      {
         bit_dc1_cb2 = 0;
         bit_dc1_cb1 = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,voltage_dc1,%d.%d", IP, dc11, dc12);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV nam trong canh bao muc 1 tu %d,%dV den %d,%dV ", tram, dc11, dc12, dc_low_11 / 10, dc_low_11 % 10, dc_low_21 / 10, dc_low_21 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV nam trong canh bao muc 1 tu %d,%dV den %d,%dV ", tram, dc11, dc12, dc_low_11 / 10, dc_low_11 % 10, dc_low_21 / 10, dc_low_21 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // canh bao 1
      }
      if ((dc11 * 10 + dc22 > dc_low_21) & (dc11 * 10 + dc22 < dc_high1) & ((bit_dc1_cb1 == 1) | (bit_dc1_cb2 == 1)))
      {
         led_vang = 0;
         bit_dc1_cb2 = 0;
         bit_dc1_cb1 = 0;
         sprintf(enthernet, "%s,voltage_dc1,%d.%d", IP, dc11, dc12);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV da ve muc binh thuong tu  %d,%dV den %d,%dV", tram, dc11, dc12, dc_low_21 / 10, dc_low_21 % 10, dc_high1 / 10, dc_high1 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV da ve muc binh thuong tu  %d,%dV den %d,%dV", tram, dc11, dc12, dc_low_21 / 10, dc_low_21 % 10, dc_high1 / 10, dc_high1 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // an toan
      }
      if ((dc11 * 10 + dc22 > dc_high1) & (bit_dc1_cb2 == 0))
      {
         bit_dc1_cb2 = 1;
         bit_dc1_cb1 = 0;
         led_vang = 0;
         bit_coi = 1;
         sprintf(enthernet, "%s,voltage_dc1,%d.%d", IP, dc11, dc12);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV cao hon muc %d,%dV", tram, dc11, dc12, dc_high1 / 10, dc_high1 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC1 hien tai o tram %s la %d,%dV cao hon muc %d,%dV", tram, dc11, dc12, dc_high1 / 10, dc_high1 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // cao hon nguong
      }
      //////////////
      if ((dc21 * 10 + dc22 < dc_low_11) & (bit_dc2_cb2 == 0))
      {
         bit_coi = 1;
         bit_dc2_cb2 = 1;
         bit_dc2_cb1 = 0;
         led_vang = 0;
         sprintf(enthernet, "%s,voltage_dc2,%d.%d", IP, dc21, dc22);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV thap hon muc %d,%dV", tram, dc21, dc22, dc_low_11 / 10, dc_low_11 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV thap hon muc %d,%dV", tram, dc21, dc22, dc_low_11 / 10, dc_low_11 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // nho hon nguong
      }
      if ((dc21 * 10 + dc22 >= dc_low_11) & (dc21 * 10 + dc22 <= dc_low_21) & (bit_dc2_cb1 == 0))
      {
         bit_dc2_cb2 = 0;
         bit_dc2_cb1 = 1;
         led_vang = 1;
         bit_coi = 1;
         sprintf(enthernet, "%s,voltage_dc2,%d.%d", IP, dc21, dc22);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV nam trong canh bao muc 1 tu %d,%dV den %d,%dV ", tram, dc21, dc22, dc_low_11 / 10, dc_low_11 % 10, dc_low_21 / 10, dc_low_21 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV nam trong canh bao muc 1 tu %d,%dV den %d,%dV ", tram, dc21, dc22, dc_low_11 / 10, dc_low_11 % 10, dc_low_21 / 10, dc_low_21 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // canh bao 1
      }
      if ((dc21 * 10 + dc22 > dc_low_21) & (dc21 * 10 + dc22 < dc_high1) & ((bit_dc2_cb1 == 1) | (bit_dc2_cb2 == 1)))
      {
         led_vang = 0;
         bit_dc2_cb2 = 0;
         bit_dc2_cb1 = 0;
         sprintf(enthernet, "%s,voltage_dc2,%d.%d", IP, dc21, dc22);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV da ve muc binh thuong tu  %d,%dV den %d,%dV", tram, dc21, dc12, dc_low_21 / 10, dc_low_21 % 10, dc_high1 / 10, dc_high1 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV da ve muc binh thuong tu  %d,%dV den %d,%dV", tram, dc21, dc12, dc_low_21 / 10, dc_low_21 % 10, dc_high1 / 10, dc_high1 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // an toan
      }
      if ((dc21 * 10 + dc22 > dc_high1) & (bit_dc2_cb2 == 0))
      {
         bit_dc2_cb2 = 1;
         bit_dc2_cb1 = 0;
         bit_coi = 1;
         led_vang = 0;
         sprintf(enthernet, "%s,voltage_dc2,%d.%d", IP, dc21, dc22);
         puts1(enthernet);
         delay_ms(100);
         sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV cao hon muc %d,%dV", tram, dc21, dc22, dc_high1 / 10, dc_high1 % 10);
         send_sms(sdt_sms, sms);
         delay_ms(1000);
         // sprintf(sms, "Dien ap DC2 hien tai o tram %s la %d,%dV cao hon muc %d,%dV", tram, dc21, dc22, dc_high1 / 10, dc_high1 % 10);
         // send_sms(sdt_sms, sms);
         // delay_ms(1000);
         // cao hon nguong
      }

      //////////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////
      bit_canh_bao = bit_temp || bit_humi || bit_ac1 || bit_ac2 || bit_dc1_cb1 || bit_dc1_cb2 || bit_dc2_cb1 || bit_dc2_cb2;
      if ((bit_canh_bao == 1) && (bit_coi == 1))
      {
         coi = 1;
         // led_vang = 1;
      }
      else
      {
         coi = 0;
         // led_vang = 0;
      }
      if (bit_temp | bit_humi | bit_ac1 | bit_ac2 | bit_dc1_cb2 | bit_dc2_cb2)
      {
         led_do = 1;
      }
      else
      {
         led_do = 0;
      }
      ////////////////////////////////////////////////////////////////
      if (bit_canh_bao == 1)
      {
         led_xanh = 0;
      }
      else
      {
         led_xanh = 1;
      }
   }
}