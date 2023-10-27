#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>

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
