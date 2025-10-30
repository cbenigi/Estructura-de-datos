#include <iostream>
#include <cctype>
#include <string>
using namespace std;

// estructura para materias de un estudiante
struct MateriaNodo {
    string nombre;
    float nota;
    MateriaNodo* siguiente;
};

// estructura base para lista enlazada, pila y cola
struct Estudiante {
    string nombre;
    string codigo;
    string concepto; // ahora puede ser usado como "nota" o "concepto" según el caso
    MateriaNodo* materias; // lista de materias con notas
    Estudiante* siguiente;
};

// materias y pesos globales
const string MATERIAS[] = {
    "Matemáticas",
    "Física",
    "Programación",
    "Estructuras de Datos",
    "Bases de Datos"
};
const float PESOS[] = { 0.25f, 0.20f, 0.20f, 0.20f, 0.15f }; // suma 1.0
const int NUM_MATERIAS = sizeof(MATERIAS) / sizeof(MATERIAS[0]);

// ---------- VALIDACIONES ----------

// valida que una cadena contenga solo números
bool esNumero(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();
}

// valida que una cadena contenga solo letras y espacios
bool esTexto(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return !str.empty();
}

// valida que la nota esté entre 0 y 5
float pedirNotaValida() {
    float nota;
    do {
        cout << "Nota (0 a 5): ";
        cin >> nota;
        if (cin.fail() || nota < 0 || nota > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Ingrese una nota válida entre 0 y 5.\n";
        } else {
            break;
        }
    } while (true);
    return nota;
}

