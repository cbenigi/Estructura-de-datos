#include <iostream>

using namespace std;

struct NodoMateria {
    string nombreMateria;
    float nota;
    NodoMateria* siguiente;
};

struct NodoEstudiante {
    string nombre;
    string codigo;
    NodoMateria* materias;
    NodoEstudiante* siguiente;
};

struct NodoCola {
    string nombre;
    string codigo;
    NodoCola* siguiente;
};

struct NodoPila {
    string nombre;
    string codigo;
    float promedio;
    NodoPila* siguiente;
};

class Cola {
private:
    NodoCola* frente;
    NodoCola* fondo;

public:
    Cola() {
        frente = nullptr;
        fondo = nullptr;
    }

    bool vacia() {
        return frente == nullptr;
    }

    void encolar(string nombre, string codigo) {
        NodoCola* nuevo = new NodoCola;
        nuevo->nombre = nombre;
        nuevo->codigo = codigo;
        nuevo->siguiente = nullptr;

        if (vacia()) {
            frente = nuevo;
        } else {
            fondo->siguiente = nuevo;
        }
        fondo = nuevo;
    }

    void desencolar(string& nombre, string& codigo) {
        if (!vacia()) {
            NodoCola* temp = frente;
            nombre = temp->nombre;
            codigo = temp->codigo;
            frente = frente->siguiente;
            if (frente == nullptr) {
                fondo = nullptr;
            }
            delete temp;
        }
    }
};

class Pila {
private:
    NodoPila* cima;

public:
    Pila() {
        cima = nullptr;
    }

    bool vacia() {
        return cima == nullptr;
    }

    void push(string nombre, string codigo, float promedio) {
        NodoPila* nuevo = new NodoPila;
        nuevo->nombre = nombre;
        nuevo->codigo = codigo;
        nuevo->promedio = promedio;
        nuevo->siguiente = cima;
        cima = nuevo;
    }

    void pop(string& nombre, string& codigo, float& promedio) {
        if (!vacia()) {
            NodoPila* temp = cima;
            nombre = temp->nombre;
            codigo = temp->codigo;
            promedio = temp->promedio;
            cima = cima->siguiente;
            delete temp;
        }
    }

    int contar() {
        int count = 0;
        NodoPila* temp = cima;
        while (temp != nullptr) {
            count++;
            temp = temp->siguiente;
        }
        return count;
    }
};

NodoEstudiante* listaEstudiantes = nullptr;
string* nombresMaterias = nullptr;
int M = 0;

