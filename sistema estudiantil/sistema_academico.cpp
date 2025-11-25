#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

using namespace std;

// ============================
//  Utilidades visuales / menú
// ============================

void limpiarConsola() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausa() {
    cout << "\nPresiona ENTER para continuar...";
    cin.get();
}

string repetir(char c, int n) { return string(n, c); }

void imprimirLinea(int ancho = 60, char c='=') {
    cout << repetir(c, ancho) << "\n";
}

void imprimirTitulo(const string& t, int ancho = 60) {
    imprimirLinea(ancho, '=');
    int espacios = (ancho - (int)t.size()) / 2;
    cout << repetir(' ', max(0, espacios)) << t << "\n";
    imprimirLinea(ancho, '=');
}

void imprimirSeccion(const string& t, int ancho = 60) {
    cout << "\n" << repetir('-', ancho) << "\n";
    cout << " " << t << "\n";
    cout << repetir('-', ancho) << "\n";
}

string floatAString(float num) {
    stringstream ss;
    ss << fixed << setprecision(1) << num;
    return ss.str();
}

// ============================
//  Estructuras del sistema
// ============================

struct Materia {
    string nombreMateria;
    vector<float> notas;
};

struct Estudiante {
    string nombre;
    string codigo;
    vector<Materia> materias;

    // para cola / pila
    string concepto;
    string materiaPendiente;
    Estudiante* siguiente = nullptr;

    // para BST
    Estudiante* izquierdo = nullptr;
    Estudiante* derecho = nullptr;
};

// ============================
//  GRAFO: Materias (prerrequisitos)
//  - muestra ciclo exacto si existe
//  - muestra plan topológico si no existe
// ============================

struct GrafoMaterias {
    unordered_map<string, vector<string>> adj; // A -> {B,C}

    void agregarMateria(const string& m) {
        if (!adj.count(m)) adj[m] = {};
    }

    void agregarPrerequisito(const string& pre, const string& post) {
        agregarMateria(pre);
        agregarMateria(post);
        adj[pre].push_back(post);
    }

    vector<string> prerrequisitosDe(const string& materia) {
        vector<string> pres;
        for (auto &par : adj) {
            for (auto &dest : par.second) {
                if (dest == materia) pres.push_back(par.first);
            }
        }
        return pres;
    }

    void mostrar() {
        imprimirSeccion("GRAFO DE PRERREQUISITOS (A -> B)");
        if (adj.empty()) {
            cout << "No hay materias registradas en el grafo.\n";
            return;
        }
        for (auto &par : adj) {
            cout << " " << par.first << "  ->  ";
            if (par.second.empty()) cout << "Ninguna";
            else {
                for (size_t i=0;i<par.second.size();++i){
                    cout << par.second[i];
                    if (i+1<par.second.size()) cout << ", ";
                }
            }
            cout << "\n";
        }
    }

    // ---- Encontrar ciclo exacto ----
    bool dfsCiclo(const string& u,
                  unordered_set<string>& visit,
                  unordered_set<string>& enPila,
                  unordered_map<string,string>& parent,
                  vector<string>& ciclo) {
        visit.insert(u);
        enPila.insert(u);

        for (auto &v : adj[u]) {
            if (!visit.count(v)) {
                parent[v] = u;
                if (dfsCiclo(v, visit, enPila, parent, ciclo)) return true;
            } else if (enPila.count(v)) {
                // Back-edge u -> v => ciclo
                ciclo.clear();
                string cur = u;
                ciclo.push_back(v);
                while (cur != v) {
                    ciclo.push_back(cur);
                    cur = parent[cur];
                }
                ciclo.push_back(v);
                reverse(ciclo.begin(), ciclo.end());
                return true;
            }
        }

        enPila.erase(u);
        return false;
    }

