//
//  main.cpp
//  MathsParser
//
//  Created by Saloni Sharma on 29/04/2018.
//  Copyright © 2018 Saloni Sharma. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <list>
#include <iterator>
#include <ctype.h>
#include<fstream>

using namespace std;

inline bool isLeftBracket(string str)
{
    return str == "(" ? true : false;
}

inline bool isRightBracket(string str)
{
    return str == ")" ? true : false;
}

inline bool isOperator(string str)
{
    if(str == "+" || str == "-" || str == "*" || str == "/")
    {
        return true;
    }
    return false;
}

inline bool isInteger(string str)
{
    int num = atoi(str.c_str());
    if (num == 0 && str[0] != '0')
    {
        return false;
    }
    return true;
}

// Simple math operations to perform calcuation
inline int eval(string a, string b, string o)
{
    if( o == "+"){
        return stoi(a) + stoi(b);
    }
    else if( o == "-"){
        return stoi(a) - stoi(b);
    }
    else if( o == "*"){
        return stoi(a) * stoi(b);
    }
    else if( o == "/"){
        return stoi(a) / stoi(b);
    }
    else{
        return 0;
    }
}

// Evaluate the reverse post fix expression and return the final result of the expression
// The checking for a valid expression should be done before calling this evalRPN function
// other than the devision by '0'
// By the time we reach here we are sure that the expression is a valid expression
int evalRPN(std::list<string>& rpf)
{
    int result = stoi(*(rpf.begin()));
    for(auto i = std::next(rpf.begin()); i != std::prev(rpf.end());){
        if(isOperator(*(std::next(i)))){
            if(*i == "0" && *(std::next(i)) == "/"){
                cout << "INVALID Expresssion: division by zero\n";
                result = -1;
                break;
            }
            result = eval(*(std::prev(i)), *i, *(std::next(i)));
            *(std::prev(i, 1)) = to_string(result);
            rpf.erase(i++);
            rpf.erase(i++);
            i = std::prev(i);
        }
        else{
            ++i;
        }
    }
    return result;
}

// Read the raw expression string, remove white spaces and do validity check for characters and order of operators
bool readExpressionInput(string expression, vector<string>& processedExp)
{
    // Remove white spaces
    expression.erase(std::remove(expression.begin(), expression.end(), ' '),
                     expression.end());
    
    for(int i = 0; i < expression.size(); ++i){
        string toInput;
        toInput +=  expression[i];
        if(isdigit(expression[i])){
            while(isdigit(expression[i+1])){
                toInput = toInput + expression[i+1];
                ++i;
            }
        }
        // First element can't be an operator
        if (processedExp.empty() && isOperator(toInput)) {
            cout << "INVALID: first element is operator\n";
            return false;
        }
        if(!isOperator(toInput) && !isLeftBracket(toInput) && !isRightBracket(toInput) && !isInteger(toInput)){
            cout << "INVALID Input type\n";
            return false;
        }
        processedExp.push_back(toInput);
    }
    
    // Validity checks for order of operators and braces
    for(int i = 0; i < processedExp.size() - 1; ++i)
    {
        if(isOperator(processedExp[i]) && isOperator(processedExp[i+1]))
        {
            cout << "INVALID: consecutive operators'n";
            return false;
        }
        if(isInteger(processedExp[i]) && isLeftBracket(processedExp[i+1]))
        {
            cout << "INVALID Expression: no operator between open brace and number\n";
            return false;
        }
    }
    return true;
}

// this is the main function which reads the expression, removes all white spaces and
// validates that all elements passed in are valid and in order and the creates the reverse post fix from infix expression
// then evaluates the expression and sends back the result
bool evaluate(string& expression, int& result)
{
    vector<string> mathsExp;
    
    //check for consecutive operator and open bracket after integer
    bool isValid = readExpressionInput(expression, mathsExp);
    if(!isValid)
    {
        return false;
    }
    
    stack<string> operatorStack;
    list<string> outputList;
    
    //main loop for generating output to be evaluated from left to right
    for(int  i = 0; i < mathsExp.size(); ++i)
    {
        string element = mathsExp[i];
        
        if (isLeftBracket(mathsExp[i])){
            operatorStack.push(element);
        }
        else if(isRightBracket(mathsExp[i])){
            if (operatorStack.empty()) {
                cout << "INVALID Braces sequence\n";
                return false;
            }
            while(!isLeftBracket(operatorStack.top())){
                if(!operatorStack.empty()){
                    outputList.push_back(operatorStack.top());
                    operatorStack.pop();
                    if (operatorStack.empty()) {
                        cout << "INVALID Braces sequence\n";
                        return false;
                    }
                }
            }
            // pop out '('
            operatorStack.pop();
            // get the operator just before the '('
            if(!operatorStack.empty() && isOperator(operatorStack.top())){
                outputList.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }
        else if (isOperator(mathsExp[i])){
            operatorStack.push(element);
        }
        else if(isInteger(mathsExp[i])){
            outputList.push_back(element);
            if(!operatorStack.empty() && isOperator(operatorStack.top())){
                outputList.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }
    } // end of for loop
    
    //check for equal and well positioned brackets
    if(!operatorStack.empty()){
        cout << "INVALID Braces sequence\n";
        return false;
    }
    
    // Now evaluate the reverse post fix expression
    result = evalRPN(outputList);
    if(result == -1)
    {
        return false;
    }
    
    return true;
}

void printOutput(bool isValid, int result) {
    if(isValid)
    {
        cout << "Result :" << result << " Return Code : true" << endl;
    }
    else
    {
        cout << "Result :N/A" << " Return Code : false" << endl;
    }
    cout << endl;
}

int main(int argc, const char * argv[]) {
    bool isValid = false;
    int result = 0;
    string expression;
    
    cout << "This calculator outputs only integer value\n";
    cout << "Enter a valid test file (full path)" << endl;
    string filepath;
    cin >> filepath;
    
    std::ifstream myfile(filepath);
    if (myfile.is_open())
    {
        while (getline(myfile, expression) && !myfile.eof()) {
            cout << expression << endl;
            isValid = evaluate(expression, result);
            printOutput(isValid, result);
        }
        cout << "file opened"<< endl;
        myfile.close();
    }
    else {
        cout << "Unable to open file";
        cout << "Enter the expression to evaluate manually" << endl;
        cin >> expression;
        
        isValid = evaluate(expression, result);
        printOutput(isValid, result);
    }
    
    return 0;
}
