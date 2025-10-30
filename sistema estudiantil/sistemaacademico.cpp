#include <iostream>
#include <cctype>
#include <string>
#include <limits>
using namespace std;

// ---------- ESTRUCTURAS ----------

struct Nota {
    float valor;
    int porcentaje; // 1..100
    Nota* siguiente;
};

struct Materia {
    string nombre;
    Nota* notas;      // lista de notas con pesos
    float notaFinal;  // calculada
    Materia* siguiente;
};

struct Estudiante {
    string nombre;
    string codigo;
    Materia* materias;   // materias asignadas
    float promedioFinal;  // promedio de notaFinal de materias
    Estudiante* siguiente; // lista de estudiantes
};

// Catálogo global de materias (nombres)
struct NodoCatalogoMateria {
    string nombre;
    NodoCatalogoMateria* siguiente;
};

// Colas específicas de estudiantes (para materias y para notas)
struct NodoColaEstudiante {
    Estudiante* estudiante;
    NodoColaEstudiante* siguiente;
};

struct Cola {
    NodoColaEstudiante* frente = nullptr;
    NodoColaEstudiante* fondo = nullptr;
    bool vacia() const { return frente == nullptr; }
    void encolar(Estudiante* e) {
        NodoColaEstudiante* n = new NodoColaEstudiante();
        n->estudiante = e;
        n->siguiente = nullptr;
        if (vacia()) {
            frente = fondo = n;
        } else {
            fondo->siguiente = n;
            fondo = n;
        }
    }
    Estudiante* desencolar() {
        if (vacia()) return nullptr;
        NodoColaEstudiante* n = frente;
        Estudiante* e = n->estudiante;
        frente = frente->siguiente;
        if (frente == nullptr) fondo = nullptr;
        delete n;
        return e;
    }
    void mostrar() const {
        cout << "\n--- Cola (pendientes) ---\n";
        if (vacia()) { cout << "(vacía)\n"; return; }
        int i = 1;
        NodoColaEstudiante* cur = frente;
        while (cur) {
            cout << i++ << ") " << cur->estudiante->nombre << " (" << cur->estudiante->codigo << ")\n";
            cur = cur->siguiente;
        }
    }
};

// Pila administrativa (historial)
struct NodoHistorial {
    string nombre;
    string codigo;
    string concepto;
    NodoHistorial* siguiente;
};

struct PilaHistorial {
    NodoHistorial* cima = nullptr;
    bool vacia() const { return cima == nullptr; }
    void push(const string& nombre, const string& codigo, const string& concepto) {
        NodoHistorial* n = new NodoHistorial();
        n->nombre = nombre; n->codigo = codigo; n->concepto = concepto;
        n->siguiente = cima; cima = n;
    }
    void pop() {
        if (vacia()) { cout << "El historial está vacío.\n"; return; }
        NodoHistorial* n = cima; cima = cima->siguiente;
        cout << "Eliminado del historial: " << n->nombre << " (" << n->codigo << ")\n";
        delete n;
    }
    void mostrar() const {
        cout << "\n====== HISTORIAL DE ACCIONES ======\n";
        if (vacia()) { cout << "No hay registros en el historial.\n"; return; }
        NodoHistorial* cur = cima;
        while (cur) {
            cout << cur->nombre << " | " << cur->codigo << " | " << cur->concepto << '\n';
            cur = cur->siguiente;
        }
    }
};

// ---------- VALIDACIONES ----------

bool esNumero(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

bool esTexto(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!(isalpha(static_cast<unsigned char>(c)) || c == ' ')) return false;
    }
    return true;
}

string pedirCodigoValido() {
    string codigo;
    do {
        cout << "Código (solo números): ";
        cin >> codigo;
        if (!esNumero(codigo)) {
            cout << "  Solo se permiten números. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
    return codigo;
}

string pedirNombreLinea(const string& etiqueta) {
    string nombre;
    cout << etiqueta;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nombre);
    while (!esTexto(nombre)) { cout << "  Inválido. Intente de nuevo: "; getline(cin, nombre); }
    return nombre;
}

float pedirNotaValida() {
    float nota;
    while (true) {
        cout << "Valor de la nota (0 a 5): ";
        cin >> nota;
        if (cin.fail() || nota < 0.0f || nota > 5.0f) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Ingrese una nota válida entre 0 y 5.\n";
        } else {
            return nota;
        }
    }
}

int pedirEnteroPositivo(const string& etiqueta) {
    int v;
    while (true) {
        cout << etiqueta;
        cin >> v;
        if (cin.fail() || v <= 0) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "  Ingrese un entero positivo.\n"; }
        else return v;
    }
}

int pedirPorcentajeValido(int maxRestante) {
    int p;
    while (true) {
        cout << "Porcentaje de la nota (1.." << maxRestante << "): ";
        cin >> p;
        if (cin.fail() || p <= 0 || p > maxRestante) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Ingrese un porcentaje válido.\n";
        } else return p;
    }
}

// ---------- UTILIDADES ----------

