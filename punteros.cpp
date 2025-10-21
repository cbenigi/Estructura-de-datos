#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    unsigned char edad = 25; 
    int suma = 1500;         
    float estatura = 1.75;  
    char inicialNombre = 'a'; 
    long numeroIdentificacion = 1018236959; 

    cout <<"Variables y Punteros"<< endl;
    cout << "  (Las direcciones de memoria se muestran en formato hexadecimal)" << endl;
    cout << endl;

    unsigned char *pEdad = &edad;
    int *pSuma = &suma;
    float *pEstatura = &estatura;
    char *pInicialNombre = &inicialNombre;
    long *pNumeroIdentificacion = &numeroIdentificacion;

    cout << hex << uppercase;

    cout << "### Información Inicial de Variables y Punteros ###" << endl;

    cout << "\n[TIPO: byte (unsigned char)]" << endl;
    cout << dec << "Valor de edad: " << (int)edad << endl;
    cout << "Dirección de edad: " << (void*)&edad << endl;
    cout << "Dirección almacenada en pEdad: " << (void*)pEdad << endl;
    cout << dec << "Valor apuntado por pEdad (*pEdad): " << (int)*pEdad << endl;

    cout << "\n[TIPO: int]" << endl;
    cout << dec << "Valor de suma: " << suma << endl;
    cout << "Dirección de suma: " << &suma << endl;
    cout << "Dirección almacenada en pSuma: " << pSuma << endl;
    cout << dec << "Valor apuntado por pSuma (*pSuma): " << *pSuma << endl;

    cout << "\n[TIPO: float]" << endl;
    cout << dec << fixed << setprecision(2) << "Valor de estatura: " << estatura << endl;
    cout << hex << "Dirección de estatura: " << &estatura << endl;
    cout << "Dirección almacenada en pEstatura: " << pEstatura << endl;
    cout << dec << "Valor apuntado por pEstatura (*pEstatura): " << *pEstatura << endl;

    cout << "\n[TIPO: char]" << endl;
    cout << dec << "Valor de inicialNombre: " << inicialNombre << endl;
    cout << hex << "Dirección de inicialNombre: " << (void*)&inicialNombre << endl;
    cout << "Dirección almacenada en pInicialNombre: " << (void*)pInicialNombre << endl;
    cout << dec << "Valor apuntado por pInicialNombre (*pInicialNombre): " << *pInicialNombre << endl;

    cout << "\n[TIPO: long]" << endl;
    cout << dec << "Valor de numeroIdentificacion: " << numeroIdentificacion << endl;
    cout << hex << "Dirección de numeroIdentificacion: " << &numeroIdentificacion << endl;
    cout << "Dirección almacenada en pNumeroIdentificacion: " << pNumeroIdentificacion << endl;
    cout << dec << "Valor apuntado por pNumeroIdentificacion (*pNumeroIdentificacion): " << *pNumeroIdentificacion << endl;

    cout << endl;
    cout << "--- Ejemplo de Cambio con Puntero ---" << endl;
    cout << dec << "Valor original de 'edad': " << (int)edad << endl;

    *pEdad = 30;

    cout << "Nuevo valor de 'edad' después de *pEdad = 30: " << (int)edad << endl;
    cout << "Valor apuntado por 'pEdad' ahora (*pEdad): " << (int)*pEdad << endl;

    cout << "\nFin del programa." << endl;

    return 0;
}

