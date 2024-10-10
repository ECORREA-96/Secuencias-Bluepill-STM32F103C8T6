#include "mbed.h"
#include "stdio.h"
#include <cstdint>
#include <string.h>
#include <ctype.h>

UnbufferedSerial pc(PA_2, PA_3);
UnbufferedSerial celular(PB_10, PB_11);
DigitalOut ledRojo(PA_7);     
DigitalOut ledVerde(PB_0);

char buffer[500];       
char contrasena[10];
char contrasenaIngresada[10];
char *habitaciones[] = {"301", "302", "303"};
char contrasenaHabitacion1[10];
char contrasenaHabitacion2[10];
char contrasenaHabitacion3[10];
char habitacionSeleccionada[10];
char caracterRecibido;
uint8_t resultadoValidacion;

void enviarPC(uint8_t longitud);
void enviarCelular(uint8_t longitud);
void limpiarContrasena(void);
void leerDesdePC(uint8_t habitacion);
void leerDesdeCelular(void);
void configuracionInicial(void);
void validarHabitacion(void);
uint8_t validarContrasena(void);
void opcionSeleccionHabitacion(void);
uint8_t validarFormatoContrasena(const char* contrasena);
void limpiarPantallaPC(void);

int main() {
    ledVerde = 0;
    ledRojo = 0;
    limpiarPantallaPC();
    configuracionInicial();
    
    while (1) {        
        validarHabitacion();
        do {
            resultadoValidacion = validarContrasena();
        } while (resultadoValidacion == 0);
        
        opcionSeleccionHabitacion(); 
    }   
}

void limpiarPantallaPC(void) {
    sprintf(buffer, "\033[2J\033[H"); // Comando ANSI para limpiar la pantalla
    enviarPC(strlen(buffer));
}

void enviarPC(uint8_t longitud) {
    int indice = 0; 
    do {
        if (pc.writeable()) {
            pc.write(&buffer[indice], 1);
            indice++;
        }    
    } while (indice < longitud);
}

void enviarCelular(uint8_t longitud) {
    int indice = 0; 
    do {
        if (celular.writeable()) {
            celular.write(&buffer[indice], 1);
            indice++;
        }    
    } while (indice < longitud);
}

void limpiarContrasena(void) {
    for (int i = 0; i < 10; i++) {
        contrasena[i] = '\0';
    }
}

void leerDesdePC(uint8_t habitacion) {
    int indice = 0;  
    while (true) { 
        limpiarContrasena(); 
        indice = 0;
        enviarPC(sprintf(buffer, "Parametros requeridos para cada contraseña:\n\r")); 
        enviarPC(sprintf(buffer, "\n\r1. minimo 4 caracteres\n\r"));
        enviarPC(sprintf(buffer, "2. debe contener 2 numeros\n\r"));
        enviarPC(sprintf(buffer, "3. debe contener 2 letras \n\r"));
        enviarPC(sprintf(buffer, "4. No repetir los caracteres\n\r"));
        enviarPC(sprintf(buffer, "5. No se deben repetir las contraseñas\n\r"));                 
        
        bool contrasenaValida = false; 
        while (indice < 4) { 
            if (pc.readable()) {
                pc.read(&caracterRecibido, 1);
                if (caracterRecibido == '\r') {
                    break; 
                } else if (caracterRecibido != '\n') { 
                    contrasena[indice] = caracterRecibido;
                    indice++;
                }
            }
        }
        
        if (strcmp(contrasena, contrasenaHabitacion1) == 0 || strcmp(contrasena, contrasenaHabitacion2) == 0 || strcmp(contrasena, contrasenaHabitacion3) == 0) {
            enviarPC(sprintf(buffer, "ESTA CONTRASENA YA EXISTE.\n\r"));
            enviarPC(sprintf(buffer, "Por favor ingrese una diferente.\n\r"));
            ThisThread::sleep_for(1s);
            continue; 
        }
        
        if (validarFormatoContrasena(contrasena) == 0) {
            enviarPC(sprintf(buffer, "CONTRASENA INVALIDA\n\r"));
            enviarPC(sprintf(buffer, "Por favor intentelo de nuevo.\n\r"));
            ThisThread::sleep_for(1s);
            continue; 
        }

        contrasenaValida = true;
        break; 
    }

    if (habitacion == 1) {
        strcpy(contrasenaHabitacion1, contrasena);
    } else if (habitacion == 2) {
        strcpy(contrasenaHabitacion2, contrasena);
    } else if (habitacion == 3) {
        strcpy(contrasenaHabitacion3, contrasena);
    }
}

void leerDesdeCelular(void) {
    int flagFin = 1, flagInicio = 1, indice = 0;             
    do { 
        limpiarContrasena();
        flagFin = 1;              
        if (celular.readable()) {
            do {
                if (celular.readable()) {
                    celular.read(&caracterRecibido, 1);
                    if (caracterRecibido == '\r') {
                        flagFin = 0;
                    } else if (caracterRecibido == '\n') {}
                    else {
                        contrasena[indice] = caracterRecibido;
                        flagInicio = 0;
                        indice++;
                    }
                } 
            } while (flagFin); 
        }
    } while (flagInicio);
}