Estudiante* crearEstudiante(const string& nombre, const string& codigo) {
    Estudiante* e = new Estudiante();
    e->nombre = nombre;
    e->codigo = codigo;
    e->materias = nullptr;
    e->promedioFinal = 0.0f;
    e->siguiente = nullptr;
    return e;
}

void insertarEstudianteLista(Estudiante*& cabeza, Estudiante* nuevo) {
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

bool existeMateriaEnCatalogo(NodoCatalogoMateria* catalogo, const string& nombre) {
    NodoCatalogoMateria* cur = catalogo;
    while (cur) { if (cur->nombre == nombre) return true; cur = cur->siguiente; }
    return false;
}

void agregarMateriaACatalogo(NodoCatalogoMateria*& catalogo, const string& nombre) {
    if (existeMateriaEnCatalogo(catalogo, nombre)) { cout << "  La materia ya existe.\n"; return; }
    NodoCatalogoMateria* n = new NodoCatalogoMateria();
    n->nombre = nombre; n->siguiente = catalogo; catalogo = n;
}

void listarCatalogo(NodoCatalogoMateria* catalogo) {
    cout << "\n--- Catálogo de Materias ---\n";
    if (!catalogo) { cout << "(vacío)\n"; return; }
    int i = 1; while (catalogo) { cout << i++ << ") " << catalogo->nombre << '\n'; catalogo = catalogo->siguiente; }
}

void asignarMateriaAEstudiante(Estudiante* est, const string& nombreMateria) {
    Materia* m = new Materia();
    m->nombre = nombreMateria; m->notas = nullptr; m->notaFinal = 0.0f; m->siguiente = est->materias;
    est->materias = m;
}

void mostrarEstudiantes(Estudiante* lista) {
    cout << "\n====== LISTADO GENERAL DE ESTUDIANTES ======\n";
    if (!lista) { cout << "No hay estudiantes registrados.\n"; return; }
    Estudiante* cur = lista;
    while (cur) { cout << "Nombre: " << cur->nombre << " | Codigo: " << cur->codigo << '\n'; cur = cur->siguiente; }
}

// ---------- NOTAS ----------

void calcularNotaFinalMateria(Materia* m) {
    float acumulado = 0.0f; int suma = 0; Nota* n = m->notas;
    while (n) { acumulado += n->valor * (static_cast<float>(n->porcentaje) / 100.0f); suma += n->porcentaje; n = n->siguiente; }
    m->notaFinal = (suma == 100) ? acumulado : 0.0f;
}

void calcularPromedioEstudiante(Estudiante* e) {
    float suma = 0.0f; int c = 0; for (Materia* m = e->materias; m; m = m->siguiente) { suma += m->notaFinal; c++; }
    e->promedioFinal = (c > 0) ? (suma / c) : 0.0f;
}

void capturarNotasParaMateria(Materia* m) {
    cout << "\nMateria: " << m->nombre << '\n';
    int n = pedirEnteroPositivo("¿Cuántas notas tendrá esta materia? ");
    int restante = 100;
    for (int i = 1; i <= n; i++) {
        cout << "Nota " << i << ":\n";
        float valor = pedirNotaValida();
        cout << "Porcentaje restante: " << restante << "%\n";
        int p = pedirPorcentajeValido(restante);
        restante -= p;
        Nota* no = new Nota(); no->valor = valor; no->porcentaje = p; no->siguiente = m->notas; m->notas = no;
    }
    if (restante != 0) cout << "  Advertencia: los porcentajes no suman 100%. Nota final será 0.\n";
    calcularNotaFinalMateria(m);
}

void capturarNotasParaEstudiante(Estudiante* e) {
    if (!e->materias) { cout << "  Este estudiante no tiene materias asignadas.\n"; return; }
    for (Materia* m = e->materias; m; m = m->siguiente) capturarNotasParaMateria(m);
    calcularPromedioEstudiante(e);
    cout << "\nResumen de notas de " << e->nombre << " (" << e->codigo << ")\n";
    for (Materia* m = e->materias; m; m = m->siguiente) cout << " - " << m->nombre << ": " << m->notaFinal << '\n';
    cout << "Promedio final: " << e->promedioFinal << "\n";
}

// ---------- MENÚS ----------

void menuAdministrativo(Estudiante*& listaEst, NodoCatalogoMateria*& catalogo,
                        Cola& colaMaterias, Cola& colaNotas, PilaHistorial& historial) {
    int op;
    do {
        cout << "\n--- ADMINISTRATIVO ---\n";
        cout << "1) Registrar estudiante (se encola para registrar materias)\n";
        cout << "2) Registrar materias en catálogo global\n";
        cout << "3) Asignar materias a estudiante (procesa cola de materias)\n";
        cout << "4) Ver estudiantes\n";
        cout << "5) Historial: agregar registro\n";
        cout << "6) Historial: eliminar último\n";
        cout << "7) Historial: ver\n";
        cout << "8) Ver cola de materias\n";
        cout << "9) Ver cola de notas\n";
        cout << "0) Volver\n";
        cout << "Seleccione: ";
        cin >> op;
        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); op = -1; }

        switch (op) {
            case 1: {
                string nombre;
                cout << "Nombre (solo letras): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nombre);
                while (!esTexto(nombre)) { cout << "  Inválido. Intente de nuevo: "; getline(cin, nombre); }
                string codigo = pedirCodigoValido();
                Estudiante* e = crearEstudiante(nombre, codigo);
                insertarEstudianteLista(listaEst, e);
                colaMaterias.encolar(e);
                cout << "  Registrado y encolado para registrar materias.\n";
                break;
            }
            case 2: {
                int k = pedirEnteroPositivo("¿Cuántas materias desea agregar al catálogo? ");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int i = 0; i < k; i++) {
                    string nom; cout << "Nombre materia " << (i+1) << ": "; getline(cin, nom);
                    while (!esTexto(nom)) { cout << "  Inválido. Intente de nuevo: "; getline(cin, nom); }
                    agregarMateriaACatalogo(catalogo, nom);
                }
                listarCatalogo(catalogo);
                break;
            }
            case 3: {
                if (colaMaterias.vacia()) { cout << "No hay estudiantes en cola de materias.\n"; break; }
                Estudiante* e = colaMaterias.desencolar();
                cout << "Asignando materias a: " << e->nombre << " (" << e->codigo << ")\n";
                if (!catalogo) { cout << "  No hay catálogo. Agregue materias primero.\n"; break; }
                listarCatalogo(catalogo);
                int c = pedirEnteroPositivo("¿Cuántas materias asignará? ");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int i = 0; i < c; i++) {
                    string nom; cout << "Materia " << (i+1) << " (nombre exacto): "; getline(cin, nom);
                    if (!existeMateriaEnCatalogo(catalogo, nom)) { cout << "  No existe en catálogo. Intente de nuevo.\n"; i--; continue; }
                    asignarMateriaAEstudiante(e, nom);
                }
                colaNotas.encolar(e);
                cout << "  Materias asignadas. Encolado para registrar notas.\n";
                break;
            }
            case 4:
                mostrarEstudiantes(listaEst);
                break;
            case 5: {
                string nombre, concepto; cout << "Nombre (solo letras): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
                while (!esTexto(nombre)) { cout << "  Inválido. Intente de nuevo: "; getline(cin, nombre); }
                string codigo = pedirCodigoValido();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Concepto: "; getline(cin, concepto);
                historial.push(nombre, codigo, concepto);
                break;
            }
            case 6:
                historial.pop();
                break;
            case 7:
                historial.mostrar();
                break;
            case 8:
                colaMaterias.mostrar();
                break;
            case 9:
                colaNotas.mostrar();
                break;
            case 0:
                break;
            default:
                cout << "Opción no válida.\n";
        }
    } while (op != 0);
}

