#include "mbed.h"

// Configuración del Ticker, Serial y PWM
Ticker ORGANIZADOR;                      // Crea objeto que permite gestionar ISR
UnbufferedSerial PUERTO_PC(PA_9, PA_10); // (TX, RX)
PwmOut REVOLUCIONES(PB_13);              // PWM en pin PB_13
AnalogIn PIN_A0(A0);                     // Entrada analógica en pin A0

// Variables y banderas
volatile int8_t BANDERA = 0;             // Variable de bandera para la interrupción
char RX_BUFFER[10];                      // Buffer para recibir datos seriales
int index = 0;                           // Índice del buffer de recepción
float dutyCycle = 0.5f;                  // Duty cycle inicial al 10%

// Prototipos de funciones
void INTERRUP_PERIODICA(void);           // Interrupción periódica
int CAMBIAR_VELOCIDAD(void);         // Tarea para procesar el ciclo útil ingresado
void leerVelocidad(void);                // Función para leer la velocidad del motor

int main() {
    // Configurar el periodo del PWM y el Ticker
    REVOLUCIONES.period_ms(1);           // Establecer el periodo de 1 ms
    ORGANIZADOR.attach(&INTERRUP_PERIODICA, 0.1); // Ejecuta la interrupción cada 100 ms
    
    
        int validar=0;
        int imprimir=0;
    while (1) {
        if(validar==imprimir){
		// Mensaje de bienvenida y solicitud inicial
        PUERTO_PC.write("\n\r\t\tCONTROL DE VELOCIDAD\n\r---------------------\n\r", 52);
        PUERTO_PC.write("Seleccione la velocidad:\r\n", 25);
        PUERTO_PC.write("1 = 10%\r\n", 8);
        PUERTO_PC.write("2 = 20%\r\n", 8);
        PUERTO_PC.write("3 = 30%\r\n", 8);
        PUERTO_PC.write("4 = 40%\r\n", 8);
        PUERTO_PC.write("5 = 50%\r\n", 8);
        PUERTO_PC.write("6 = 60%\r\n", 8);
        PUERTO_PC.write("7 = 70%\r\n", 8);
        PUERTO_PC.write("8 = 80%\r\n", 8);
        PUERTO_PC.write("9 = 90%\r\n", 8);
        PUERTO_PC.write("0 = 100%\r\n", 9);
        
        imprimir=1;
        }
        if (BANDERA == 1) {
            BANDERA = 0;
            imprimir=CAMBIAR_VELOCIDAD(); // Procesa el ciclo ingresado cuando hay datos
            leerVelocidad();     // Leer y enviar la velocidad del motor
        }
    }
}

// Función para limpiar el buffer y resetear el índice
void reinicioBuffer(char* buffer, int& index) {
    for (int i = 0; i < 10; i++) {
        buffer[i] = '\0';
    }
    index = 0;
}

// Tarea de procesamiento de ciclo útil ingresado
int CAMBIAR_VELOCIDAD() {
    int v=1;
    if (PUERTO_PC.readable()) { // Comprobar si hay datos disponibles
            int valorIngresado = atoi(RX_BUFFER); // Convertir a entero
            char mensaje[50];
            v=0;
			PUERTO_PC.read(&RX_BUFFER[index], 1); // Leer un carácter
            // Validar y calcular el duty cycle
            if (valorIngresado >= 0 && valorIngresado <= 5) {
				if (valorIngresado==1){
					dutyCycle = 0.1f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 10%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==2){
					dutyCycle = 0.2f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 20%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==3){
					dutyCycle = 0.3f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 30%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==4){
					dutyCycle = 0.4f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 40%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==5){
					dutyCycle = 0.5f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 50%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==6){
					dutyCycle = 0.6f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 60%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==7){
					dutyCycle = 0.7f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 70%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==8){
					dutyCycle = 0.8f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 80%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==9){	
					dutyCycle = 0.9f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 90%\r\n");
					PUERTO_PC.write(mensaje, longitud);}
                if (valorIngresado==0){
					dutyCycle = 1.0f;
					int longitud = sprintf(mensaje, "Velocidad establecida: 100%\r\n");
					PUERTO_PC.write(mensaje, longitud);} // Duty cycle en porcentaje
                REVOLUCIONES.write(dutyCycle); // Establece el duty cycle
                v=0;
				} 
            reinicioBuffer(RX_BUFFER, index);
    }
    return v;
}

// Interrupción periódica
void INTERRUP_PERIODICA(void) {
    BANDERA = 1; // Establece la bandera para activar la lectura en el main
}

// Función para leer la velocidad del motor
void leerVelocidad(void) {
    // Leer el valor de la entrada analógica A0
    float lecturaAnalogica = PIN_A0.read();
    
    // Convertir la lectura a una velocidad en RPM (suponiendo que 0-3.3V mapea a 0-10000 RPM)
    // Puedes ajustar esta conversión según las características de tu sensor
    float velocidadRPM = lecturaAnalogica * 10000.0f;
    
    // Enviar la velocidad medida a través del puerto serial
    char mensaje[50];
    int longitud = sprintf(mensaje, "Velocidad medida: %.2f RPM\r\n", velocidadRPM);
    PUERTO_PC.write(mensaje, longitud);
}
