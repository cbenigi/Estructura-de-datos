#include <iostream>

using namespace std;

const int MAX_EST = 100;
const int MAX_MAT = 10;

int N, M;
string estudiantes[MAX_EST];
string codigos[MAX_EST];
string materias[MAX_MAT];
float notas[MAX_EST][MAX_MAT];

float promedio_estudiante(int i) {
    float suma = 0;
    for (int j = 0; j < M; j++) suma += notas[i][j];
    return (M > 0) ? suma / M : 0;
}

float promedio_materia(int j) {
    float suma = 0;
    for (int i = 0; i < N; i++) suma += notas[i][j];
    return (N > 0) ? suma / N : 0;
}

int main() {
    cout << "=== Sistema Academico Basico ===\n";
    cout << "Numero de estudiantes: ";
    cin >> N;
    cout << "Numero de materias: ";
    cin >> M;

    cin.ignore(); 

    
    cout << "\n--- Ingreso de materias ---\n";
    for (int j = 0; j < M; j++) {
        cout << "Materia " << j + 1 << ": ";
        getline(cin, materias[j]);
    }

    cout << "\n--- Ingreso de estudiantes ---\n";
    for (int i = 0; i < N; i++) {
        cout << "Nombre estudiante " << i + 1 << ": ";
        getline(cin, estudiantes[i]);
        cout << "Codigo: ";
        getline(cin, codigos[i]);

        for (int j = 0; j < M; j++) {
            cout << "Nota en " << materias[j] << ": ";
            cin >> notas[i][j];
        }
        cin.ignore();
    }

    cout << "\n--- Listado ---\n";
    for (int i = 0; i < N; i++) {
        cout << estudiantes[i] << " (" << codigos[i] << ")\n";
        for (int j = 0; j < M; j++) {
            cout << "   " << materias[j] << ": " << notas[i][j] << "\n";
        }
        cout << "   Promedio: " << promedio_estudiante(i) << "\n";
    }

    cout << "\n--- Estadisticas ---\n";
    for (int j = 0; j < M; j++) {
        cout << materias[j] << " -> Promedio: " << promedio_materia(j) << "\n";
    }

    return 0;
}