// pide un código numérico
string pedirCodigoValido() {
    string codigo;
    do {
        cout << "Código (solo números): ";
        cin >> codigo;
        if (!esNumero(codigo)) {
            cout << " Solo se permiten números. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return codigo;
}

// pide un nombre válido (solo letras)
string pedirNombreValido() {
    string nombre;
    do {
        cout << "Nombre (solo letras): ";
        cin.ignore();
        getline(cin, nombre);
        if (!esTexto(nombre)) {
            cout << "  Solo se permiten letras. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return nombre;
}

// ---------- FUNCIONES DE LISTA ENLAZADA ----------

void registrarEstudiante(Estudiante*& cabeza, string nombre, string codigo) {
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = "";
    nuevo->materias = nullptr;
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

void mostrarRegistro(Estudiante* cabeza) {
    cout << "\n====== LISTADO GENERAL DE ESTUDIANTES ======\n";
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }
    Estudiante* actual = cabeza;
    while (actual != nullptr) {
        cout << "Nombre: " << actual->nombre
             << " | Codigo: " << actual->codigo
             << " | Materia: " << (actual->concepto.empty() ? string("(sin asignar)") : actual->concepto) << endl;
        actual = actual->siguiente;
    }
}

// ---------- FUNCIONES DE COLA (FIFO) ----------

void registrarSolicitud(Estudiante*& frente, Estudiante*& final, string nombre, string codigo, string concepto) {
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = concepto;
    nuevo->siguiente = nullptr;

    if (final == nullptr) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
}

void actualizarMateriaPorCodigo(Estudiante* cabeza, const string& codigo, const string& materia, float nota) {
    Estudiante* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigo == codigo) {
            // guardar "última materia" visible en concepto
            actual->concepto = materia;
            // buscar o agregar materia en la lista de materias
            MateriaNodo* m = actual->materias;
            while (m != nullptr) {
                if (m->nombre == materia) {
                    m->nota = nota;
                    return;
                }
                m = m->siguiente;
            }
            MateriaNodo* nuevo = new MateriaNodo{ materia, nota, actual->materias };
            actual->materias = nuevo;
            return;
        }
        actual = actual->siguiente;
    }
}

void atenderSolicitud(Estudiante*& frente, Estudiante*& final, Estudiante* lista) {
    if (frente == nullptr) {
        cout << "No hay solicitudes pendientes.\n";
        return;
    }

    Estudiante* temp = frente;
    cout << "\n====== REGISTRO DE MATERIAS ======\n";
    cout << "Estudiante: " << temp->nombre
         << " | Codigo: " << temp->codigo << endl;

    int opcionMateria = 0;
    do {
        cout << "Seleccione la materia a registrar:\n";
        for (int i = 0; i < NUM_MATERIAS; ++i) {
            cout << (i + 1) << ". " << MATERIAS[i] << '\n';
        }
        cout << "Opción: ";
        cin >> opcionMateria;
        if (cin.fail() || opcionMateria < 1 || opcionMateria > NUM_MATERIAS) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Opción inválida. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);

    string materiaSeleccionada = MATERIAS[opcionMateria - 1];
    float nota = pedirNotaValida();
    actualizarMateriaPorCodigo(lista, temp->codigo, materiaSeleccionada, nota);
    cout << "Materia registrada: " << materiaSeleccionada << " | Nota: " << nota << "\n";
    frente = frente->siguiente;

    if (frente == nullptr)
        final = nullptr;

    delete temp;
}

void mostrarSolicitudes(Estudiante* frente) {
    cout << "\n====== SOLICITUDES PENDIENTES ======\n";
    if (frente == nullptr) {
        cout << "No hay solicitudes en espera.\n";
        return;
    }
    Estudiante* actual = frente;
    while (actual != nullptr) {
        cout << "Nombre: " << actual->nombre
             << " | Codigo: " << actual->codigo
             << " | Concepto: " << actual->concepto << endl;
        actual = actual->siguiente;
    }
}

// ---------- FUNCIONES DE PILA (LIFO) ----------

void registrarHistorial(Estudiante*& cima, string nombre, string codigo, string concepto) {
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = concepto;
    nuevo->siguiente = cima;
    cima = nuevo;
}

void eliminarUltimoHistorial(Estudiante*& cima) {
    if (cima == nullptr) {
        cout << "El historial está vacío.\n";
        return;
    }

    Estudiante* temp = cima;
    cout << "\n====== ELIMINANDO DEL HISTORIAL ======\n";
    cout << "Estudiante: " << temp->nombre
         << " | Codigo: " << temp->codigo << endl;
    cima = cima->siguiente;
    delete temp;
}

void mostrarHistorial(Estudiante* cima) {
    cout << "\n====== HISTORIAL DE ACCIONES ======\n";
    if (cima == nullptr) {
        cout << "No hay registros en el historial.\n";
        return;
    }
    Estudiante* actual = cima;
    while (actual != nullptr) {
        cout << "Nombre: " << actual->nombre
             << " | Codigo: " << actual->codigo
             << " | Concepto: " << actual->concepto << endl;
        actual = actual->siguiente;
    }
}

// ---------- MENÚ PRINCIPAL ----------

float calcularPromedioPonderado(Estudiante* lista, const string& codigo) {
    Estudiante* actual = lista;
    while (actual != nullptr && actual->codigo != codigo) {
        actual = actual->siguiente;
    }
    if (actual == nullptr) {
        cout << "No se encontró el estudiante.\n";
        return -1.0f;
    }

    float sumaPonderada = 0.0f;
    float sumaPesos = 0.0f;
    for (int i = 0; i < NUM_MATERIAS; ++i) {
        // buscar si el estudiante tiene la materia i
        MateriaNodo* m = actual->materias;
        while (m != nullptr) {
            if (m->nombre == MATERIAS[i]) {
                sumaPonderada += m->nota * PESOS[i];
                sumaPesos += PESOS[i];
                break;
            }
            m = m->siguiente;
        }
    }
    if (sumaPesos == 0.0f) {
        cout << "El estudiante no tiene materias registradas.\n";
        return -1.0f;
    }
    // Si faltan materias, normalizamos respecto a los pesos presentes
    float promedio = sumaPonderada / sumaPesos;
    return promedio;
}

int main() {
    Estudiante* lista = nullptr;
    Estudiante* frente = nullptr;
    Estudiante* final = nullptr;
    Estudiante* cima = nullptr;

    int opcion;
    do {
        cout << "\n----- SISTEMA DE GESTIÓN ACADÉMICA -----\n";
        cout << "1. Registrar nuevo estudiante (se encola para materias)\n";
        cout << "2. Mostrar todos los estudiantes\n";
        cout << "3. Ver estudiantes en cola para registrar materias\n";
        cout << "4. Atender registro de materias\n";
        cout << "5. Calcular promedio por estudiante\n";
        cout << "6. Registrar acción en historial\n";
        cout << "7. Eliminar último registro del historial\n";
        cout << "8. Ver historial de acciones\n";
        cout << "0. Salir del sistema\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        string nombre, codigo, concepto;

        switch (opcion) {
            case 1:
                nombre = pedirNombreValido();
                codigo = pedirCodigoValido();
                registrarEstudiante(lista, nombre, codigo);
                registrarSolicitud(frente, final, nombre, codigo, "pendiente");
                break;

            case 2:
                mostrarRegistro(lista);
                break;

            case 3:
                mostrarSolicitudes(frente);
                break;

            case 4:
                atenderSolicitud(frente, final, lista);
                break;

            case 5:
                codigo = pedirCodigoValido();
                {
                    float prom = calcularPromedioPonderado(lista, codigo);
                    if (prom >= 0.0f) {
                        cout << "Promedio ponderado: " << prom << "\n";
                    }
                }
                break;

            case 6:
                nombre = pedirNombreValido();
                codigo = pedirCodigoValido();
                cin.ignore();
                cout << "Concepto: ";
                getline(cin, concepto);
                registrarHistorial(cima, nombre, codigo, concepto);
                break;

            case 7:
                eliminarUltimoHistorial(cima);
                break;

            case 8:
                mostrarHistorial(cima);
                break;

            case 0:
                cout << "Saliendo del sistema académico...\n";
                break;

            default:
                cout << "Opción no válida.\n";
        }

    } while (opcion != 0);

    return 0;
}
