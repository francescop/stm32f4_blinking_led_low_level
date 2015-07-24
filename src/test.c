#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx_conf.h"
#include "utils.h"
#include "delay.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void Delay(__IO uint32_t nCount);
void print_bin(uint32_t v);

#define LED_GREEN      12
#define LED_ORANGE     13
#define LED_RED        14
#define LED_BLUE       15

#define t_PERIPH_BASE           (0x40000000) /*!< Peripheral base address in the alias region                                */
#define t_AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define t_RCC_BASE              (t_AHB1PERIPH_BASE + 0x3800)
#define t_RCC_AHB1ENR           (( uint32_t *) (uint32_t)( t_RCC_BASE + 0x30 ))  /* (RCC_AHB1ENR),                        Address offset: 0x30     */

#define t_GPIOD_BASE  ( 0x40020C00 )
#define t_GPIOD       (( uint32_t *) t_GPIOD_BASE )
#define t_MODER       (( uint32_t *) t_GPIOD_BASE + 0x00) /*!> GPIO port mode register,               Address offset: 0x00      */

#define t_GPIOD_BSRR  (( uint32_t *) ( t_GPIOD_BASE + 0x18 ))   /* GPIOx_BSRR                           Address offset: 0x18      */

#define t_OTYPER      (( uint32_t *) t_GPIOD_BASE + 0x04)   /*!> GPIO port output type register,        Address offset: 0x04      */
#define t_OSPEEDR     (( uint32_t *) t_GPIOD_BASE + 0x08)  /*!> GPIO port output speed register,       Address offset: 0x08      */
#define t_PUPDR       (( uint32_t *) t_GPIOD_BASE + 0x0c)    /*!> GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
#define t_IDR         (( uint32_t *) t_GPIOD_BASE + 0x10)      /*!> GPIO port input data register,         Address offset: 0x10      */
#define t_ODR         (( uint32_t *) t_GPIOD_BASE + 0x14)      /*!> GPIO port output data register,        Address offset: 0x14      */
#define t_BSRRL       (( uint16_t *) t_GPIOD_BASE + 0x18)    /*!> GPIO port bit set/reset low register,  Address offset: 0x18      */
#define t_BSRRH       (( uint16_t *) t_GPIOD_BASE + 0x1A)    /*!> GPIO port bit set/reset high register, Address offset: 0x1A      */
#define t_LCKR        (( uint32_t *) t_GPIOD_BASE + 0x1C)     /*!> GPIO port configuration lock register, Address offset: 0x1C      */

int main() {

  int pin = LED_RED;
  
  uint32_t clock = 0x01 << 3; 
  uint32_t mode = 0x01 << (pin * 2); 

  uint32_t speed = 0x01 << (pin * 2);
  uint32_t type = 0x00 << pin;
  uint32_t pullup = 0x00 << (pin * 2);

  //
  //  Initialise the peripheral clock.
  //

  printf("rcc->ahb1enr address: %p\n", ( uint32_t * )t_RCC_AHB1ENR);
  printf("rcc->ahb1enr value: %lu\n", *t_RCC_AHB1ENR);
  
  *t_RCC_AHB1ENR |= clock;

  //uint32_t *value = (uint32_t *) 0x40020c60;

  uint32_t *value = (uint32_t *) ( t_GPIOD_BASE + 0x18 ); // input data register

  //
  //  Initilaise the GPIO port.
  //

  *t_MODER |= mode; 
  *t_OSPEEDR |= speed;
  *t_OTYPER |= type;
  *t_PUPDR |= pullup;


  //
  //  Toggle the selected LED indefinitely.
  //
  while (1) {

    *t_GPIOD_BSRR |= (0 << 14);
    *t_GPIOD_BSRR |= (1 << 30);

    delay(500);

    *t_GPIOD_BSRR |= (0 << 30);
    *t_GPIOD_BSRR |= (1 << 14);

    delay(500);
  }
}


void _init() {
}

void print_bin(uint32_t v){
  int c, k;
  uint32_t p = v;
  for (c = 31; c >= 0; c--) {
    k = p >> c;
 
    if (k & 1)
      printf("1");
    else
      printf("0");
  }
  printf("\n");
}

void Delay(__IO uint32_t nCount) { 
  while(nCount--) { } 
}
