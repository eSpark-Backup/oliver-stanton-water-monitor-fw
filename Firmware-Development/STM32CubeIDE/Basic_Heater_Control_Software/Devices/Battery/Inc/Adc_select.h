// adc_select.h
#ifndef ADC_SELECT_H
#define ADC_SELECT_H

#include <cstdint>

// Forward declarations of the classes
class TDS;
class BatteryMonitor;

// Declaration of the friend function
void ADCSelectCH(uint32_t channel);

#endif // ADC_SELECT_H
