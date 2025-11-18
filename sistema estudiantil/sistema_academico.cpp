#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iomanip> // Necesario para setprecision

using namespace std;

// Función para limpiar la consola
void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Convierte un float a string controlando la precisión decimal a 1.
 * @param num El número flotante a convertir.
 * @return El número como string con un decimal.
 */
string floatAString(float num) {
    stringstream ss;
    ss << fixed << setprecision(1) << num;
    return ss.str();
}

// estructura base para lista enlazada, pila y cola
struct Estudiante {
    string nombre;
    string codigo;
    // En la Lista principal, 'concepto' guarda la lista de NOTAS (ej: "4.5,3.0").
    // En la Cola, 'concepto' guarda la NOTA PENDIENTE (una sola).
    // En la Pila, 'concepto' guarda la ACCIÓN del historial.
    string concepto; 
    Estudiante* siguiente;
};

// ---------- VALIDACIONES Y UTILIDADES (Cuerpos omitidos por brevedad, pero incluidos en el código) ----------

// Las funciones de validación 'esNumero', 'esTexto', 'pedirNotaValida', 'pedirCodigoValido', 'pedirNombreValido', 
// 'codigoExiste', 'pedirConfirmacion' y 'pedirOpcionValida' se mantienen sin cambios funcionales mayores, 
// solo se ajustan los mensajes para quitar emojis.

bool esNumero(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();
}

bool esTexto(const string& str) {
    bool soloTexto = true;
    for (char c : str) {
        if (!isalpha(c) && c != ' ') {
            soloTexto = false;
            break;
        }
    }
    return soloTexto && !str.empty();
}

float pedirNotaValida() {
    float nota;
    do {
        cout << " Nota (0 a 5): ";
        cin >> nota;
        if (cin.fail() || nota < 0 || nota > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [ERROR] Ingrese una nota válida entre 0 y 5.\n";
        } else {
            cin.ignore(1000, '\n');
            break;
        }
    } while (true);
    return nota;
}

