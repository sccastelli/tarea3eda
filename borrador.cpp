#include <iostream>
#include <stack>
#include <string>
#include <sstream>

bool esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int prioridad(char operador) {
    if (operador == '+' || operador == '-') {
        return 1;
    } else if (operador == '*' || operador == '/') {
        return 2;
    }
    return 0;
}

std::string notacionFijaAPostfija(const std::string& expresion) {
    std::stack<char> pila;
    std::istringstream entrada(expresion);
    std::string salida;
    char elemento;

    while (entrada >> elemento) {
        if (isdigit(elemento)) {
            salida += elemento;
            salida += ' ';
        } else if (esOperador(elemento)) {
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
                salida += pila.top();
                salida += ' ';
                pila.pop();
            }
            if (!pila.empty() && pila.top() == '(') {
                pila.pop();
            }
        }
    }

    while (!pila.empty()) {
        salida += pila.top();
        salida += ' ';
        pila.pop();
    }

    return salida;
}

int main() {
    std::string expresion;
    std::cout << "Ingresa una expresión en notación fija (con espacios entre elementos): ";
    std::getline(std::cin, expresion);

    std::string postfija = notacionFijaAPostfija(expresion);
    std::cout << "Expresión en notación postfija: " << postfija << std::endl;

    return 0;
}