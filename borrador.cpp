#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>

struct NodoExpresion {
    std::string valor;
    NodoExpresion* izquierda;
    NodoExpresion* derecha;

    NodoExpresion(const std::string& val) : valor(val), izquierda(nullptr), derecha(nullptr) {}
};

bool esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int prioridad(char operador) {
    if (operador == '^') {
        return 3;
    } else if (operador == '*' || operador == '/') {
        return 2;
    } else if (operador == '+' || operador == '-') {
        return 1;
    }
    return 0;
}

std::string notacionFijaAPostfija(const std::string& expresion) {
    std::stack<char> pila;
    std::istringstream entrada(expresion);
    std::string salida;
    char elemento;

    while (entrada >> elemento) {
        if (isdigit(elemento) || (elemento == '.' && isdigit(entrada.peek()))) {
            salida += elemento;
        } else if (elemento == 'x') {
            salida += "x";
        } else if (esOperador(elemento)) {
            salida += ' ';
            while (!pila.empty() && esOperador(pila.top()) &&
                   prioridad(pila.top()) >= prioridad(elemento)) {
                salida += pila.top();
                salida += ' ';
                pila.pop();
            }
            pila.push(elemento);
        } else if (elemento == '(') {
            pila.push(elemento);
        } else if (elemento == ')') {
            while (!pila.empty() && pila.top() != '(') {
                salida += ' ';
                salida += pila.top();
                pila.pop();
            }
            if (!pila.empty() && pila.top() == '(') {
                pila.pop();
            }
        }
    }

    while (!pila.empty()) {
        salida += ' ';
        salida += pila.top();
        pila.pop();
    }

    return salida;
}

NodoExpresion* construirArbol(const std::string& expresion) {
    std::stack<NodoExpresion*> pila;
    std::istringstream entrada(expresion);
    std::string elemento;

    while (entrada >> elemento) {
        if (isdigit(elemento[0]) || (elemento[0] == '.' && isdigit(elemento[1])) || elemento == "x") {
            NodoExpresion* nodo = new NodoExpresion(elemento);
            pila.push(nodo);
        } else if (esOperador(elemento[0])) {
            NodoExpresion* nodo = new NodoExpresion(elemento);
            nodo->derecha = pila.top();
            pila.pop();
            nodo->izquierda = pila.top();
            pila.pop();
            pila.push(nodo);
        }
    }

    if (!pila.empty()) {
        return pila.top();
    } else {
        return nullptr;
    }
}

void imprimirArbol(NodoExpresion* raiz, int nivel) {
    if (raiz) {
        for (int i = 0; i < nivel; i++) {
            std::cout << "$ ";
        }
        std::cout << "--- " << raiz->valor << std::endl;

        if (raiz->izquierda || raiz->derecha) {
            imprimirArbol(raiz->izquierda, nivel + 1);
            imprimirArbol(raiz->derecha, nivel + 1);
        }
    }
}

double resolverArbol(NodoExpresion* raiz, const std::map<std::string, double>& variables) {
    if (!raiz) {
        return 0.0; // Valor predeterminado
    }

    if (isdigit(raiz->valor[0]) || (raiz->valor[0] == '.' && isdigit(raiz->valor[1]))) {
        return std::stod(raiz->valor);
    } else if (raiz->valor == "x") {
        return variables.at("x");
    } else if (esOperador(raiz->valor[0])) {
        double izquierda = resolverArbol(raiz->izquierda, variables);
        double derecha = resolverArbol(raiz->derecha, variables);

        if (raiz->valor == "+") {
            return izquierda + derecha;
        } else if (raiz->valor == "-") {
            return izquierda - derecha;
        } else if (raiz->valor == "*") {
            return izquierda * derecha;
        } else if (raiz->valor == "/") {
            return izquierda / derecha;
        } else if (raiz->valor == "^") {
            return std::pow(izquierda, derecha);
        }
    }

    return 0.0; // Valor predeterminado
}

int main() {
    std::map<std::string, double> variables;
    double ans = 0.0; // Inicializar 'ans' a un valor predeterminado (puede ser 0).

    std::string expresion;

    std::cout << "¡Bienvenido a la calculadora interactiva!" << std::endl;

    while (true) {
        std::cout << "Ingrese una expresión: ";
        std::getline(std::cin, expresion);

        if (expresion == "FIN") {
            break;
        }

        if (expresion.find('=') != std::string::npos) {
            // Asignar valor a una variable
            std::istringstream input(expresion);
            std::string variable_name;
            char equals_sign;
            double variable_value;
            input >> variable_name >> equals_sign >> variable_value;
            if (equals_sign == '=') {
                variables[variable_name] = variable_value;
                std::cout << "Variable '" << variable_name << "' asignada como " << variable_value << std::endl;
            } else {
                std::cout << "Error en la asignación de variable. Use 'variable = valor'." << std::endl;
            }
        } else {
            // Calcular expresión
            // Reemplaza 'ans' con su valor actual
            std::string expresion_con_ans = expresion;
            size_t pos = expresion_con_ans.find("ans");
            while (pos != std::string::npos) {
                expresion_con_ans.replace(pos, 3, std::to_string(ans)); // Reemplaza 'ans' por su valor actual
                pos = expresion_con_ans.find("ans", pos + 1);
            }

            try {
                double result = resolverArbol(construirArbol(expresion_con_ans), variables);
                ans = result; // Actualiza el valor de 'ans'
                std::cout << "Resultado: " << result << std::endl;

                // Construye y muestra el árbol de la última expresión evaluada
                NodoExpresion* arbol = construirArbol(expresion_con_ans);
                if (arbol) {
                    std::cout << "Árbol de expresión:" << std::endl;
                    imprimirArbol(arbol, 0);
                } else {
                    std::cout << "Error: Expresión no válida." << std::endl;
                }
            } catch (const std::runtime_error& e) {
                std::cout << "Error: Expresión no válida." << std::endl;
            }
        }
    }

    std::cout << "¡Hasta luego!" << std::endl;

    return 0;
}