string pedirCodigoValido() {
    string codigo;
    do {
        cout << " Código (solo números): ";
        getline(cin, codigo);
        if (!esNumero(codigo)) {
            cout << "  [ERROR] Solo se permiten números. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return codigo;
}

string pedirNombreValido() {
    string nombre;
    do {
        cout << " Nombre (solo letras): ";
        getline(cin, nombre);
        if (!esTexto(nombre)) {
            cout << "  [ERROR] Solo se permiten letras y espacios. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return nombre;
}

bool codigoExiste(Estudiante* cabeza, const string& codigo) {
    Estudiante* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigo == codigo) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool pedirConfirmacion(const string& accion) {
    char respuesta;
    cout << "\n [CONFIRMACIÓN] ¿Está seguro de " << accion << "? (S/N): ";
    cin >> respuesta;
    cin.ignore(1000, '\n');
    return (respuesta == 'S' || respuesta == 's');
}

int pedirOpcionValida(int minOp, int maxOp) {
    int opcion;
    while (!(cin >> opcion) || opcion < minOp || opcion > maxOp) {
        cout << "  [ERROR] Opción no válida. Ingrese un número entre " << minOp << " y " << maxOp << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return opcion;
}

// ---------- FUNCIONES DE LISTA ENLAZADA (Registro Inicial) ----------

void registrarEstudiante(Estudiante*& cabeza, string nombre, string codigo) {
    if (codigoExiste(cabeza, codigo)) {
        cout << "  [ERROR] Ya existe un estudiante con el código " << codigo << ". No se registró.\n";
        return;
    }
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = ""; // Inicializar sin notas.
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    cout << "  [ÉXITO] Estudiante " << nombre << " registrado con éxito (Sin notas iniciales).\n";
}

Estudiante* buscarEstudiante(Estudiante* cabeza, const string& codigo) {
    Estudiante* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigo == codigo) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void mostrarRegistro(Estudiante* cabeza) {
    cout << "\n====== LISTADO GENERAL DE ESTUDIANTES ======\n";
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }
    Estudiante* actual = cabeza;
    while (actual != nullptr) {
        // Muestra las notas separadas por comas. Si está vacío, muestra "N/A"
        string notas = actual->concepto.empty() ? "N/A" : actual->concepto;
        cout << "Nombre: " << actual->nombre
             << " | Código: " << actual->codigo
             << " | Notas: " << notas << endl;
        actual = actual->siguiente;
    }
}

/**
 * @brief Calcula el promedio de una cadena de notas separadas por comas.
 */
float calcularPromedio(const string& notasStr) {
    if (notasStr.empty()) return 0.0f;

    stringstream ss(notasStr);
    string segmento;
    float suma = 0.0f;
    int contador = 0;

    // Recorre la cadena segmentando por coma
    while (getline(ss, segmento, ',')) {
        try {
            suma += stof(segmento);
            contador++;
        } catch (...) {
            // Ignora segmentos no válidos
        }
    }

    if (contador == 0) return 0.0f;

    return suma / contador;
}

void buscarEstudianteYMostrarPromedio(Estudiante* cabeza) {
    string codigoBuscado;
    cout << "\n====== BÚSQUEDA DE ESTUDIANTE Y PROMEDIO ======\n";
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados para buscar.\n";
        return;
    }
    
    cout << "Ingrese el Código del estudiante a buscar: ";
    getline(cin, codigoBuscado);
    if (!esNumero(codigoBuscado)) {
        cout << "  [ERROR] Código inválido. Solo se permiten números.\n";
        return;
    }

    Estudiante* encontrado = buscarEstudiante(cabeza, codigoBuscado);
    if (encontrado != nullptr) {
        string notasStr = encontrado->concepto;
        string notasDisplay = notasStr.empty() ? "N/A" : notasStr;
        float promedio = calcularPromedio(notasStr);
        string promedioStr = floatAString(promedio);

        cout << "\n  Estudiante Encontrado!\n";
        cout << "  Nombre: " << encontrado->nombre << endl;
        cout << "  Código: " << encontrado->codigo << endl;
        cout << "  Notas Registradas: " << notasDisplay << endl;
        cout << "  Promedio Exclusivo: " << promedioStr << endl; 
    } else {
        cout << "  [ERROR] No se encontró ningún estudiante con el código " << codigoBuscado << ".\n";
    }
}


// ---------- FUNCIONES DE COLA (FIFO - REGISTRO DE NOTAS PENDIENTES) ----------

void registrarNotaPendiente(Estudiante*& frente, Estudiante*& final, Estudiante* lista, string codigo, string notaStr) {
    Estudiante* estudianteExistente = buscarEstudiante(lista, codigo);
    if (estudianteExistente == nullptr) {
        cout << "  [ERROR] El estudiante con código " << codigo << " no existe en el registro principal.\n";
        return;
    }

    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = estudianteExistente->nombre; 
    nuevo->codigo = codigo;
    nuevo->concepto = notaStr; // Una sola nota
    nuevo->siguiente = nullptr;

    if (final == nullptr) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    cout << "  [ÉXITO] Nota " << notaStr << " registrada en la cola para el código " << codigo << ".\n";
}

void procesarNota(Estudiante*& frente, Estudiante*& final, Estudiante* lista) {
    if (frente == nullptr) {
        cout << "  [ADVERTENCIA] No hay notas pendientes para procesar.\n";
        return;
    }
    
    if (!pedirConfirmacion("procesar la siguiente nota")) {
        cout << "  Proceso de nota cancelado.\n";
        return;
    }

    Estudiante* temp = frente;
    Estudiante* estudianteLista = buscarEstudiante(lista, temp->codigo);
    string nuevaNotaStr = temp->concepto;

    cout << "\n====== PROCESANDO NOTA (FIFO) ======\n";
    cout << "  Estudiante: " << temp->nombre
         << " | Código: " << temp->codigo
         << " | Nota a Añadir: " << nuevaNotaStr << endl;
    
    if (estudianteLista != nullptr) {
        // AGREGAR LA NUEVA NOTA A LA LISTA EXISTENTE SEPARADA POR COMA
        if (estudianteLista->concepto.empty()) {
            estudianteLista->concepto = nuevaNotaStr;
        } else {
            estudianteLista->concepto += "," + nuevaNotaStr;
        }
        cout << "  [ÉXITO] Nota actualizada en el registro principal. Notas: " << estudianteLista->concepto << "\n";
    } else {
        cout << "  [ERROR] Error crítico: Estudiante no encontrado en la lista principal (Nota no actualizada).\n";
    }

    // Eliminar el nodo de la cola
    frente = frente->siguiente;
    if (frente == nullptr)
        final = nullptr;

    delete temp;
    cout << "  [ÉXITO] Nota procesada y eliminada de la cola.\n";
}

void mostrarNotasPendientes(Estudiante* frente) {
    cout << "\n====== NOTAS PENDIENTES EN COLA (FIFO) ======\n";
    if (frente == nullptr) {
        cout << "No hay notas pendientes en espera.\n";
        return;
    }
    Estudiante* actual = frente;
    int i = 1;
    while (actual != nullptr) {
        cout << "  " << i++ << ". Código: " << actual->codigo
             << " | Nombre: " << actual->nombre
             << " | Nota a Asignar: " << actual->concepto << endl;
        actual = actual->siguiente;
    }
}

// ---------- FUNCIONES DE PILA (LIFO - Historial) ----------

void registrarHistorial(Estudiante*& cima, string nombre, string codigo, string concepto) {
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = concepto;
    nuevo->siguiente = cima;
    cima = nuevo;
    cout << "  [ÉXITO] Acción registrada en el historial (Pila).\n";
}

void eliminarUltimoHistorial(Estudiante*& cima) {
    if (cima == nullptr) {
        cout << "  [ADVERTENCIA] El historial está vacío.\n";
        return;
    }

    if (!pedirConfirmacion("eliminar el último registro del historial")) {
        cout << "  Eliminación cancelada.\n";
        return;
    }

    Estudiante* temp = cima;
    cout << "\n====== ELIMINANDO DEL HISTORIAL (LIFO) ======\n";
    cout << "  Registro Eliminado: Nombre: " << temp->nombre
         << " | Código: " << temp->codigo
         << " | Acción: " << temp->concepto << endl;
    cima = cima->siguiente;
    delete temp;
    cout << "  [ÉXITO] Registro eliminado con éxito.\n";
}

void mostrarHistorial(Estudiante* cima) {
    cout << "\n====== HISTORIAL DE ACCIONES (LIFO) ======\n";
    if (cima == nullptr) {
        cout << "No hay registros en el historial.\n";
        return;
    }
    Estudiante* actual = cima;
    while (actual != nullptr) {
        cout << "  Acción: " << actual->concepto
             << " | Nombre: " << actual->nombre
             << " | Código: " << actual->codigo << endl;
        actual = actual->siguiente;
    }
}

// ---------- MENÚ PRINCIPAL ----------

int main() {
    Estudiante* lista = nullptr;
    Estudiante* frente = nullptr;
    Estudiante* final = nullptr;
    Estudiante* cima = nullptr;

    int opcion;
    string nombre, codigo, concepto, notaStr;
    float nota;

    do {
        limpiarConsola(); 
        
        cout << "\n----- SISTEMA DE GESTIÓN ACADÉMICA (ESTRUCTURAS DE DATOS) -----\n";
        cout << "  [ LISTA ENLAZADA - REGISTRO DE ESTUDIANTES ]\n";
        cout << "  1. Registrar nuevo estudiante (Inicia sin notas)\n";
        cout << "  2. Mostrar todos los estudiantes\n";
        cout << "  9. Buscar estudiante y ver notas/promedio exclusivo\n";
        cout << "  [ COLA (FIFO) - ASIGNACIÓN DE NOTAS PENDIENTES ]\n";
        cout << "  3. Registrar Nota Pendiente para un estudiante\n";
        cout << "  4. Procesar la siguiente nota pendiente (Asignar)\n";
        cout << "  5. Ver notas pendientes en espera\n";
        cout << "  [ PILA (LIFO) - HISTORIAL DE ACCIONES ]\n";
        cout << "  6. Registrar acción en historial\n";
        cout << "  7. Eliminar último registro del historial\n";
        cout << "  8. Ver historial de acciones\n";
        cout << "\n  0. Salir del sistema\n";
        cout << "Seleccione una opción: ";

        opcion = pedirOpcionValida(0, 9);

        limpiarConsola(); 

        switch (opcion) {
            case 1: 
                cout << "\n-- REGISTRAR ESTUDIANTE --\n";
                nombre = pedirNombreValido();
                codigo = pedirCodigoValido();
                registrarEstudiante(lista, nombre, codigo);
                break;

            case 2:
                mostrarRegistro(lista);
                break;
                
            case 9:
                buscarEstudianteYMostrarPromedio(lista);
                break;

            case 3: // Registrar Nota Pendiente (Cola - FIFO)
                cout << "\n-- REGISTRAR NOTA PENDIENTE (COLA) --\n";
                cout << "Ingrese el código y la nota a registrar:\n";
                codigo = pedirCodigoValido();
                nota = pedirNotaValida();
                // Usar la función de formato para asegurar un decimal
                notaStr = floatAString(nota); 
                registrarNotaPendiente(frente, final, lista, codigo, notaStr);
                break;

            case 4: // Procesar Nota Pendiente (Cola - FIFO)
                procesarNota(frente, final, lista);
                break;

            case 5: // Mostrar Notas Pendientes (Cola - FIFO)
                mostrarNotasPendientes(frente);
                break;

            case 6: // Registrar en historial (Pila)
                cout << "\n-- REGISTRAR ACCIÓN EN HISTORIAL --\n";
                nombre = pedirNombreValido();
                codigo = pedirCodigoValido();
                cout << " Descripción de la Acción: ";
                getline(cin, concepto); 
                registrarHistorial(cima, nombre, codigo, concepto);
                break;

            case 7: // Eliminar último historial (Pila)
                eliminarUltimoHistorial(cima);
                break;

            case 8: // Mostrar historial (Pila)
                mostrarHistorial(cima);
                break;

            case 0:
                cout << "Saliendo del sistema académico...\n";
                break;
        }
        
        if (opcion != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 0);

    return 0;
}