    vector<string> encontrarCiclo() {
        unordered_set<string> visit, enPila;
        unordered_map<string,string> parent;
        vector<string> ciclo;

        for (auto &par : adj) {
            const string& nodo = par.first;
            if (!visit.count(nodo)) {
                parent[nodo] = nodo;
                if (dfsCiclo(nodo, visit, enPila, parent, ciclo))
                    return ciclo;
            }
        }
        return {}; // vacío => no hay ciclo
    }

    // ---- Orden topológico (plan posible) ----
    vector<string> ordenTopologico() {
        unordered_map<string,int> indeg;

        for (auto &par : adj) indeg[par.first] = 0;
        for (auto &par : adj)
            for (auto &v : par.second)
                indeg[v]++;

        queue<string> q;
        for (auto &par : indeg)
            if (par.second == 0) q.push(par.first);

        vector<string> orden;
        while (!q.empty()) {
            string u = q.front(); q.pop();
            orden.push_back(u);
            for (auto &v : adj[u]) {
                indeg[v]--;
                if (indeg[v] == 0) q.push(v);
            }
        }

        return orden; // si tamaño < adj.size() => había ciclo
    }

    void mostrarPlanTopologico() {
        imprimirSeccion("PLAN POSIBLE DE CURSO (SIN CICLOS)");
        auto orden = ordenTopologico();
        if (orden.size() != adj.size()) {
            cout << "No se puede mostrar plan porque hay ciclos.\n";
            return;
        }

        for (size_t i=0;i<orden.size();++i) {
            cout << orden[i];
            if (i+1<orden.size()) cout << " -> ";
        }
        cout << "\n";
    }
};

// ============================
//  Validaciones
// ============================

bool esNumero(const string& str) {
    for (char c : str) if (!isdigit(c)) return false;
    return !str.empty();
}

bool esTexto(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return !str.empty();
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
        } else break;
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
        } else break;
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
        } else break;
    } while (true);
    return nombreMateria;
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
        cout << "  [ERROR] Opción no válida. Ingrese un número entre "
             << minOp << " y " << maxOp << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return opcion;
}

// ============================
//  BST estudiantes
// ============================

bool materiaExiste(const vector<Materia>& materias, const string& nombreMateria) {
    for (const auto& mat : materias)
        if (mat.nombreMateria == nombreMateria) return true;
    return false;
}

Estudiante* insertarEnBST(Estudiante* raiz, Estudiante* nuevo) {
    if (raiz == nullptr) return nuevo;
    if (stoi(nuevo->codigo) < stoi(raiz->codigo))
        raiz->izquierdo = insertarEnBST(raiz->izquierdo, nuevo);
    else
        raiz->derecho = insertarEnBST(raiz->derecho, nuevo);
    return raiz;
}

Estudiante* buscarEnBST(Estudiante* raiz, const string& codigo) {
    if (raiz == nullptr || raiz->codigo == codigo) return raiz;
    if (stoi(codigo) < stoi(raiz->codigo))
        return buscarEnBST(raiz->izquierdo, codigo);
    return buscarEnBST(raiz->derecho, codigo);
}

void mostrarInorder(Estudiante* raiz) {
    if (raiz != nullptr) {
        mostrarInorder(raiz->izquierdo);

        cout << "Nombre: " << raiz->nombre
             << " | Identificación: " << raiz->codigo
             << " | Materias: ";

        if (raiz->materias.empty()) cout << "N/A";
        else {
            for (size_t i = 0; i < raiz->materias.size(); ++i) {
                cout << raiz->materias[i].nombreMateria << "(";
                for (size_t j = 0; j < raiz->materias[i].notas.size(); ++j) {
                    cout << raiz->materias[i].notas[j];
                    if (j + 1 < raiz->materias[i].notas.size()) cout << ",";
                }
                cout << ")";
                if (i + 1 < raiz->materias.size()) cout << " ";
            }
        }
        cout << "\n";

        mostrarInorder(raiz->derecho);
    }
}

