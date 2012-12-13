#ifndef FANS_H
#define FANS_H

// Variablen

typedef struct {
	uint8_t ctrl;
	uint8_t humi;
	uint8_t timeH;
	uint8_t timeM;
	uint8_t dur;
	uint8_t rep;
} Fan_t;

extern Fan_t Fan_fans[];

// Makros



// Funktions-Dummies

int8_t Fan_State_1_Ctrl(int8_t input);
int8_t Fan_State_1_Humi(int8_t input);
int8_t Fan_State_1_On(int8_t input);
int8_t Fan_State_1_Rep(int8_t input);
int8_t Fan_State_1_Dur(int8_t input);
int8_t Fan_State_1_Back(int8_t input);

int8_t Fan_State_2_Ctrl(int8_t input);
int8_t Fan_State_2_Humi(int8_t input);
int8_t Fan_State_2_On(int8_t input);
int8_t Fan_State_2_Rep(int8_t input);
int8_t Fan_State_2_Dur(int8_t input);
int8_t Fan_State_2_Back(int8_t input);

int8_t Fan_State_3_Ctrl(int8_t input);
int8_t Fan_State_3_Humi(int8_t input);
int8_t Fan_State_3_On(int8_t input);
int8_t Fan_State_3_Rep(int8_t input);
int8_t Fan_State_3_Dur(int8_t input);
int8_t Fan_State_3_Back(int8_t input);

int8_t Fan_State_4_Ctrl(int8_t input);
int8_t Fan_State_4_Humi(int8_t input);
int8_t Fan_State_4_On(int8_t input);
int8_t Fan_State_4_Rep(int8_t input);
int8_t Fan_State_4_Dur(int8_t input);
int8_t Fan_State_4_Back(int8_t input);

// Eigentliche Funktionen

int8_t Fan_State_Ctrl(uint8_t fan, int8_t input);

int8_t Fan_State_Humi(uint8_t fan, int8_t input);

int8_t Fan_State_On(uint8_t fan, int8_t input);

int8_t Fan_State_Rep(uint8_t fan, int8_t input);

int8_t Fan_State_Dur(uint8_t fan, int8_t input);

int8_t Fan_State_Back(uint8_t fan, int8_t input);

// Task

Task_t* Fans(void);

#endif