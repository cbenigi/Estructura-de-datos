#include <iostream>
#include <cstdlib> 
#include <limits>  

using namespace std;

template <class T>
struct Nodo {
    T dato;             
    Nodo<T> *siguiente;   
};

template <class T>
class Cola {
private:
    Nodo<T> *frente;
    Nodo<T> *fondo;  

public:
    Cola() {
        frente = NULL; 
        fondo = NULL; 
    }

    bool vacia() const {
        return frente == NULL;
    }

    void encolar(T valor) {
        Nodo<T> *nuevo_nodo = new Nodo<T>();
        nuevo_nodo->dato = valor;
        nuevo_nodo->siguiente = NULL; 

        if (vacia()) {
            frente = nuevo_nodo;
        } else {
            fondo->siguiente = nuevo_nodo;
        }
        
        fondo = nuevo_nodo;
        cout<<" [COLA] -> Elemento "<<valor<<" encolado correctamente en el fondo.\n";
    }

    T desencolar() {
        if (vacia()) {
            cerr << " [COLA] Error de DESENCOLAR: La cola está vacía.\n";
            throw runtime_error("Cola vacía");
        }

        T valor_sacado = frente->dato; 
        Nodo<T> *aux = frente; 
        frente = frente->siguiente;

        if (frente == NULL) {
             fondo = NULL;
        }

        delete aux;
        cout<<" [COLA] -> Elemento "<<valor_sacado<<" desencolado del frente.\n";
        return valor_sacado;
    }
    
    T frenteValor() const {
        if (vacia()) {
            cerr << " [COLA] Error de FRENTE: La cola está vacía, no hay elemento en el frente.\n";
            throw runtime_error("Cola vacía");
        }
        return frente->dato;
    }
};

void menuCola() {
    Cola<int> miCola;
    int opcion, valor;

    do {
        cout<<"\n====================================\n";
        cout<<"         OPERACIONES - COLA\n";
        cout<<"====================================\n";
        cout<<"1. ENCOLAR (Insertar elemento al fondo)\n";
        cout<<"2. DESENCOLAR (Eliminar elemento del frente)\n";
        cout<<"3. FRENTE (Obtener valor del frente)\n";
        cout<<"4. Comprobar si está vacía\n";
        cout<<"5. Salir del Menú Cola\n";
        cout<<"Ingrese su opción: ";
        
        if (!(cin >> opcion)) {
            cout<<"Entrada inválida. Limpiando y reintentando.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        try {
            switch (opcion) {
                case 1:
                    cout<<"  -> Ingrese el valor a encolar: ";
                    cin >> valor;
                    miCola.encolar(valor);
                    break;
                case 2:
                    if (!miCola.vacia()) {
                        miCola.desencolar();
                    } else {
                        cout<<"  -> La cola está vacía. Operación DESENCOLAR no realizada.\n";
                    }
                    break;
                case 3:
                    if (!miCola.vacia()) {
                        cout<<"  -> El valor en el FRENTE es: " << miCola.frenteValor() << "\n";
                    } else {
                        cout<<"  -> La cola está vacía. Operación FRENTE no realizada.\n";
                    }
                    break;
                case 4:
                    if (miCola.vacia()) {
                        cout<<"  -> Resultado: La cola está VACÍA.\n";
                    } else {
                        cout<<"  -> Resultado: La cola NO está vacía.\n";
                    }
                    break;
                case 5:
                    cout<<"Saliendo del menú COLA.\n";
                    break;
                default:
                    cout<<"Opción no válida. Intente de nuevo.\n";
            }
        } catch (const exception& e) {
            cerr << "Se produjo un error: " << e.what() << '\n';
        }
    } while (opcion != 5);
}

int main() {
    int opcion;
    menuCola(); 
    return 0;
}