void menuPrincipalRegistroNotas(Cola& colaNotas) {
    if (colaNotas.vacia()) { cout << "No hay estudiantes en cola de notas.\n"; return; }
    Estudiante* e = colaNotas.desencolar();
    cout << "\n== Registrando notas para: " << e->nombre << " (" << e->codigo << ") ==\n";
    capturarNotasParaEstudiante(e);
}

// ---------- LIBERACIÓN DE MEMORIA ----------

void liberarCatalogo(NodoCatalogoMateria*& catalogo) {
    while (catalogo) { NodoCatalogoMateria* n = catalogo; catalogo = catalogo->siguiente; delete n; }
}

void liberarNotas(Nota*& notas) {
    while (notas) { Nota* n = notas; notas = notas->siguiente; delete n; }
}

void liberarMaterias(Materia*& materias) {
    while (materias) { Materia* m = materias; materias = materias->siguiente; liberarNotas(m->notas); delete m; }
}

void liberarEstudiantes(Estudiante*& lista) {
    while (lista) { Estudiante* e = lista; lista = lista->siguiente; liberarMaterias(e->materias); delete e; }
}

// ---------- MAIN ----------

int main() {
    Estudiante* listaEst = nullptr;
    NodoCatalogoMateria* catalogo = nullptr;
    Cola colaMaterias;   // estudiantes pendientes de asignar materias
    Cola colaNotas;      // estudiantes pendientes de registrar notas
    PilaHistorial historial;

    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1) Administrativo\n";
        cout << "2) Registrar Notas (procesar cola de notas)\n";
        cout << "3) Ver cola de materias\n";
        cout << "4) Ver cola de notas\n";
        cout << "0) Salir\n";
        cout << "Seleccione: ";
        cin >> opcion;
        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion = -1; }

        switch (opcion) {
            case 1:
                menuAdministrativo(listaEst, catalogo, colaMaterias, colaNotas, historial);
                break;
            case 2:
                menuPrincipalRegistroNotas(colaNotas);
                break;
            case 3:
                colaMaterias.mostrar();
                break;
            case 4:
                colaNotas.mostrar();
                break;
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
    } while (opcion != 0);

    liberarCatalogo(catalogo);
    liberarEstudiantes(listaEst);
    return 0;
}