void validarHabitacion(void) {
    int flag = 1, indice = 0; 
    do {    
        ledRojo = 1;
        ledVerde = 0;
        enviarCelular(sprintf(buffer, "Seleccione su habitacion:\n\r"));
        enviarCelular(sprintf(buffer, "301  302  303\n\r")); 
        leerDesdeCelular();    

        if (strcmp(contrasena, habitaciones[0]) == 0 || strcmp(contrasena, habitaciones[1]) == 0 || strcmp(contrasena, habitaciones[2]) == 0) {
            strcpy(habitacionSeleccionada, contrasena);
            enviarCelular(sprintf(buffer, "La habitacion es %s.\n\r", habitacionSeleccionada));
            enviarPC(sprintf(buffer, "\n\rAccediendo a la habitacion %s.\n\r", habitacionSeleccionada));
            flag = 0;
        } else {
            enviarCelular(sprintf(buffer, "La habitacion no existe.\n\r"));
            flag = 1;
        }
    } while (flag == 1);
}

void configuracionInicial(void) {
    enviarPC(sprintf(buffer, "             BIENVENIDO AL SISTEMA\n\r"));
    enviarCelular(sprintf(buffer, "BIENVENIDOS A SU HOTEL\n\r"));
    enviarCelular(sprintf(buffer, "     LAS DELICIAS\n\r"));
    enviarCelular(sprintf(buffer, "Por favor espere, estamos configurando...\n\r"));

    for (int i = 1; i < 4; i++) {
        int numeroHabitacion = 300 + i; 
        enviarPC(sprintf(buffer, "\n\rIngrese la contrasena para la habitacion %i\n\r", numeroHabitacion));
        leerDesdePC(i);
    }        

    enviarPC(sprintf(buffer, "\n\rContrasenas guardadas correctamente.\n\r"));
    enviarPC(sprintf(buffer, "%s", contrasenaHabitacion1));
    enviarPC(sprintf(buffer, "\n\r%s\n\r", contrasenaHabitacion2));
    enviarPC(sprintf(buffer, "%s\n\r", contrasenaHabitacion3));
    ThisThread::sleep_for(2s);
    enviarPC(sprintf(buffer, "Accediendo a las habitaciones...\n\r"));
}

uint8_t validarContrasena(void) {
    uint8_t resultadoHabitacion1 = 1, resultadoHabitacion2 = 1, resultadoHabitacion3 = 1;

    enviarCelular(sprintf(buffer, "Ingrese la contrasena %s\n\r", habitacionSeleccionada));
    leerDesdeCelular();

    if (strcmp(habitacionSeleccionada, habitaciones[0]) == 0) {
        resultadoHabitacion1 = strcmp(contrasena, contrasenaHabitacion1);
    }
    if (strcmp(habitacionSeleccionada, habitaciones[1]) == 0) {
        resultadoHabitacion2 = strcmp(contrasena, contrasenaHabitacion2);
    }
    if (strcmp(habitacionSeleccionada, habitaciones[2]) == 0) {
        resultadoHabitacion3 = strcmp(contrasena, contrasenaHabitacion3);
    }

    if (resultadoHabitacion1 == 0 || resultadoHabitacion2 == 0 || resultadoHabitacion3 == 0) {
        ledRojo = 0;
        ledVerde = 1; 
        enviarPC(sprintf(buffer, "Contrasena correcta.\n\r"));
        enviarCelular(sprintf(buffer, "Contrasena correcta.\n\r"));
        ThisThread::sleep_for(3s);  
        ledVerde = 0;
        return 1;
    } else {
        enviarPC(sprintf(buffer, "Contrasena incorrecta.\n\r"));
        enviarCelular(sprintf(buffer, "Contrasena incorrecta.\n\r"));
        ledRojo = 1; 
        return 0;
    }
}

void opcionSeleccionHabitacion(void) {
    char respuesta;
    enviarCelular(sprintf(buffer, "Desea otra habitacion? (S/N)\n\r"));
    enviarPC(sprintf(buffer, "\n\rPor favor espere estamos validando\n\r"));
    enviarPC(sprintf(buffer, "Si el huesped desea otra habitacion\n\r"));
    leerDesdeCelular();
    respuesta = contrasena[0];
    if (respuesta == 'S' || respuesta == 's') {
        enviarPC(sprintf(buffer, "\n\rSe esta seleccionando otra habitacion.\n\r"));
        enviarCelular(sprintf(buffer, "Por favor, seleccione otra habitacion.\n\r"));
    } else {
        enviarPC(sprintf(buffer, "\n\rGracias por usar el sistema.\n\r"));
        enviarCelular(sprintf(buffer, "\n\rQue disfrute su estadia.\n\r"));
        while (1);
    }
}
uint8_t validarFormatoContrasena(const char* contrasena) {
    int letras = 0, numeros = 0;
    char caracteresUsados[256] = {0};

    for (int i = 0; i < 4; i++) {
        if (isalpha(contrasena[i])) {
            letras++;
        } else if (isdigit(contrasena[i])) {
            numeros++;
        }
        // Verificar que no haya caracteres repetidos
        if (caracteresUsados[(unsigned char)contrasena[i]] > 0) {
            
            enviarPC(sprintf(buffer,"\n\rCaracter repetido, contraseña no válida.\n\r"));
            ThisThread::sleep_for(1s); 
            return 0;  
        }
        caracteresUsados[(unsigned char)contrasena[i]]++;
    }

    if (letras == 2 && numeros == 2) {
        return 1;  
    } else {
        enviarPC(sprintf(buffer,"\n\rContraseña debe tener 2 letras y 2 números.\n\r"));
        ThisThread::sleep_for(1s); 
        return 0;  
    }
}
