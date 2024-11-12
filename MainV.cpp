#include "mbed.h"

#define WAIT_TIME 50  // En milisegundos

PwmOut SALIDA_R(PB_13);
PwmOut SALIDA_G(PB_14);
PwmOut SALIDA_B(PB_15);

BusOut DATOS(PC_15,PA_0,PA_1,PA_2,PA_3,PA_4,PA_5,PA_6);
DigitalIn Pulsador(PB_10);
DigitalOut ENABLES(PC_14);
DigitalOut RS(PC_13);

char buffer[100];
UnbufferedSerial bluetooth(PB_10, PB_11);

void enviarBluetooth(const char* mensaje);
void R_I(uint8_t COMANDO);
void R_D(uint8_t CARACTER);
void INICIALIZAR(void);
void mostrarColorEnPantallaYLED(const char* texto, float R, float G, float B);

int main() {
    INICIALIZAR();
    SALIDA_R.period_ms(20);
    SALIDA_G.period_ms(20);
    SALIDA_B.period_ms(20);

    int color = 0;
    int cambio=0;
    int imprimir=0;
    while (1) {

        if (Pulsador == 0) {
            cambio=0;
        }
        if(cambio==0){
            color = (color < 13) ? color + 1 : 0;
            thread_sleep_for(200);  // Anti-rebote en milisegundos
        }
        cambio=1;

        switch (color) {
            case 0:
                if(imprimir==color){
                mostrarColorEnPantallaYLED("Rojo Naranja R=100 G=25 B=0", 1.0f, 0.25f, 0.0f);
                 }
                imprimir=color+1; 
                break;
            case 1:
                if(imprimir==color){
                    
                mostrarColorEnPantallaYLED("Rojo R=100 G=0 B=0", 1.0f, 0.0f, 0.0f);

                }
                imprimir=color+1;
                break;
            case 2:
                if(imprimir==color){
                    
                mostrarColorEnPantallaYLED("Rojo Violeta R=75 G=0 B=25", 0.75f, 0.0f, 0.25f);
                }
                imprimir=color+1;
                break;
            case 3:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Violeta R=50 G=0 B=50", 0.5f, 0.0f, 0.5f);
                }
                imprimir=color+1;
                
                break;
            case 4:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Azul Violeta R=40 G=11 B=55", 0.40f, 0.1137f, 0.5529f);
                }
                imprimir=color+1;
                break;
            case 5:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Azul R=48 G=40 B=93", 0.4823f, 0.4078f, 0.9333f);
                }
                imprimir=color+1;
                
                break;
            case 6:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Azul Verde R=00 G=70 B=98", 0.0f, 0.70f, 0.9803f);
                }
                imprimir=color+1;
                
                break;
            case 7:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Verde R=0 G=50 B=0", 0.0f, 0.5f, 0.0f);
                }
                imprimir=color+1;
                
                break;
            case 8:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Verde Amarillo R=50 G=100 B=66", 0.250f, 0.50f, 0.25f);
                }
                imprimir=color+1;
                
                break;
            case 9:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Amarillo R=100 G=100 B=0", 1.0f, 1.0f, 0.0f);
                }
                imprimir=color+1;
                
                break;
            case 10:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Amarillo Naranja R=100 G=50 B=0", 1.0f, 0.50f, 0.00f);
                }
                imprimir=color+1;
                
                break;
            case 11:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Naranja R=100 G=25 B=0", 1.0f, 0.25f, 0.0f);
                }
                imprimir=color+1;
                
                break;
            case 12:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Blanco R=100 G=100 B=100", 1.0f, 1.0f, 1.0f);
                }
                imprimir=color+1;
                
                break;
            case 13:
                if(imprimir==color){
                    mostrarColorEnPantallaYLED("Negro R=10 G=10 B=10", 0.10f, 0.10f, 0.10f);
                }
                imprimir=0;
                
                break;
        }
    }
}

void mostrarColorEnPantallaYLED(const char* texto, float R, float G, float B) {
    SALIDA_R.write(R);
    SALIDA_G.write(G);
    SALIDA_B.write(B);

    sprintf(buffer, "%s", texto);  // Corregido con "%s" para evitar advertencias de formato
    R_I(0b00000001);  // Limpiar pantalla
    char POS[30] = {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,140,141,142,143, 192, 193, 194, 195, 196, 197, 198, 199, 200, 202, 203, 204, 205};  
    for (uint8_t i = 0; texto[i] != '\0' && i < 33; i++) {
        R_I(POS[i]);
        R_D(texto[i]);
        thread_sleep_for(WAIT_TIME);
    }
}

// Implementación de funciones de inicialización y pantalla LCD
void R_I(uint8_t COMANDO) {
    RS = 0;
    ENABLES = 0;
    DATOS = COMANDO;
    ENABLES = 1;
    thread_sleep_for(10);  // En milisegundos
    ENABLES = 0;
    thread_sleep_for(2);  // 2000 microsegundos son 2 milisegundos
}

void R_D(uint8_t CARACTER) {
    RS = 1;
    ENABLES = 0;
    DATOS = CARACTER;
    ENABLES = 1;
    thread_sleep_for(10);  // En milisegundos
    ENABLES = 0;
    thread_sleep_for(2);  // 2000 microsegundos son 2 milisegundos
}

void INICIALIZAR() {
    thread_sleep_for(50);  // En milisegundos
    R_I(0b00110000);
    thread_sleep_for(25);  // En milisegundos
    R_I(0b00110000);
    thread_sleep_for(1);  // En milisegundos
    R_I(0b00110000);
    R_I(0b00111000);  // 2 líneas y modo 8 bits
    R_I(0b00001111);  // Cursor parpadeante
    R_I(0b00000001);  // Limpiar pantalla
}
