#ifndef CALIBRATION_H
#define CALIBRATION_H

extern int8_t Calibration_humidity;
extern int8_t Calibration_temperature;

#define Calibration_getHumi() Calibration_humidity
#define Calibration_getTemp() Calibration_temperature

int8_t Calibration_State_Temp(int8_t input);
int8_t Calibration_State_Humi(int8_t input);
int8_t Calibration_State_Back(int8_t input);

void Calibration(void);

#endif