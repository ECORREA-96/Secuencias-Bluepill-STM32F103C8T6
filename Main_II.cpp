#include "mbed.h"

#define BK_R 1s
#define BK_R1 250ms
#define BK_R_LEDS 400ms

void wait(float seconds) { thread_sleep_for(seconds * 1000); }

// DECLARACION DE ENTRADAS(INTERRUPTOR) Y SALIDAS(LEDS)

BusIn INTERRUPTOR(PA_0, PA_1, PA_2); // PUERTOS DIPSWITCH
BusOut DATOS(PA_4, PA_5, PA_6, PA_7, PB_0, PB_1, PB_10, PB_11);
DigitalOut ENABLES(PA_3);
DigitalOut RS(PC_15);

BusOut LEDS(PB_9, PB_8, PB_7, PB_6, PB_5, PB_4, PB_3, PA_15, PA_12, PA_11,PA_10, PA_9, PA_8, PB_15, PB_14, PB_13);

// SECUENCIAS EN ORDEN ESPECIFICADO

// SECUENCIA 1
BusOut SECF1(PB_4, PA_11, PB_15, PA_8, PA_12, PB_5, PB_9, PB_8, PB_7, PB_6,PA_15, PA_9, PB_13, PB_14, PA_10, PB_3);

/*SECUENCIA 2 DIVIDIDA EN DOS PARTES
PARA EL FUNCIONAMIENTO SINCRONO*/
BusOut SECF2_1(PB_8, PB_4, PB_5, PA_12, PA_8, PA_11);  // SECUENCIA F2_1
BusOut SECF2_2(PB_14, PA_10, PA_9, PA_15, PB_6, PB_3); // SECUENCIA F2_2

/*SECUENCIA 3 DIVIDIDA EN CUATRO PARTES
PARA EL ENCENDIDO AL TIEMPO DE DIFERENTES SALIDAS
SE REPITEN LAS SALIDAS PERO NO EN LAS MISMAS CANTIDADES*/
BusOut SECF3_1(PA_8, PA_12, PB_5, PB_9, PB_8, PB_7, PB_6);    // SECUENCIA F3_1
BusOut SECF3_2(PA_8, PA_12, PA_11, PB_4, PB_3, PB_7, PB_6);   // SECUENCIA F3_2
BusOut SECF3_3(PA_8, PA_12, PA_11, PA_10, PB_3, PB_7, PB_6);  // SECUENCIA F3_3
BusOut SECF3_4(PA_8, PB_15, PB_14, PB_13, PA_9, PA_15, PB_6); // SECUENCIA F3_3

// SECUENCIA 4
BusOut SECF4(PA_15, PB_7, PB_8, PB_5, PA_12, PB_15, PB_14, PA_9, PB_3, PB_4,PA_11, PA_10);

/*/SECUENCIA 5 SE DIVIDE EN 4 PARTES QUE
A SU VEZ FUNCIONAN EN DOS PAREJAS PARA EL ENCENDIDO SINCRONO*/
BusOut SECF5_1(PB_5, PB_8, PB_3, PB_6);
BusOut SECF5_2(PA_12, PB_15, PA_10, PB_13);
BusOut SECF5_3(PA_15, PB_7, PB_4, PB_9);
BusOut SECF5_4(PA_9, PB_14, PA_11, PA_8);

void R_I(uint8_t COMANDO);
void R_D(uint8_t CARACTER);
void INICIALIZAR(void);
void F0(void);
void F1(void);
void F2(void);
void F3(void);
void F4(void);
void F5(void);

int main(void) {
  INICIALIZAR();
  while (1) {
    switch (INTERRUPTOR) {
    case 0:
      F0();
      break;
    case 1:
      F1();
      break;
    case 2:
      F2();
      break;
    case 3:
      F3();
      break;
    case 4:
      F4();
      break;
    case 5:
      F5();
      break;
    default:
      F0();
      break;
    }
  }
}

void R_I(uint8_t COMANDO) {
  RS = 0;
  ENABLES = 0;
  DATOS = COMANDO;
  ENABLES = 1;
  wait_us(10);
  ENABLES = 0;
  wait_us(2000);
}

void R_D(uint8_t CARACTER) {
  RS = 1;
  ENABLES = 0;
  DATOS = CARACTER;
  ENABLES = 1;
  wait_us(10);
  ENABLES = 0;
  wait_us(2000);
}

void INICIALIZAR(void) {
  wait_us(50000);
  R_I(0b00110000);

  wait_us(25000);
  R_I(0b00110000);

  wait_us(1000);
  R_I(0b00110000);

  R_I(0b00111000); // Activa la segunda línea y establece la interfaz en 8 bits
  R_I(0b00001111); // Cursor parpadeando
  R_I(0b00000001); // Limpiar pantalla y volver cursor a inicio
}

void F0(void) {
  R_I(0b00000001); // Limpiar la pantalla
  char POS[15] = {133, 196, 137, 199, 131, 202, 135,200, 136, 198, 134, 197, 132, 201};
  char Letras[15] = {"GEARSANEDTUNEG"};
  uint8_t i = 0;
  R_I(0b00001100);
  for (i = 0; i < 14; i++) {
    R_I(POS[i]);    // Colocar cursor en la posición indicada
    R_D(Letras[i]); // Mostrar la letra correspondiente
    wait(0.2);      // Tiempo de espera entre cada letra
  }
}

