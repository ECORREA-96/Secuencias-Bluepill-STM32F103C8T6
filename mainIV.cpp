#include "mbed.h"

UnbufferedSerial bluetooth(PB_10, PB_11); 
AnalogIn sensor(PA_0);                  
char buffer[100];
float voltajeAnterior = 0;           
const float variacionMinima = 0.01;  

void enviarBluetooth(const char* mensaje);
void floatToString(char* buffer, float valor, int precision);

int main() {
    while (1) {
        float valorAnalogico = sensor.read();       
        float voltaje = valorAnalogico * 3.24;   
     
        if (fabs(voltaje - voltajeAnterior) >= variacionMinima) {
            voltajeAnterior = voltaje; 
            voltaje=voltaje*3;            
            int volt=voltaje/1;
            int dec=((voltaje*100)-(volt*100))/1;
            sprintf(buffer, "Voltaje: %i.%i V\r\n", volt,dec);
            enviarBluetooth(buffer);                 
        }
        
        ThisThread::sleep_for(500ms); 
    }
}

void enviarBluetooth(const char* mensaje) {
    int i = 0;
    while (mensaje[i] != '\0') {
        if (bluetooth.writeable()) {
            bluetooth.write(&mensaje[i], 1);
            i++;
        }
    }
}

void floatToString(char* buffer, float valor, int precision) {
    int entero = (int)valor;                               
    int decimal = (int)((valor - entero) * pow(10, precision));  
    sprintf(buffer, "%d.%0*d", entero, precision, decimal);  
}
