#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iomanip>

using namespace std;

void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string floatAString(float num) {
    stringstream ss;
    ss << fixed << setprecision(1) << num;
    return ss.str();
}

struct Materia {
    string nombreMateria;
    vector<float> notas;
};

struct Estudiante {
    string nombre;
    string codigo;
    vector<Materia> materias;
    string concepto;
    string materiaPendiente;
    Estudiante* siguiente;
    Estudiante* izquierdo;
    Estudiante* derecho;
};

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

string pedirIdentificacionValida() {
    string identificacion;
    do {
        cout << " Identificación (solo números): ";
        getline(cin, identificacion);
        if (!esNumero(identificacion)) {
            cout << "  [ERROR] Solo se permiten números. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return identificacion;
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

string pedirNombreMateriaValido() {
    string nombreMateria;
    do {
        cout << " Nombre de la Materia (solo letras y espacios): ";
        getline(cin, nombreMateria);
        if (!esTexto(nombreMateria)) {
            cout << "  [ERROR] Solo se permiten letras y espacios. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return nombreMateria;
}

bool materiaExiste(const vector<Materia>& materias, const string& nombreMateria) {
    for (const auto& mat : materias) {
        if (mat.nombreMateria == nombreMateria) {
            return true;
        }
    }
    return false;
}

Estudiante* insertarEnBST(Estudiante* raiz, Estudiante* nuevo) {
    if (raiz == nullptr) return nuevo;
    if (stoi(nuevo->codigo) < stoi(raiz->codigo)) {
        raiz->izquierdo = insertarEnBST(raiz->izquierdo, nuevo);
    } else {
        raiz->derecho = insertarEnBST(raiz->derecho, nuevo);
    }
    return raiz;
}

Estudiante* buscarEnBST(Estudiante* raiz, const string& codigo) {
    if (raiz == nullptr || raiz->codigo == codigo) return raiz;
    if (stoi(codigo) < stoi(raiz->codigo)) {
        return buscarEnBST(raiz->izquierdo, codigo);
    } else {
        return buscarEnBST(raiz->derecho, codigo);
    }
}

void mostrarInorder(Estudiante* raiz) {
    if (raiz != nullptr) {
        mostrarInorder(raiz->izquierdo);
        cout << "Nombre: " << raiz->nombre
             << " | Identificación: " << raiz->codigo
             << " | Materias: ";
        if (raiz->materias.empty()) {
            cout << "N/A";
        } else {
            for (size_t i = 0; i < raiz->materias.size(); ++i) {
                cout << raiz->materias[i].nombreMateria << "(";
                for (size_t j = 0; j < raiz->materias[i].notas.size(); ++j) {
                    cout << raiz->materias[i].notas[j];
                    if (j < raiz->materias[i].notas.size() - 1) cout << ",";
                }
                cout << ")";
                if (i < raiz->materias.size() - 1) cout << " ";
            }
        }
        cout << endl;
        mostrarInorder(raiz->derecho);
    }
}

bool codigoExiste(Estudiante* cabeza, const string& codigo) {
    return buscarEnBST(cabeza, codigo) != nullptr;
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

void registrarEstudiante(Estudiante*& cabeza, string nombre, string codigo) {
    if (codigoExiste(cabeza, codigo)) {
        cout << "  [ERROR] Ya existe un estudiante con el código " << codigo << ". No se registró.\n";
        return;
    }
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->izquierdo = nullptr;
    nuevo->derecho = nullptr;
    cabeza = insertarEnBST(cabeza, nuevo);
    cout << "  [ÉXITO] Estudiante " << nombre << " registrado con éxito (Sin materias iniciales).\n";
}

Estudiante* buscarEstudiante(Estudiante* cabeza, const string& codigo) {
    return buscarEnBST(cabeza, codigo);
}

void mostrarRegistro(Estudiante* cabeza) {
    cout << "\n====== LISTADO GENERAL DE ESTUDIANTES (ORDENADO POR CÓDIGO) ======\n";
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }
    mostrarInorder(cabeza);
}

float calcularPromedio(const string& notasStr) {
    if (notasStr.empty()) return 0.0f;

    stringstream ss(notasStr);
    string segmento;
    float suma = 0.0f;
    int contador = 0;

    while (getline(ss, segmento, ',')) {
        try {
            suma += stof(segmento);
            contador++;
        } catch (...) {
        }
    }

    if (contador == 0) return 0.0f;

    return suma / contador;
}

float calcularPromedioMateria(const vector<float>& notas) {
    if (notas.empty()) return 0.0f;
    float suma = 0.0f;
    for (float n : notas) suma += n;
    return suma / notas.size();
}

float calcularPromedioGeneral(const vector<Materia>& materias) {
    if (materias.empty()) return 0.0f;
    float sumaPromedios = 0.0f;
    for (const auto& mat : materias) {
        sumaPromedios += calcularPromedioMateria(mat.notas);
    }
    return sumaPromedios / materias.size();
}

void buscarEstudianteYMostrarPromedio(Estudiante* cabeza) {
    string codigoBuscado;
    cout << "\n====== BÚSQUEDA DE ESTUDIANTE Y PROMEDIOS ======\n";
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados para buscar.\n";
        return;
    }

    cout << "Ingrese la Identificación del estudiante a buscar: ";
    getline(cin, codigoBuscado);
    if (!esNumero(codigoBuscado)) {
        cout << "  [ERROR] Código inválido. Solo se permiten números.\n";
        return;
    }

    Estudiante* encontrado = buscarEstudiante(cabeza, codigoBuscado);
    if (encontrado != nullptr) {
        cout << "\n  Estudiante Encontrado!\n";
        cout << "  Nombre: " << encontrado->nombre << endl;
        cout << "  Identificación: " << encontrado->codigo << endl;
        if (encontrado->materias.empty()) {
            cout << "  Materias: N/A" << endl;
            cout << "  Promedio General: N/A" << endl;
        } else {
            cout << "  Materias y Notas:" << endl;
            for (const auto& mat : encontrado->materias) {
                cout << "    " << mat.nombreMateria << ": ";
                if (mat.notas.empty()) {
                    cout << "Sin notas";
                } else {
                    for (size_t i = 0; i < mat.notas.size(); ++i) {
                        cout << mat.notas[i];
                        if (i < mat.notas.size() - 1) cout << ", ";
                    }
                    cout << " (Promedio: " << floatAString(calcularPromedioMateria(mat.notas)) << ")";
                }
                cout << endl;
            }
            cout << "  Promedio General: " << floatAString(calcularPromedioGeneral(encontrado->materias)) << endl;
        }
    } else {
        cout << "  [ERROR] No se encontró ningún estudiante con el código " << codigoBuscado << ".\n";
    }
}

void agregarMateriaAEstudiante(Estudiante* estudiante, string nombreMateria) {
    if (materiaExiste(estudiante->materias, nombreMateria)) {
        cout << "  [ERROR] La materia " << nombreMateria << " ya existe para este estudiante.\n";
        return;
    }
    Materia nueva;
    nueva.nombreMateria = nombreMateria;
    estudiante->materias.push_back(nueva);
    cout << "  [ÉXITO] Materia " << nombreMateria << " agregada al estudiante " << estudiante->nombre << ".\n";
}

void eliminarMateriaDeEstudiante(Estudiante* estudiante, string nombreMateria) {
    for (auto it = estudiante->materias.begin(); it != estudiante->materias.end(); ++it) {
        if (it->nombreMateria == nombreMateria) {
            estudiante->materias.erase(it);
            cout << "  [ÉXITO] Materia " << nombreMateria << " eliminada del estudiante " << estudiante->nombre << ".\n";
            return;
        }
    }
    cout << "  [ERROR] Materia " << nombreMateria << " no encontrada para este estudiante.\n";
}

void registrarNotaEnMateria(Estudiante* estudiante, string nombreMateria, float nota) {
    for (auto& mat : estudiante->materias) {
        if (mat.nombreMateria == nombreMateria) {
            mat.notas.push_back(nota);
            cout << "  [ÉXITO] Nota " << floatAString(nota) << " registrada en " << nombreMateria << " para " << estudiante->nombre << ".\n";
            return;
        }
    }
    cout << "  [ERROR] Materia " << nombreMateria << " no encontrada para este estudiante.\n";
}

void registrarNotaPendiente(Estudiante*& frente, Estudiante*& final, Estudiante* lista, string codigo, string nombreMateria, float nota) {
    Estudiante* estudianteExistente = buscarEstudiante(lista, codigo);
    if (estudianteExistente == nullptr) {
        cout << "  [ERROR] El estudiante con código " << codigo << " no existe en el registro principal.\n";
        return;
    }
    if (!materiaExiste(estudianteExistente->materias, nombreMateria)) {
        cout << "  [ERROR] La materia " << nombreMateria << " no existe para el estudiante " << estudianteExistente->nombre << ".\n";
        return;
    }

    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = estudianteExistente->nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = floatAString(nota);
    nuevo->materiaPendiente = nombreMateria;
    nuevo->siguiente = nullptr;

    if (final == nullptr) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    cout << "  [ÉXITO] Nota " << floatAString(nota) << " para " << nombreMateria << " registrada en la cola para " << estudianteExistente->nombre << ".\n";
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
    string materia = temp->materiaPendiente;

    cout << "\n====== PROCESANDO NOTA (FIFO) ======\n";
    cout << "  Estudiante: " << temp->nombre
         << " | Identificación: " << temp->codigo
         << " | Materia: " << materia
         << " | Nota a Añadir: " << nuevaNotaStr << endl;

    if (estudianteLista != nullptr) {
        registrarNotaEnMateria(estudianteLista, materia, stof(nuevaNotaStr));
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
    cout << "\n====== NOTAS PENDIENTES EN COLA ======\n";
    if (frente == nullptr) {
        cout << "No hay notas pendientes en espera.\n";
        return;
    }
    Estudiante* actual = frente;
    int i = 1;
    while (actual != nullptr) {
        cout << "  " << i++ << ". Identificación: " << actual->codigo
             << " | Nombre: " << actual->nombre
             << " | Materia: " << actual->materiaPendiente
             << " | Nota a Asignar: " << actual->concepto << endl;
        actual = actual->siguiente;
    }
}

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
    cout << "\n====== ELIMINANDO DEL HISTORIAL ======\n";
    cout << "  Registro Eliminado: Nombre: " << temp->nombre
          << " | Identificación: " << temp->codigo
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
              << " | Identificación: " << actual->codigo << endl;
        actual = actual->siguiente;
    }
}

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
        
        cout << "\n----- SISTEMA DE GESTIÓN ACADÉMICA (ESTRUCTURAS DE DATOS) -----\n\n";
        cout << "  [ BST - REGISTRO DE ESTUDIANTES ]\n";
        cout << "  1. Registrar nuevo estudiante (Inicia sin materias)\n";
        cout << "  2. Mostrar todos los estudiantes\n";
        cout << "  3. Buscar estudiante y ver materias/notas/promedios\n\n";
        cout << "  [ GESTIÓN DE MATERIAS ]\n";
        cout << "  4. Agregar materia a estudiante\n";
        cout << "  5. Eliminar materia de estudiante\n\n";
        cout << "  [ COLA - ASIGNACIÓN DE NOTAS PENDIENTES ]\n";
        cout << "  6. Registrar Nota Pendiente para un estudiante (especificar materia)\n";
        cout << "  7. Procesar la siguiente nota pendiente (Asignar)\n";
        cout << "  8. Ver notas pendientes en espera\n\n";
        cout << "  [ PILA - HISTORIAL DE ACCIONES ]\n";
        cout << "  9. Registrar acción en historial\n";
        cout << "  10. Eliminar último registro del historial\n";
        cout << "  11. Ver historial de acciones\n\n";
        cout << "  0. Salir del sistema\n";
        cout << "Seleccione una opción: ";

        opcion = pedirOpcionValida(0, 11);

        limpiarConsola(); 

        switch (opcion) {
            case 1: {
                cout << "\n-- REGISTRAR ESTUDIANTE --\n";
                nombre = pedirNombreValido();
                codigo = pedirIdentificacionValida();
                registrarEstudiante(lista, nombre, codigo);
                break;
            }

            case 2: {
                mostrarRegistro(lista);
                break;
            }

            case 3: {
                buscarEstudianteYMostrarPromedio(lista);
                break;
            }

            case 4: {
                cout << "\n-- AGREGAR MATERIA A ESTUDIANTE --\n";
                cout << "Ingrese la identificación del estudiante:\n";
                codigo = pedirIdentificacionValida();
                Estudiante* estAgregar = buscarEstudiante(lista, codigo);
                if (estAgregar == nullptr) {
                    cout << "  [ERROR] Estudiante no encontrado.\n";
                } else {
                    cout << "Ingrese el nombre de la materia:\n";
                    nombre = pedirNombreMateriaValido();
                    agregarMateriaAEstudiante(estAgregar, nombre);
                }
                break;
            }

            case 5: {
                cout << "\n-- ELIMINAR MATERIA DE ESTUDIANTE --\n";
                cout << "Ingrese la identificación del estudiante:\n";
                codigo = pedirIdentificacionValida();
                Estudiante* estEliminar = buscarEstudiante(lista, codigo);
                if (estEliminar == nullptr) {
                    cout << "  [ERROR] Estudiante no encontrado.\n";
                } else {
                    cout << "Ingrese el nombre de la materia a eliminar:\n";
                    nombre = pedirNombreMateriaValido();
                    eliminarMateriaDeEstudiante(estEliminar, nombre);
                }
                break;
            }


            case 6: {
                cout << "\n-- REGISTRAR NOTA PENDIENTE --\n";
                cout << "Ingrese la identificación del estudiante:\n";
                codigo = pedirIdentificacionValida();
                Estudiante* estudiante = buscarEstudiante(lista, codigo);
                if (estudiante == nullptr) {
                    cout << "  [ERROR] Estudiante no encontrado.\n";
                } else if (estudiante->materias.empty()) {
                    cout << "  [ERROR] El estudiante no tiene materias registradas.\n";
                } else {
                    cout << "Materias disponibles para " << estudiante->nombre << ":\n";
                    for (size_t i = 0; i < estudiante->materias.size(); ++i) {
                        cout << "  " << (i + 1) << ". " << estudiante->materias[i].nombreMateria << endl;
                    }
                    cout << "Seleccione el número de la materia: ";
                    int numMateria = pedirOpcionValida(1, estudiante->materias.size());
                    string nombreMateria = estudiante->materias[numMateria - 1].nombreMateria;
                    do {
                        nota = pedirNotaValida();
                        registrarNotaPendiente(frente, final, lista, codigo, nombreMateria, nota);
                        cout << "¿Quieres agregar otra nota a la misma materia? (S/N): ";
                        char respuesta;
                        cin >> respuesta;
                        cin.ignore(1000, '\n');
                        if (respuesta != 'S' && respuesta != 's') break;
                    } while (true);
                }
                break;
            }

            case 7: {
                procesarNota(frente, final, lista);
                break;
            }

            case 8: {
                mostrarNotasPendientes(frente);
                break;
            }

            case 9: {
                cout << "\n-- REGISTRAR ACCIÓN EN HISTORIAL --\n";
                nombre = pedirNombreValido();
                codigo = pedirIdentificacionValida();
                cout << " Descripción de la Acción: ";
                getline(cin, concepto);
                registrarHistorial(cima, nombre, codigo, concepto);
                break;
            }

            case 10: {
                eliminarUltimoHistorial(cima);
                break;
            }

            case 11: {
                mostrarHistorial(cima);
                break;
            }

            case 0: {
                cout << "Saliendo del sistema académico...\n";
                break;
            }
        }
        
        if (opcion != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 0);

    return 0;
}