void insertarMateria(NodoEstudiante* estudiante, string nombreMat, float nota) {
    NodoMateria* nuevo = new NodoMateria;
    nuevo->nombreMateria = nombreMat;
    nuevo->nota = nota;
    nuevo->siguiente = nullptr;
    
    if (estudiante->materias == nullptr) {
        estudiante->materias = nuevo;
    } else {
        NodoMateria* temp = estudiante->materias;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
}

void insertarEstudiante(string nombre, string codigo) {
    NodoEstudiante* nuevo = new NodoEstudiante;
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->materias = nullptr;
    nuevo->siguiente = nullptr;
    
    if (listaEstudiantes == nullptr) {
        listaEstudiantes = nuevo;
    } else {
        NodoEstudiante* temp = listaEstudiantes;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
}

NodoEstudiante* obtenerUltimoEstudiante() {
    if (listaEstudiantes == nullptr) return nullptr;
    
    NodoEstudiante* temp = listaEstudiantes;
    while (temp->siguiente != nullptr) {
        temp = temp->siguiente;
    }
    return temp;
}

float promedioEstudiante(NodoEstudiante* estudiante) {
    float suma = 0;
    int count = 0;
    NodoMateria* temp = estudiante->materias;
    
    while (temp != nullptr) {
        suma += temp->nota;
        count++;
        temp = temp->siguiente;
    }
    
    return (count > 0) ? suma / count : 0;
}

float promedioMateria(string nombreMat) {
    float suma = 0;
    int count = 0;
    NodoEstudiante* tempEst = listaEstudiantes;
    
    while (tempEst != nullptr) {
        NodoMateria* tempMat = tempEst->materias;
        while (tempMat != nullptr) {
            if (tempMat->nombreMateria == nombreMat) {
                suma += tempMat->nota;
                count++;
                break;
            }
            tempMat = tempMat->siguiente;
        }
        tempEst = tempEst->siguiente;
    }
    
    return (count > 0) ? suma / count : 0;
}

void mostrarEstudiantes() {
    cout << "\n--- Listado ---\n";
    NodoEstudiante* temp = listaEstudiantes;
    
    while (temp != nullptr) {
        cout << temp->nombre << " (" << temp->codigo << ")\n";
        
        NodoMateria* tempMat = temp->materias;
        while (tempMat != nullptr) {
            cout << "   " << tempMat->nombreMateria << ": " << tempMat->nota << "\n";
            tempMat = tempMat->siguiente;
        }
        
        cout << "   Promedio: " << promedioEstudiante(temp) << "\n";
        temp = temp->siguiente;
    }
}

void mostrarEstadisticas() {
    cout << "\n--- Estadisticas ---\n";
    for (int i = 0; i < M; i++) {
        cout << nombresMaterias[i] << " -> Promedio: " << promedioMateria(nombresMaterias[i]) << "\n";
    }
}

void procesarReprobados() {
    Pila pilaReprobados;
    NodoEstudiante* temp = listaEstudiantes;
    
    while (temp != nullptr) {
        float promedio = promedioEstudiante(temp);
        if (promedio < 3.0) {
            pilaReprobados.push(temp->nombre, temp->codigo, promedio);
        }
        temp = temp->siguiente;
    }
    
    int totalReprobados = pilaReprobados.contar();
    
    if (totalReprobados == 0) {
        cout << "\n--- Habilitaciones ---\n";
        cout << "No hay estudiantes reprobados.\n";
        return;
    }
    
    cout << "\n--- Habilitaciones (Estudiantes Reprobados) ---\n";
    cout << "Total de estudiantes reprobados: " << totalReprobados << "\n";
    cout << "Procesando en orden de habilitacion:\n\n";
    
    int posicion = 1;
    while (!pilaReprobados.vacia()) {
        string nombre, codigo;
        float promedio;
        pilaReprobados.pop(nombre, codigo, promedio);
        cout << posicion << ". " << nombre << " (" << codigo << ") - Promedio: " << promedio << "\n";
        posicion++;
    }
}

void liberarMemoria() {
    NodoEstudiante* tempEst = listaEstudiantes;
    
    while (tempEst != nullptr) {
        NodoMateria* tempMat = tempEst->materias;
        while (tempMat != nullptr) {
            NodoMateria* borrarMat = tempMat;
            tempMat = tempMat->siguiente;
            delete borrarMat;
        }
        
        NodoEstudiante* borrarEst = tempEst;
        tempEst = tempEst->siguiente;
        delete borrarEst;
    }
    
    delete[] nombresMaterias;
}

int main() {
    int N;
    Cola colaInscripciones;
    
    cout << "=== Sistema Academico con Cola y Pila ===\n";
    cout << "Numero de estudiantes: ";
    cin >> N;
    cout << "Numero de materias: ";
    cin >> M;

    cin.ignore();

    nombresMaterias = new string[M];
    
    cout << "\n--- Ingreso de materias ---\n";
    for (int j = 0; j < M; j++) {
        cout << "Materia " << j + 1 << ": ";
        getline(cin, nombresMaterias[j]);
    }

    cout << "\n--- Cola de Inscripciones ---\n";
    cout << "Los estudiantes llegan y se encolan...\n\n";
    
    for (int i = 0; i < N; i++) {
        string nombre, codigo;
        
        cout << "Estudiante " << i + 1 << " llega:\n";
        cout << "  Nombre: ";
        getline(cin, nombre);
        cout << "  Codigo: ";
        getline(cin, codigo);
        
        colaInscripciones.encolar(nombre, codigo);
        cout << "  [Encolado exitosamente]\n\n";
    }

    cout << "\n--- Procesando Inscripciones (Desencolando) ---\n";
    
    while (!colaInscripciones.vacia()) {
        string nombre, codigo;
        colaInscripciones.desencolar(nombre, codigo);
        
        cout << "\nAtendiendo a: " << nombre << " (" << codigo << ")\n";
        
        insertarEstudiante(nombre, codigo);
        NodoEstudiante* estudianteActual = obtenerUltimoEstudiante();

        for (int j = 0; j < M; j++) {
            float nota;
            do {
                cout << "  Nota en " << nombresMaterias[j] << " (0-5): ";
                cin >> nota;
                if (nota < 0 || nota > 5) {
                    cout << "  Error: Ingrese una nota entre 0 y 5.\n";
                }
            } while (nota < 0 || nota > 5);
            insertarMateria(estudianteActual, nombresMaterias[j], nota);
        }
        cin.ignore();
        
        cout << "  [Inscripcion completada]\n";
    }

    mostrarEstudiantes();
    mostrarEstadisticas();
    procesarReprobados();
    liberarMemoria();

    return 0;
}