bool codigoExiste(Estudiante* cabeza, const string& codigo) {
    return buscarEnBST(cabeza, codigo) != nullptr;
}

void registrarEstudiante(Estudiante*& cabeza, string nombre, string codigo) {
    if (codigoExiste(cabeza, codigo)) {
        cout << "  [ERROR] Ya existe un estudiante con el código " << codigo << ".\n";
        return;
    }
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    cabeza = insertarEnBST(cabeza, nuevo);
    cout << "  [ÉXITO] Estudiante " << nombre << " registrado (sin materias).\n";
}

Estudiante* buscarEstudiante(Estudiante* cabeza, const string& codigo) {
    return buscarEnBST(cabeza, codigo);
}

void mostrarRegistro(Estudiante* cabeza) {
    imprimirSeccion("LISTADO GENERAL DE ESTUDIANTES (BST por código)");
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }
    mostrarInorder(cabeza);
}

// ============================
//  Promedios
// ============================

float calcularPromedioMateria(const vector<float>& notas) {
    if (notas.empty()) return 0.0f;
    float suma = 0.0f;
    for (float n : notas) suma += n;
    return suma / notas.size();
}

float calcularPromedioGeneral(const vector<Materia>& materias) {
    if (materias.empty()) return 0.0f;
    float sumaProm = 0.0f;
    for (const auto& mat : materias)
        sumaProm += calcularPromedioMateria(mat.notas);
    return sumaProm / materias.size();
}

void buscarEstudianteYMostrarPromedio(Estudiante* cabeza) {
    imprimirSeccion("BÚSQUEDA DE ESTUDIANTE Y PROMEDIOS");
    if (cabeza == nullptr) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    string codigoBuscado;
    cout << "Ingrese la Identificación del estudiante: ";
    getline(cin, codigoBuscado);

    if (!esNumero(codigoBuscado)) {
        cout << "  [ERROR] Código inválido. Solo números.\n";
        return;
    }

    Estudiante* e = buscarEstudiante(cabeza, codigoBuscado);
    if (!e) {
        cout << "  [ERROR] No se encontró estudiante con código " << codigoBuscado << ".\n";
        return;
    }

    cout << "\n  Estudiante Encontrado\n";
    cout << "  Nombre: " << e->nombre << "\n";
    cout << "  Identificación: " << e->codigo << "\n";

    if (e->materias.empty()) {
        cout << "  Materias: N/A\n";
        cout << "  Promedio general: N/A\n";
        return;
    }

    cout << "  Materias y Notas:\n";
    for (auto &mat : e->materias) {
        cout << "   - " << mat.nombreMateria << ": ";
        if (mat.notas.empty()) cout << "Sin notas";
        else {
            for (size_t i = 0; i < mat.notas.size(); ++i) {
                cout << mat.notas[i];
                if (i + 1 < mat.notas.size()) cout << ", ";
            }
            cout << " (Promedio: " << floatAString(calcularPromedioMateria(mat.notas)) << ")";
        }
        cout << "\n";
    }

    cout << "  Promedio general: " << floatAString(calcularPromedioGeneral(e->materias)) << "\n";
}

// ============================
//  Materias (por estudiante)
// ============================

bool estudianteTieneMateria(Estudiante* e, const string& nombreMateria) {
    for (auto &m : e->materias)
        if (m.nombreMateria == nombreMateria) return true;
    return false;
}

void agregarMateriaAEstudiante(Estudiante* estudiante,
                               string nombreMateria,
                               GrafoMaterias& grafo) {
    if (materiaExiste(estudiante->materias, nombreMateria)) {
        cout << "  [ERROR] La materia ya existe para este estudiante.\n";
        return;
    }

    // Validar prerrequisitos desde grafo
    auto pres = grafo.prerrequisitosDe(nombreMateria);
    for (auto &pre : pres) {
        if (!estudianteTieneMateria(estudiante, pre)) {
            cout << "  [ERROR] No puede inscribir \"" << nombreMateria
                 << "\" sin haber cursado prerrequisito: \"" << pre << "\"\n";
            return;
        }
    }

    Materia nueva;
    nueva.nombreMateria = nombreMateria;
    estudiante->materias.push_back(nueva);
    cout << "  [ÉXITO] Materia agregada a " << estudiante->nombre << ".\n";
}

