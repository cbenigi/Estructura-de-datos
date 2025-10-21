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
class Pila {
private:
    Nodo<T> *cima; 

public:
    Pila() {
        cima = NULL; 
    }

    bool vacia() const {
        return cima == NULL;
    }

    void push(T valor) {
        Nodo<T> *nuevo_nodo = new Nodo<T>();
        nuevo_nodo->dato = valor;

        nuevo_nodo->siguiente = cima;

        cima = nuevo_nodo;
        cout<<" [PILA] -> Elemento "<<valor<<" apilado correctamente.\n";
    }

    T pop() {
        if (vacia()) {
            cerr << " [PILA] Error de POP: La pila está vacía.\n";
            
            throw runtime_error("Pila vacía");
        }

        T valor_sacado = cima->dato; 
        Nodo<T> *aux = cima;     
     
        cima = cima->siguiente;

        delete aux;
        cout<<" [PILA] -> Elemento "<<valor_sacado<<" desapilado.\n";
        return valor_sacado;
    }

    T top() const {
        if (vacia()) {
            cerr << " [PILA] Error de TOP: La pila está vacía, no hay elemento en la cima.\n";
            throw runtime_error("Pila vacía");
        }
        return cima->dato;
    }
};

void menuPila() {
 
    Pila<int> miPila;
    int opcion, valor;

    do {
        cout<<"\n====================================\n";
        cout<<"         MENÚ DE OPERACIONES - PILA\n";
        cout<<"====================================\n";
        cout<<"1. PUSH (Apilar/Insertar elemento)\n";
        cout<<"2. POP (Desapilar/Eliminar elemento de la cima)\n";
        cout<<"3. TOP (Obtener valor de la cima)\n";
        cout<<"4. Comprobar si esta vacía\n";
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
                    cout<<"  -> Ingrese el valor a apilar: ";
                    cin >> valor;
                    miPila.push(valor);
                    break;
                case 2:
                    if (!miPila.vacia()) {
                        miPila.pop();
                    } else {
                        cout<<"  -> La pila está vacía. Operación POP no realizada.\n";
                    }
                    break;
                case 3:
                    if (!miPila.vacia()) {
                        cout<<"  -> El valor en la CIMA es: " << miPila.top() << "\n";
                    } else {
                        cout<<"  -> La pila está vacía. Operación TOP no realizada.\n";
                    }
                    break;
                case 4:
                    if (miPila.vacia()) {
                        cout<<"  -> Resultado: La pila está VACÍA.\n";
                    } else {
                        cout<<"  -> Resultado: La pila NO está vacía.\n";
                    }
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
    menuPila();
    
    return 0;
}

