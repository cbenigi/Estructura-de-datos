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
    
    cout << "=== Sistema Academico Basico ===\n";
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

    cout << "\n--- Ingreso de estudiantes ---\n";
    for (int i = 0; i < N; i++) {
        string nombre, codigo;
        
        cout << "Nombre estudiante " << i + 1 << ": ";
        getline(cin, nombre);
        cout << "Codigo: ";
        getline(cin, codigo);

        insertarEstudiante(nombre, codigo);
        
        NodoEstudiante* temp = listaEstudiantes;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        for (int j = 0; j < M; j++) {
            float nota;
            cout << "Nota en " << nombresMaterias[j] << ": ";
            cin >> nota;
            insertarMateria(temp, nombresMaterias[j], nota);
        }
        cin.ignore();
    }

    mostrarEstudiantes();
    mostrarEstadisticas();
    liberarMemoria();

    return 0;
}