void eliminarMateriaDeEstudiante(Estudiante* estudiante, string nombreMateria) {
    for (auto it = estudiante->materias.begin(); it != estudiante->materias.end(); ++it) {
        if (it->nombreMateria == nombreMateria) {
            estudiante->materias.erase(it);
            cout << "  [ÉXITO] Materia eliminada.\n";
            return;
        }
    }
    cout << "  [ERROR] Materia no encontrada.\n";
}

void registrarNotaEnMateria(Estudiante* estudiante, string nombreMateria, float nota) {
    for (auto& mat : estudiante->materias) {
        if (mat.nombreMateria == nombreMateria) {
            mat.notas.push_back(nota);
            cout << "  [ÉXITO] Nota " << floatAString(nota) << " registrada en " << nombreMateria << ".\n";
            return;
        }
    }
    cout << "  [ERROR] Materia no encontrada.\n";
}

// ============================
//  Cola de notas pendientes (FIFO)
// ============================

void registrarNotaPendiente(Estudiante*& frente, Estudiante*& final,
                            Estudiante* lista,
                            string codigo, string nombreMateria, float nota) {
    Estudiante* estudianteExistente = buscarEstudiante(lista, codigo);
    if (!estudianteExistente) {
        cout << "  [ERROR] Estudiante no existe en registro.\n";
        return;
    }
    if (!materiaExiste(estudianteExistente->materias, nombreMateria)) {
        cout << "  [ERROR] La materia no existe para ese estudiante.\n";
        return;
    }

    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = estudianteExistente->nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = floatAString(nota);
    nuevo->materiaPendiente = nombreMateria;
    nuevo->siguiente = nullptr;

    if (!final) frente = final = nuevo;
    else {
        final->siguiente = nuevo;
        final = nuevo;
    }

    cout << "  [ÉXITO] Nota encolada para " << estudianteExistente->nombre << ".\n";
}

void procesarNota(Estudiante*& frente, Estudiante*& final, Estudiante* lista) {
    if (!frente) {
        cout << "  [ADVERTENCIA] No hay notas pendientes.\n";
        return;
    }

    if (!pedirConfirmacion("procesar la siguiente nota")) {
        cout << "  Proceso cancelado.\n";
        return;
    }

    Estudiante* temp = frente;
    Estudiante* estudianteLista = buscarEstudiante(lista, temp->codigo);

    imprimirSeccion("PROCESANDO NOTA (FIFO)");
    cout << "  Estudiante: " << temp->nombre
         << " | Identificación: " << temp->codigo
         << " | Materia: " << temp->materiaPendiente
         << " | Nota: " << temp->concepto << "\n";

    if (estudianteLista)
        registrarNotaEnMateria(estudianteLista, temp->materiaPendiente, stof(temp->concepto));
    else
        cout << "  [ERROR] Estudiante no encontrado en lista principal.\n";

    frente = frente->siguiente;
    if (!frente) final = nullptr;

    delete temp;
    cout << "  [ÉXITO] Nota procesada y eliminada de la cola.\n";
}

void mostrarNotasPendientes(Estudiante* frente) {
    imprimirSeccion("NOTAS PENDIENTES EN COLA");
    if (!frente) {
        cout << "No hay notas pendientes.\n";
        return;
    }
    int i=1;
    Estudiante* a = frente;
    while (a) {
        cout << "  " << i++ << ") " << a->codigo
             << " | " << a->nombre
             << " | " << a->materiaPendiente
             << " | Nota: " << a->concepto << "\n";
        a = a->siguiente;
    }
}

