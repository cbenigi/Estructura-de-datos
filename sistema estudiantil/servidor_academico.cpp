#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

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
    Cola() : frente(nullptr), fondo(nullptr) {}

    bool vacia() { return frente == nullptr; }

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

    bool desencolar(string& nombre, string& codigo) {
        if (vacia()) return false;
        
        NodoCola* temp = frente;
        nombre = temp->nombre;
        codigo = temp->codigo;
        frente = frente->siguiente;
        if (frente == nullptr) fondo = nullptr;
        delete temp;
        return true;
    }

    string toJSON() {
        string json = "[";
        NodoCola* temp = frente;
        bool first = true;
        
        while (temp != nullptr) {
            if (!first) json += ",";
            json += "{\"nombre\":\"" + temp->nombre + "\",\"codigo\":\"" + temp->codigo + "\"}";
            first = false;
            temp = temp->siguiente;
        }
        
        json += "]";
        return json;
    }
};

class Pila {
private:
    NodoPila* cima;

public:
    Pila() : cima(nullptr) {}

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

    bool pop(string& nombre, string& codigo, float& promedio) {
        if (vacia()) return false;
        
        NodoPila* temp = cima;
        nombre = temp->nombre;
        codigo = temp->codigo;
        promedio = temp->promedio;
        cima = cima->siguiente;
        delete temp;
        return true;
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

    string toJSON() {
        string json = "[";
        NodoPila* temp = cima;
        bool first = true;
        
        while (temp != nullptr) {
            if (!first) json += ",";
            
            json += "{\"nombre\":\"" + temp->nombre + "\",";
            json += "\"codigo\":\"" + temp->codigo + "\",";
            
            ostringstream ss;
            ss << fixed << setprecision(2) << temp->promedio;
            json += "\"promedio\":" + ss.str() + "}";
            
            first = false;
            temp = temp->siguiente;
        }
        
        json += "]";
        return json;
    }
};

class SistemaAcademico {
private:
    NodoEstudiante* listaEstudiantes;
    Cola colaInscripciones;
    Pila pilaReprobados;
    vector<string> materias;

public:
    SistemaAcademico() : listaEstudiantes(nullptr) {}

    void setMaterias(vector<string> mats) {
        materias = mats;
    }

    void encolarEstudiante(string nombre, string codigo) {
        colaInscripciones.encolar(nombre, codigo);
    }

    string getColaJSON() {
        return colaInscripciones.toJSON();
    }

    void insertarEstudiante(string nombre, string codigo, vector<float> notas) {
        NodoEstudiante* nuevo = new NodoEstudiante;
        nuevo->nombre = nombre;
        nuevo->codigo = codigo;
        nuevo->materias = nullptr;
        nuevo->siguiente = nullptr;

        for (size_t i = 0; i < materias.size() && i < notas.size(); i++) {
            NodoMateria* nuevaMat = new NodoMateria;
            nuevaMat->nombreMateria = materias[i];
            nuevaMat->nota = notas[i];
            nuevaMat->siguiente = nullptr;

            if (nuevo->materias == nullptr) {
                nuevo->materias = nuevaMat;
            } else {
                NodoMateria* temp = nuevo->materias;
                while (temp->siguiente != nullptr) temp = temp->siguiente;
                temp->siguiente = nuevaMat;
            }
        }

        if (listaEstudiantes == nullptr) {
            listaEstudiantes = nuevo;
        } else {
            NodoEstudiante* temp = listaEstudiantes;
            while (temp->siguiente != nullptr) temp = temp->siguiente;
            temp->siguiente = nuevo;
        }
    }

    float calcularPromedio(NodoEstudiante* est) {
        float suma = 0;
        int count = 0;
        NodoMateria* temp = est->materias;
        
        while (temp != nullptr) {
            suma += temp->nota;
            count++;
            temp = temp->siguiente;
        }
        
        return (count > 0) ? suma / count : 0;
    }

    string getEstudiantesJSON() {
        string json = "[";
        NodoEstudiante* temp = listaEstudiantes;
        bool first = true;
        
        while (temp != nullptr) {
            if (!first) json += ",";
            
            json += "{\"nombre\":\"" + temp->nombre + "\",";
            json += "\"codigo\":\"" + temp->codigo + "\",";
            json += "\"materias\":[";
            
            NodoMateria* mat = temp->materias;
            bool firstMat = true;
            while (mat != nullptr) {
                if (!firstMat) json += ",";
                json += "{\"nombre\":\"" + mat->nombreMateria + "\",";
                
                ostringstream ss;
                ss << fixed << setprecision(2) << mat->nota;
                json += "\"nota\":" + ss.str() + "}";
                
                firstMat = false;
                mat = mat->siguiente;
            }
            
            json += "],\"promedio\":";
            ostringstream prom;
            prom << fixed << setprecision(2) << calcularPromedio(temp);
            json += prom.str() + "}";
            
            first = false;
            temp = temp->siguiente;
        }
        
        json += "]";
        return json;
    }

