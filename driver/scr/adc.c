
#include "adc.h"
#include "clock.h"
#include "stm32f407xx.h"

#define ADC1_ADDRESS_BASE 0X400120001
#define ADC1_IN16  16
void adc_init()
{
    clock_enable_APB2(ADC1_peripheral);

    u32 *ADC_CR1 = (u32*)(ADC1_ADDRESS_BASE + 0X04);
    u32 *ADC_CR2 = (u32*)(ADC1_ADDRESS_BASE + 0X08);
    u32 *ADC_SR = (u32*)(ADC1_ADDRESS_BASE + 0X00);
    u32 *ADC_JSQR = (u32*)(ADC1_ADDRESS_BASE + 0X38);
    u32 *ADC_CCR = (u32*)(ADC1_ADDRESS_BASE + 0X04 + 0x300);
    u32 *ADC_SMPR1 = (u32*)(ADC1_ADDRESS_BASE + 0X0C);
    *ADC_SMPR1 |= (0b111 << 18);  // using 12 bit resolution (request 15 cycles for conversion)
    *ADC_JSQR &= ~(0b11 << 20); // Injected sequence legth=00 : 1 conversion
    *ADC_JSQR |= (ADC1_IN16 << 15); //set source for JSQ4 is temp sensor(IN16)
    *ADC_CCR |= 1 << 23; // enable temperature sensor
    *ADC_CR2 |= 1 << 0; //enable ADC1
    

        
}
float adc_get_temp_ss()
{
    float temp = 0;
    float vin = 0;
    u16 raw_data = 0;

    //trigger ADC start convert
    u32 *ADC_CR2 = (u32*)(ADC1_ADDRESS_BASE + 0X08);
    u32 *ADC_SR = (u32*)(ADC1_ADDRESS_BASE + 0X00);
    u32 *ADC_JDR1 = (u32*)(ADC1_ADDRESS_BASE + 0X08);
    *ADC_CR1 |= 1 << 2;
     
     // wait until end of conversion
    while(((*ADC_SR >> 2) & 1) == 0);
    *ADC_SR &= ~(1 << 2); //clear JEOCM fiag

     // read ADC data from JDR1 (data of JSQ4)
     raw_data = *ADC_JDR1;
     vin = (raw_data * 3.0) / 4095.0;
     temp = ((VSENSE - V25)  Avg_Slope)

       
     
}