// ============================
//  Pila historial (LIFO)
// ============================

void registrarHistorial(Estudiante*& cima, string nombre, string codigo, string concepto) {
    Estudiante* nuevo = new Estudiante();
    nuevo->nombre = nombre;
    nuevo->codigo = codigo;
    nuevo->concepto = concepto;
    nuevo->siguiente = cima;
    cima = nuevo;
    cout << "  [ÉXITO] Acción registrada en historial.\n";
}

void eliminarUltimoHistorial(Estudiante*& cima) {
    if (!cima) {
        cout << "  [ADVERTENCIA] Historial vacío.\n";
        return;
    }

    if (!pedirConfirmacion("eliminar el último historial")) {
        cout << "  Eliminación cancelada.\n";
        return;
    }

    Estudiante* temp = cima;

    imprimirSeccion("ELIMINANDO DEL HISTORIAL");
    cout << "  Nombre: " << temp->nombre
         << " | Identificación: " << temp->codigo
         << " | Acción: " << temp->concepto << "\n";

    cima = cima->siguiente;
    delete temp;

    cout << "  [ÉXITO] Registro eliminado.\n";
}

void mostrarHistorial(Estudiante* cima) {
    imprimirSeccion("HISTORIAL DE ACCIONES (LIFO)");
    if (!cima) {
        cout << "No hay acciones registradas.\n";
        return;
    }
    Estudiante* a = cima;
    while (a) {
        cout << "  Acción: " << a->concepto
             << " | " << a->nombre
             << " (" << a->codigo << ")\n";
        a = a->siguiente;
    }
}

// ============================
//  Menú
// ============================

void mostrarMenu() {
    imprimirTitulo("SISTEMA DE GESTION ACADEMICA");

    cout <<
    "  [ BST - REGISTRO DE ESTUDIANTES ]\n"
    "   1. Registrar nuevo estudiante\n"
    "   2. Mostrar todos los estudiantes\n"
    "   3. Buscar estudiante y ver promedios\n\n"

    "  [ GESTION DE MATERIAS ]\n"
    "   4. Agregar materia a estudiante (valida prerrequisitos)\n"
    "   5. Eliminar materia de estudiante\n\n"

    "  [ COLA - NOTAS PENDIENTES ]\n"
    "   6. Registrar nota pendiente\n"
    "   7. Procesar siguiente nota pendiente\n"
    "   8. Ver notas pendientes\n\n"

    "  [ PILA - HISTORIAL ]\n"
    "   9. Registrar accion en historial\n"
    "  10. Eliminar ultimo historial\n"
    "  11. Ver historial\n\n"

    "  [ GRAFO - PRERREQUISITOS DE MATERIAS ]\n"
    "  12. Agregar prerrequisito (A -> B)\n"
    "  13. Ver grafo de prerrequisitos\n"
    "  14. Verificar ciclos y mostrar plan\n\n"

    "   0. Salir\n";

    imprimirLinea();
    cout << "Seleccione una opcion: ";
}

// ============================
//  main()
// ============================

