/*
     @author: Qurbonzoda A. A.
     @project: simpleCalc, HW1
     @language: C++
     ITMO University
     Open Source


    The calculator supports brackets

    Priorities:
        First:  "(", ")"
        Second: "/", "*", "%"
        Third:  "+", "-"

    Negative sign should be separately from the number e.g  "- 3 + 4" or "1 + 3 - 4 * ( - 3 )"
*/

#include <iostream>
#include <algorithm>
#include <stack>

using namespace std;
const int MAXLENGTH = 9; //The max length of valid integer

/** I'm just declaring functions here. The bodies of functions are below "int main(){}" */

void Operate (stack<int> &numbers, stack<char> &operations, string &value); /* Depending on
    "string &value"'s value implements "stack<char> &operations"'s operations to
    "stack<int> &numbers"'s numbers and pushes "string &value" to "stack<char> &operations" */
void DoOperation (stack<int> &numbers, stack<char> &operations); /* Implements "stack<char> &operations"'s
    operations to "stack<int> &numbers"'s numbers*/
bool ValidityCheck (string &value, string &lastValue); // checks if "string &value" may come after "string &lastValue"
bool IsOperator (string value); // checks if "string &value" is an operator
int ToInt (string &value); // returns integer value of "string &value"
void Terminate (const string &value); // informs that the given expression is invalid and exits

string lastValue = "#"; // the last value before the current value

int Calc (string &expression, int &start, bool inBracket) { /* the recursion witch returns result of
                            expression inside brackets () and the whole expression. boolean inBracket
                            is true if Calc is calculating inside brackets.*/
    int a;
    string value;
    stack<int> numbers;
    stack<char> operations;
    for (int i = start; i < expression.length(); i++) {
        int j = expression.find(' ', i);
        if (j < 0) {
            j = expression.length();
        }
        value = expression.substr(i, j - i);
        if ((lastValue == "#" || lastValue == "(") && value == "-") {
            numbers.push(0);
            lastValue = "0";
        }
        if (!ValidityCheck(value, lastValue)) {
            Terminate(value);
        }
        //cout << "value -> " << value << endl;
        if (value == "(") {
            start = i + 2;
            lastValue = value;
            a = Calc(expression, start, true);
            numbers.push(a);
            i = start - 1;
        }
        else if (value == ")") {
            if (!inBracket) {
                cout<<"Brackets placed incorrectly"<<endl;
                exit(0);
            }
            start = i + 2;
            while (!operations.empty()) {
                DoOperation(numbers, operations);
            }
            lastValue = value;
            return numbers.top();
        }
        else {
            Operate(numbers, operations, value);
            lastValue = value;
            i = j;
        }
    }
    if (inBracket) {
        cout<<"Brackets placed incorrectly"<<endl;
        exit(0);
    }
    while (!operations.empty()) {
        if (numbers.size() < 2) {
            Terminate("");
        }
        DoOperation(numbers, operations);
    }
    return numbers.top();
}

int main() { /* the main function - the start. reads the expression and calls function
                Calc and writes the result if the expression is valid*/
    //freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
    string expression;
    int start = 0;
    cout << "Please enter an expression. All symbols should be separated by single space except digits of a number" << endl;
    cout << "\"- 335 + 4\" and \"1 + 37 - 41 * ( - 3 )\" are examples of valid expression" << endl;
    getline(cin, expression);
    cout<<Calc (expression, start, false);
    return 0;
}

void Operate (stack<int> &numbers, stack<char> &operations, string &value){
    if (value == "+" || value == "-") {
        while (!operations.empty()) {
            DoOperation(numbers, operations);
        }
        if (value == "+") {
            operations.push('+');
        }
        else {
            operations.push('-');
        }
    }

    else if (value == "/" || value == "*" || value == "%") {
        while (!operations.empty()) {
            if (operations.top() == '+' || operations.top() == '-') {
                break;
            }
            DoOperation(numbers, operations);
        }
        if (value == "/") {
            operations.push('/');
        }
        else if (value == "*") {
            operations.push('*');
        }
        else {
            operations.push('%');
        }
    }
    else {
        numbers.push(ToInt(value));
    }
}

int ToInt (string &value) {
    int res = 0;
    for (int i = 0; i < value.length(); i++) {
        res *= 10;
        res += value[i] - 48;
    }
    return res;
}

bool ValidityCheck (string &value, string &lastValue) {
    if (IsOperator(value)) {
        if (lastValue == "#" || (lastValue != ")" && !isdigit(lastValue[0])))
            return false;
    }
    else if (value == "(") {
        if (lastValue != "#" && lastValue != "(" && !IsOperator(lastValue))
            return false;
    }
    else if (value == ")") {
        if (lastValue != ")" && !isdigit(lastValue[0]))
            return false;
    }
    else {
        if (value.length() > MAXLENGTH) {
            return false;
        }
        for (int i = 0; i < value.length(); i++) {
            if (!isdigit(value[i])) {
                return false;
            }
        }
        if (lastValue != "#" && !IsOperator(lastValue) && lastValue != "(") {
            return false;
        }
    }
    return true;
}

void Terminate (const string &value) {
    cout<<"Invalid expression: " << value <<endl;
    exit(0);
}

bool IsOperator (string value) {
    if (value == "+" || value == "-" || value == "%" || value == "/" || value == "*") {
        return true;
    }
    return false;
}

void DoOperation (stack<int> &numbers, stack<char> &operations) {
    int b = numbers.top();
    numbers.pop();
    int a = numbers.top();
    numbers.pop();
    char this_operator = operations.top();
    operations.pop();

    if (this_operator == '-') {
        numbers.push(a - b);
    }
    else if (this_operator == '+') {
        numbers.push(a + b);
    }
    else if (this_operator == '%') {
        if (b == 0) {
            cout<<"Error: 'dividend % 0' => Division by 0"<<endl;
            exit(0);
        }
        numbers.push(a % b);
    }
    else if (this_operator == '/') {
        if (b == 0) {
            cout<<"Error: Division by 0"<<endl;
            exit(0);
        }
        numbers.push(a / b);
    }
    else {
        numbers.push(a * b);
    }
}
