#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include "trees/include/trees/tree.hpp"

// Estructura de Nodo que incluye datos, hijos izquierdo y derecho, y representación.
struct Nodo {
    char operador;
    double valor;
    Nodo* izquierda;
    Nodo* derecha;
    std::string representacion;

    // Constructor para nodos de operador.
    Nodo(char op, Nodo* left, Nodo* right, const std::string& rep)
        : operador(op), izquierda(left), derecha(right), valor(0), representacion(rep) {}

    // Constructor para nodos de valor.
    Nodo(double val, const std::string& rep)
        : operador(' '), izquierda(nullptr), derecha(nullptr), valor(val), representacion(rep) {}
};

// Función para verificar si un carácter es un operador.
bool esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Función para asignar prioridad a los operadores.
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

// Función para convertir una expresión en notación fija a notación postfija.
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

// Función para resolver una expresión en notación postfija.
double resolverPostfija(const std::string& expresion, const std::map<std::string, double>& variables) {
    std::stack<double> pila;
    std::istringstream entrada(expresion);
    std::string elemento;

    while (entrada >> elemento) {
        if (isdigit(elemento[0]) || (elemento[0] == '.' && isdigit(elemento[1]))) {
            pila.push(std::stod(elemento));
        } else if (variables.find(elemento) != variables.end()) {
            pila.push(variables.at(elemento));
        } else if (elemento == "x") {
            pila.push(variables.at("x"));
        } else if (esOperador(elemento[0])) {
            double operand2 = pila.top();
            pila.pop();
            double operand1 = pila.top();
            pila.pop();
            double result;

            switch (elemento[0]) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
                case '^':
                    result = std::pow(operand1, operand2);
                    break;
                default:
                    // Handle any other operators if needed.
                    break;
            }

            pila.push(result);
        }
    }

    if (pila.size() == 1) {
        return pila.top();
    } else {
        // Handle error, expression not properly formatted.
        throw std::runtime_error("Invalid expression.");
    }
}

// Función para construir un árbol de expresión a partir de una expresión en notación postfija.
Nodo* construirArbolExpresion(const std::string& expresion) {
    std::stack<Nodo*> pila;
    std::istringstream entrada(expresion);
    std::string elemento;

    while (entrada >> elemento) {
        if (isdigit(elemento[0]) || (elemento[0] == '.' && isdigit(elemento[1])) || elemento == "x") {
            // Si el elemento es un número, un punto o 'x', crea un nodo de valor.
            if (elemento == "x") {
                pila.push(new Nodo('x', nullptr, nullptr, elemento));
            } else {
                double valor = std::stod(elemento);
                pila.push(new Nodo(valor, nullptr, nullptr, elemento));
            }
        } else if (esOperador(elemento[0])) {
            // Si el elemento es un operador, crea un nodo de operador y pop dos nodos de la pila.
            if (pila.size() < 2) {
                throw std::runtime_error("Error en la expresión. Revise la notación postfija.");
            }
            Nodo* operand2 = pila.top();
            pila.pop();
            Nodo* operand1 = pila.top();
            pila.pop();
            pila.push(new Nodo(elemento[0], operand1, operand2, "(" + operand1->representacion + elemento + operand2->representacion + ")"));
        }
    }

    // Al final, la pila debe contener un único nodo que representa toda la expresión.
    if (pila.size() != 1) {
        throw std::runtime_error("Error en la expresión. Revise la notación postfija.");
    }

    return pila.top();
}

// Función para mostrar el árbol de expresiones utilizando traverse_rec.
void mostrarArbolExpresion(Nodo* arbolExpresion) {
    if (arbolExpresion) {
        std::cout << "Árbol de Expresión:" << std::endl;
        traverse_rec(arbolExpresion, 0);
    }
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

            std::string postfix_expression = notacionFijaAPostfija(expresion_con_ans);
            try {
                double result = resolverPostfija(postfix_expression, variables);
                ans = result; // Actualiza el valor de 'ans'
                std::cout << "Resultado: " << result << std::endl;
            } catch (const std::runtime_error& e) {
                std::cout << "Error: Expresión no válida." << std::endl;
            }
        }
    }

    std::cout << "¡Hasta luego!" << std::endl;

    return 0;
}



int main() {
    std::map<std::string, double> variables;
    double ans = 0.0;
    Tree arbol; // Crear un objeto Tree

    std::string expresion;

    std::cout << "¡Bienvenido a la calculadora interactiva!" << std::endl;

    while (true) {
        std::cout << "Ingrese una expresión: ";
        std::getline(std::cin, expresion);

        if (expresion == "FIN") {
            break;
        }

        if (expresion == "TREE") {
            // Si el usuario ingresa "TREE", muestra el árbol
            if (arbol.getRoot() != nullptr) {
                std::cout << "Árbol de Expresión:" << std::endl;
                arbol.traverse(); // Ejecuta la función traverse para mostrar el árbol
            } else {
                std::cout << "El árbol está vacío. Ingrese una expresión primero." << std::endl;
            }
        } else if (expresion.find('=') != std::string::npos) {
            // ... (código para asignar valores a variables)
        } else {
            // Calcular expresión
            // Reemplaza 'ans' con su valor actual
            std::string expresion_con_ans = expresion;
            size_t pos = expresion_con_ans.find("ans");
            while (pos != std::string::npos) {
                expresion_con_ans.replace(pos, 3, std::to_string(ans)); // Reemplaza 'ans' por su valor actual
                pos = expresion_con_ans.find("ans", pos + 1);
            }

            std::string postfix_expression = notacionFijaAPostfija(expresion_con_ans);
            try {
                double result = resolverPostfija(postfix_expression, variables);
                ans = result; // Actualiza el valor de 'ans'
                std::cout << "Resultado: " << result << std::endl;
            } catch (const std::runtime_error& e) {
                std::cout << "Error: Expresión no válida." << std::endl;
            }
        }
    }

    std::cout << "¡Hasta luego!" << std::endl;

    return 0;
}