int main() {
    Estudiante* lista = nullptr;  // BST
    Estudiante* frente = nullptr; // cola
    Estudiante* final = nullptr;  // cola
    Estudiante* cima = nullptr;   // pila

    GrafoMaterias grafoMaterias;

    int opcion;
    string nombre, codigo, concepto;
    float nota;

    do {
        limpiarConsola();
        mostrarMenu();
        opcion = pedirOpcionValida(0, 14);
        limpiarConsola();

        switch (opcion) {

        case 1: {
            imprimirSeccion("REGISTRAR ESTUDIANTE");
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
            imprimirSeccion("AGREGAR MATERIA A ESTUDIANTE");
            cout << "Ingrese la identificación del estudiante:\n";
            codigo = pedirIdentificacionValida();
            Estudiante* e = buscarEstudiante(lista, codigo);
            if (!e) cout << "  [ERROR] Estudiante no encontrado.\n";
            else {
                cout << "Ingrese el nombre de la materia:\n";
                string mat = pedirNombreMateriaValido();
                agregarMateriaAEstudiante(e, mat, grafoMaterias);
            }
            break;
        }

        case 5: {
            imprimirSeccion("ELIMINAR MATERIA DE ESTUDIANTE");
            cout << "Ingrese la identificación del estudiante:\n";
            codigo = pedirIdentificacionValida();
            Estudiante* e = buscarEstudiante(lista, codigo);
            if (!e) cout << "  [ERROR] Estudiante no encontrado.\n";
            else {
                cout << "Ingrese el nombre de la materia a eliminar:\n";
                string mat = pedirNombreMateriaValido();
                eliminarMateriaDeEstudiante(e, mat);
            }
            break;
        }

        case 6: {
            imprimirSeccion("REGISTRAR NOTA PENDIENTE");
            cout << "Ingrese la identificación del estudiante:\n";
            codigo = pedirIdentificacionValida();
            Estudiante* e = buscarEstudiante(lista, codigo);

            if (!e) {
                cout << "  [ERROR] Estudiante no encontrado.\n";
                break;
            }
            if (e->materias.empty()) {
                cout << "  [ERROR] El estudiante no tiene materias.\n";
                break;
            }

            cout << "Materias de " << e->nombre << ":\n";
            for (size_t i=0;i<e->materias.size();++i)
                cout << "  " << (i+1) << ". " << e->materias[i].nombreMateria << "\n";

            cout << "Seleccione la materia: ";
            int numMateria = pedirOpcionValida(1, (int)e->materias.size());
            string nombreMateria = e->materias[numMateria-1].nombreMateria;

            do {
                nota = pedirNotaValida();
                registrarNotaPendiente(frente, final, lista, codigo, nombreMateria, nota);

                cout << "¿Agregar otra nota a la misma materia? (S/N): ";
                char r; cin >> r; cin.ignore(1000,'\n');
                if (r!='S' && r!='s') break;
            } while (true);

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
            imprimirSeccion("REGISTRAR ACCION EN HISTORIAL");
            nombre = pedirNombreValido();
            codigo = pedirIdentificacionValida();
            cout << " Descripción de la acción: ";
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

        // ---------- GRAFO MATERIAS ----------
        case 12: {
            imprimirSeccion("AGREGAR PRERREQUISITO (A -> B)");
            cout << "Materia prerrequisito (A):\n";
            string pre = pedirNombreMateriaValido();
            cout << "Materia dependiente (B):\n";
            string post = pedirNombreMateriaValido();

            grafoMaterias.agregarPrerequisito(pre, post);
            cout << "  [ÉXITO] Relación agregada: " << pre << " -> " << post << "\n";
            break;
        }

        case 13: {
            grafoMaterias.mostrar();
            break;
        }

        case 14: {
            imprimirSeccion("VERIFICAR CICLOS Y MOSTRAR PLAN");

            auto ciclo = grafoMaterias.encontrarCiclo();
            if (!ciclo.empty()) {
                cout << "  [ALERTA] Hay un ciclo en los prerrequisitos.\n";
                cout << "  Ciclo encontrado:\n   ";
                for (size_t i=0;i<ciclo.size();++i) {
                    cout << ciclo[i];
                    if (i+1 < ciclo.size()) cout << " -> ";
                }
                cout << "\n";
                cout << "  Eso significa que esas materias se bloquean entre sí.\n";
            } else {
                cout << "  [OK] No hay ciclos. Plan consistente.\n";
                grafoMaterias.mostrarPlanTopologico();
            }

            break;
        }

        case 0:
            cout << "Saliendo del sistema...\n";
            break;
        }

        if (opcion != 0) pausa();

    } while (opcion != 0);

    return 0;
}
