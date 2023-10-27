#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <map>
#include <cmath>

using namespace std;

// Función para evaluar una expresión en notación posfija (RPN)
double evaluateRPN(const string& expression) {
    stack<double> stack;
    istringstream iss(expression);
    string token;
    
    while (iss >> token) {
        if (token == "+") {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a + b);
        } else if (token == "-") {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a - b);
        } else if (token == "*") {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a * b);
        } else if (token == "/") {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a / b);
        } else if (token == "^") {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(pow(a, b));
        } else if (token == "ans") {
            stack.push(ans);
        } else if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            stack.push(stod(token));
        } else {
            // Variable no válida
            cerr << "Variable no válida: " << token << endl;
            return 0.0;
        }
    }
    
    return stack.top();
}

int main() {
    map<string, double> variables;  // Diccionario para almacenar variables
    double ans = 0.0;  // Variable para almacenar el resultado de la última operación

    cout << "CALCULADORA INTERACTIVA" << endl;
    
    while (true) {
        cout << "Ingrese una expresión (x = 5, x + 1, x + x * 2 + ans, ans = 25, FIN para salir): ";
        string input;
        getline(cin, input);
        
        if (input == "FIN") {
            break;
        } else if (input.find('=') != string::npos) {
            // Asignación de variable
            string varName;
            double varValue;
            istringstream varStream(input);
            varStream >> varName >> varValue;

            variables[varName] = varValue;
        } else {
            // Evaluación de expresión en notación posfija (RPN)
            double result = evaluateRPN(input);
            cout << "Resultado: " << result << endl;
            ans = result;
        }
    }
    
    cout << "Adiós!" << endl;
    
    return 0;
}
