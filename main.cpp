#include "mbed.h"
#define BK_R_LEDS 400ms

//DECLARACION DE ENTRADAS(SELECTOR) Y SALIDAS(LEDS)

BusIn Selector (PA_0,PA_1,PA_2);//PUERTOS DIPSWITCH

BusOut LEDS (PB_9,PB_8,PB_7,PB_6,PB_5,PB_4,PB_3,PA_15,PA_12,PA_11,PA_10,PA_9,PA_8,PB_15,PB_14,PB_13);//SALIDA PRINCIPAL ORDEN DE LOS LEDS


//SECUENCIAS EN ORDEN ESPECIFICADO

//SECUENCIA 1
BusOut SECF1(PB_4,PA_11,PB_15,PA_8,PA_12,PB_5,PB_9,PB_8,PB_7,PB_6,PA_15,PA_9,PB_13,PB_14,PA_10,PB_3);

/*SECUENCIA 2 DIVIDIDA EN DOS PARTES 
PARA EL FUNCIONAMIENTO SINCRONO*/
BusOut SECF2_1(PB_8,PB_4,PB_5,PA_12,PA_8,PA_11);//SECUENCIA F2_1
BusOut SECF2_2(PB_14,PA_10,PA_9,PA_15,PB_6,PB_3);//SECUENCIA F2_2

/*SECUENCIA 3 DIVIDIDA EN CUATRO PARTES 
PARA EL ENCENDIDO AL TIEMPO DE DIFERENTES SALIDAS
SE REPITEN LAS SALIDAS PERO NO EN LAS MISMAS CANTIDADES*/
BusOut SECF3_1(PA_8,PA_12,PB_5,PB_9,PB_8,PB_7,PB_6);//SECUENCIA F3_1
BusOut SECF3_2(PA_8,PA_12,PA_11,PB_4,PB_3,PB_7,PB_6);//SECUENCIA F3_2
BusOut SECF3_3(PA_8,PA_12,PA_11,PA_10,PB_3,PB_7,PB_6);//SECUENCIA F3_3
BusOut SECF3_4(PA_8,PB_15,PB_14,PB_13,PA_9,PA_15,PB_6);//SECUENCIA F3_3

//SECUENCIA 4
BusOut SECF4(PA_15,PB_7,PB_8,PB_5,PA_12,PB_15,PB_14,PA_9,PB_3,PB_4,PA_11,PA_10);

/*/SECUENCIA 5 SE DIVIDE EN 4 PARTES QUE 
A SU VEZ FUNCIONAN EN DOS PAREJAS PARA EL ENCENDIDO SINCRONO*/
BusOut SECF5_1(PB_5,PB_8,PB_3,PB_6);
BusOut SECF5_2(PA_12,PB_15,PA_10,PB_13);
BusOut SECF5_3(PA_15,PB_7,PB_4,PB_9);
BusOut SECF5_4(PA_9,PB_14,PA_11,PA_8);


//DECLARACION DE FUNCIONES
void F0(void);
void F1(void);
void F2(void);
void F3(void);
void F4(void);
void F5(void);


int main(void)
{
    while(1)
       {
        switch(Selector)
	    {
          case 0:F0();break;
          case 1:F1();break;
          case 2:F2();break;
          case 3:F3();break;
          case 4:F4();break;
          case 5:F5();break;
          default:F0();break;
        }	
       }
}

//IMPLEMENTACION DE LAS FUNCIONES ANTERIORMENTE DECLARADAS

void F0(void)
{
    LEDS=0;
    ThisThread::sleep_for(BK_R_LEDS);
}

void F1(void)
{   
    uint16_t AA=0;
    uint8_t i=0;
    
    SECF1 = 0;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = 1;
        for(i=0;i<16;i++)
	    {
            SECF1 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = (AA << 1 | 1);    
	    }
    AA=65534;
        for(i=0;i<16;i++)
        {
            SECF1 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA << 1;
        }
}

void F2(void)
{
    uint8_t AA=0;
    uint8_t i=0;

    SECF2_1 = SECF2_2 = 0;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = 1;
        for(i=0;i<6;i++)
	    {
            SECF2_1 = SECF2_2 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = (AA << 1|1);    
	    }
    AA=31;
        for(i=0;i<5;i++)
        {
            SECF2_1 = SECF2_2 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA >> 1;  
        }
}
void F3(void)
{
    uint16_t AA=0;
    uint8_t i=0;
    SECF3_1 = SECF3_2 = SECF3_3 = SECF3_4 = 0;
    AA = 1;
        for(i=0;i<7;i++)
        {
        SECF3_1 = SECF3_2 = SECF3_3 = SECF3_4 = AA;
        ThisThread::sleep_for(BK_R_LEDS);
        AA = AA << 1 | 1;  
        }
    AA = 127;
        for(i=0;i<7;i++)
        {   
            SECF3_1=SECF3_2=SECF3_3=SECF3_4=AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA >> 1;
        }
}   
void F4(void)
{
    uint16_t AA=0;
    uint8_t i=0;
    SECF4 = 0;
    AA=1;
        for(i=0;i<12;i++)
        {
            SECF4 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA << 1 | 1;
        }
    AA=4095;
        for(i=0;i<12;i++)
        {
            SECF4 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA >> 1;
        }
}
void F5(void)
{
   uint8_t AA =0;
   uint8_t BB =0;
   uint8_t i=0;
   SECF5_1 = SECF5_2 = SECF5_3 = SECF5_4 = 0;
   AA=1;
        for(i=0;i<4;i++)
        {
            SECF5_1 = SECF5_2 = AA;
            ThisThread::sleep_for(BK_R_LEDS);
            AA = AA << 1 | 1;
        }
    SECF5_1 = SECF5_2 = 0;
    ThisThread::sleep_for(BK_R_LEDS);
    BB=1;
        for(i=0;i<4;i++)
        {
            SECF5_3 = SECF5_4 = BB;
            ThisThread::sleep_for(BK_R_LEDS);
            BB = BB << 1 |1;
        }
    BB = 0;
    ThisThread::sleep_for(BK_R_LEDS);
}