    string getEstadisticasJSON() {
        int total = 0, aprobados = 0;
        NodoEstudiante* temp = listaEstudiantes;
        
        while (temp != nullptr) {
            total++;
            if (calcularPromedio(temp) >= 3.0) aprobados++;
            temp = temp->siguiente;
        }
        
        string json = "{\"total\":" + to_string(total) + ",";
        json += "\"aprobados\":" + to_string(aprobados) + ",";
        json += "\"reprobados\":" + to_string(total - aprobados) + "}";
        
        return json;
    }

    void procesarReprobados() {
        pilaReprobados = Pila();
        NodoEstudiante* temp = listaEstudiantes;
        
        while (temp != nullptr) {
            float prom = calcularPromedio(temp);
            if (prom < 3.0) {
                pilaReprobados.push(temp->nombre, temp->codigo, prom);
            }
            temp = temp->siguiente;
        }
    }

    string getReprobadosJSON() {
        procesarReprobados();
        return pilaReprobados.toJSON();
    }

    string desapilarReprobado() {
        string nombre, codigo;
        float promedio;
        
        if (pilaReprobados.pop(nombre, codigo, promedio)) {
            ostringstream ss;
            ss << fixed << setprecision(2) << promedio;
            
            string json = "{\"success\":true,";
            json += "\"nombre\":\"" + nombre + "\",";
            json += "\"codigo\":\"" + codigo + "\",";
            json += "\"promedio\":" + ss.str() + "}";
            return json;
        }
        
        return "{\"success\":false,\"message\":\"Pila vacia\"}";
    }
};

SistemaAcademico sistema;

string createResponse(int code, string contentType, string body) {
    string response = "HTTP/1.1 " + to_string(code) + " OK\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type\r\n";
    response += "Content-Length: " + to_string(body.length()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += body;
    return response;
}

string urlDecode(string str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
            result += char(value);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

void handleRequest(int clientSocket, string request) {
    string method, path, body;
    
    size_t methodEnd = request.find(' ');
    method = request.substr(0, methodEnd);
    
    size_t pathEnd = request.find(' ', methodEnd + 1);
    path = request.substr(methodEnd + 1, pathEnd - methodEnd - 1);
    
    size_t bodyStart = request.find("\r\n\r\n");
    if (bodyStart != string::npos) {
        body = request.substr(bodyStart + 4);
    }
    
    string response;
    
    if (method == "OPTIONS") {
        response = createResponse(200, "text/plain", "OK");
    }
    else if (path == "/api/cola" && method == "GET") {
        response = createResponse(200, "application/json", sistema.getColaJSON());
    }
    else if (path == "/api/cola" && method == "POST") {
        size_t nombrePos = body.find("nombre=");
        size_t codigoPos = body.find("&codigo=");
        
        if (nombrePos != string::npos && codigoPos != string::npos) {
            string nombre = urlDecode(body.substr(nombrePos + 7, codigoPos - nombrePos - 7));
            string codigo = urlDecode(body.substr(codigoPos + 8));
            
            sistema.encolarEstudiante(nombre, codigo);
            response = createResponse(200, "application/json", "{\"success\":true}");
        } else {
            response = createResponse(400, "application/json", "{\"error\":\"Invalid data\"}");
        }
    }
    else if (path == "/api/estudiantes" && method == "GET") {
        response = createResponse(200, "application/json", sistema.getEstudiantesJSON());
    }
    else if (path == "/api/estadisticas" && method == "GET") {
        response = createResponse(200, "application/json", sistema.getEstadisticasJSON());
    }
    else if (path == "/api/reprobados" && method == "GET") {
        response = createResponse(200, "application/json", sistema.getReprobadosJSON());
    }
    else if (path == "/api/reprobados/pop" && method == "POST") {
        response = createResponse(200, "application/json", sistema.desapilarReprobado());
    }
    else {
        response = createResponse(404, "text/plain", "Not Found");
    }
    
    send(clientSocket, response.c_str(), response.length(), 0);
}

int main() {
    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    
    cout << "Servidor HTTP iniciado en http://localhost:8080" << endl;
    cout << "Endpoints disponibles:" << endl;
    cout << "  GET  /api/cola" << endl;
    cout << "  POST /api/cola" << endl;
    cout << "  GET  /api/estudiantes" << endl;
    cout << "  GET  /api/estadisticas" << endl;
    cout << "  GET  /api/reprobados (carga pila con PUSH)" << endl;
    cout << "  POST /api/reprobados/pop (desapila con POP)" << endl;
    cout << "\nEsperando conexiones..." << endl;
    
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        
        char buffer[4096] = {0};
        recv(clientSocket, buffer, 4096, 0);
        
        handleRequest(clientSocket, string(buffer));
        
        #ifdef _WIN32
        closesocket(clientSocket);
        #else
        close(clientSocket);
        #endif
    }
    
    #ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
    #else
    close(serverSocket);
    #endif
    
    return 0;
}