void F1(void) {
  R_I(0b00000001);
  char POS[16] = {207, 132, 142, 199, 128, 205, 134,197, 130, 201, 136, 203, 140, 195};
  char Letras[16] = {"DCEVLAEAUISDDN"};
  uint8_t i = 0;
  R_I(0b00001100); // Desactivar parpadeo del cursor y mostrar el cursor

  for (i = 0; i < 14; i++) {
    R_I(POS[i]);
    R_D(Letras[i]);
    wait(0.2);
  }
  uint16_t AA = 0;

  SECF1 = 0;
  wait(0.4);
  AA = 1;
  for (i = 0; i < 16; i++) {
    SECF1 = AA;
    wait(0.4);
    AA = (AA << 1 | 1); // enciende los LEDs uno por uno, moviendo un bit a la
  }
  AA = 65534; // 1111111111111110
  for (i = 0; i < 17; i++) {
    SECF1 = AA;
    wait(0.4);
    AA = AA << 1;
  }

  R_I(1);
}

// MENSAJE # 2 JUNTO CON SECUENCIA
void F2(void) {
  R_I(0b00000001);
  char POS[15] = {207, 132, 141, 199, 128, 205, 134,197, 130, 201, 136, 203, 138, 143};
  char Letras[15] = {"OLDIMTIVOENNOE"};
  uint8_t i = 0;
  R_I(0b00001100); // Desactivar parpadeo del cursor y mostrar el cursor

  for (i = 0; i < 14; i++) {
    R_I(POS[i]);
    R_D(Letras[i]);
    wait(0.2);
  }
  SECF2_1 = SECF2_2 = 0;
  wait(0.4);
  uint8_t AA = 0;
  AA = 1;
  for (i = 0; i < 6; i++) {
    SECF2_1 = SECF2_2 = AA;
    wait(0.4);
    AA = (AA << 1 | 1);
  }

  AA = 31;
  for (i = 0; i < 6; i++) {
    SECF2_1 = SECF2_2 = AA;
    wait(0.4);
    AA = AA >> 1;
  }

  //R_I(1);
}
// MENSAJE # 2 JUNTO CON SECUENCIA
void F3(void) {
  R_I(0b00000001);
  char POS[10] = {135, 132, 139, 136, 131, 137, 134, 138, 133};
  char Letras[10] = {"NIIAFCOHB"};
  uint8_t i = 0;
  uint16_t AA = 0;

  R_I(0b00001100); // Desactivar parpadeo del cursor y mostrar el cursor
  for (i = 0; i < 8; i++) {
    R_I(POS[i]);
    R_D(Letras[i]);
    wait(0.2);
  }
  SECF3_1 = SECF3_2 = SECF3_3 = SECF3_4 = 0;
  AA = 1;
  for (i = 0; i < 7; i++) {
    SECF3_1 = SECF3_2 = SECF3_3 = SECF3_4 = AA;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = AA << 1 | 1;
  }
  AA = 127;
  for (i = 0; i < 8; i++) {
    SECF3_1 = SECF3_2 = SECF3_3 = SECF3_4 = AA;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = AA >> 1;
  }
}
void F4(void) {
  R_I(0b00000001);
  char POS[14] = {140, 128, 207, 198, 130, 200, 203, 132, 138, 205, 136, 134};
  char Letras[14] = {"ALGDEEAYDNNE"};
  uint8_t i = 0;
  uint16_t AA = 0;
  R_I(0b00001100); // Desactivar parpadeo del cursor y mostrar el cursor
  for (i = 0; i < 12; i++) {
    R_I(POS[i]);
    R_D(Letras[i]);
    wait(0.2);
  }
  SECF4 = 0;
  AA = 1;
  for (i = 0; i < 12; i++) {
    SECF4 = AA;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = AA << 1 | 1;
  }
  AA = 4095;
  for (i = 0; i < 13; i++) {
    SECF4 = AA;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = AA >> 1;
  }
}
void F5(void) {
  R_I(0b00000001);
  char POS[11] = {195, 133, 205, 137, 203, 197, 135, 201, 199};
  char Letras[11] = {"OPEZSJEPE"};
  uint8_t i = 0;
  uint8_t AA = 0;
  uint8_t BB = 0;
  R_I(0b00001100); // Desactivar parpadeo del cursor y mostrar el cursor
  for (i = 0; i < 9; i++) {
    R_I(POS[i]);
    R_D(Letras[i]);
    wait(0.2);
  }
  SECF5_1 = SECF5_2 = SECF5_3 = SECF5_4 = 0;
  AA = 1;
  for (i = 0; i < 4; i++) {
    SECF5_1 = SECF5_2 = AA;
    ThisThread::sleep_for(BK_R_LEDS);
    AA = AA << 1 | 1;
  }
  SECF5_1 = SECF5_2 = 0;
  ThisThread::sleep_for(BK_R_LEDS);
  BB = 1;
  for (i = 0; i < 5; i++) {
    SECF5_3 = SECF5_4 = BB;
    ThisThread::sleep_for(BK_R_LEDS);
    BB = BB << 1 | 1;
  }
  BB = 0;
  ThisThread::sleep_for(BK_R_LEDS);
}
