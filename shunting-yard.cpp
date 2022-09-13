#include <iostream>
#include <cstring>
#include <stack>
#include <queue>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stack;
using std::queue;

bool isSymbol(const char& check);
bool isDigit(const char& check);
bool isDigit(const string& str);
const int priorityNumber(const char& symbol);
bool isOpenCloseMatch(const char& open, const char& close);
bool isOpenParen(const char& symbol);
bool isCloseParen(const char& symbol);

/* code by: Cristopher
    Simpple implemention of shunting yard algorithm. 
    This is not a fool proof implementation
*/



// check if the equation is balance (i.e. it has correct braces, bracket, or parenthesis)
bool isBalance(string equation)
{
    const int length = equation.length();
    int i;
    stack<char> symbol;
    for(i=0; i<length; i++)
    {
        if (isOpenParen(equation[i]))
            // put to stack
            symbol.push(equation[i]);
        else if (isCloseParen(equation[i]))
        {
            //comapare if not match return false
            if(symbol.empty())
                return false;
            if(!isOpenCloseMatch(symbol.top(), equation[i]))
                    return false;
                symbol.pop();
        }
    }
    if(symbol.size() == 0)
        return true;
}

// convert infix notation to postfix, using shunting yard algorithm
queue<string> infixToPostfix(const string equation)
{
    queue <string> parsed;
    stack<char> symbol;
    string buffer, tmp;
    const int length = equation.length();
    int i, priorityNum;
    for (i=0; i<length; i++){

        if (isDigit(equation[i]))
            buffer.push_back(equation[i]);
    
        else if (isSymbol(equation[i])){
            if (!buffer.empty()){
                parsed.push(buffer);
                buffer.clear();
            }

            priorityNum = priorityNumber(equation[i]);
            if(priorityNum == 0){
                symbol.push(equation[i]);
            }
            else if(priorityNum == 3){
                while (!symbol.empty()){
                    if(isOpenCloseMatch(symbol.top(), equation[i])){
                        symbol.pop();
                        break;
                    }
                    tmp.clear();
                    tmp = symbol.top();
                    symbol.pop();
                    parsed.push(tmp);    
                }
            }
            else{
                while (!symbol.empty()){
                    if(priorityNumber(symbol.top())<priorityNum)
                        break;
                    tmp.clear();
                    tmp = symbol.top();
                    symbol.pop();
                    parsed.push(tmp);    
                }
                symbol.push(equation[i]);
            }  
        }
    }

    //pop and push the remaining in symbol stack
    if (!buffer.empty()){
            parsed.push(buffer);
            buffer.clear();
        }
    while(!symbol.empty()){
        tmp.clear();
        tmp = symbol.top();
        symbol.pop();
        parsed.push(tmp);
    }
    return parsed;
}

double evalPostfix(queue<string> eq)
{
    stack<double> num;
    double left, right;
    string c;
    const int eqSize = eq.size();
    while(!eq.empty()){
        c = eq.front();
        eq.pop();   
        if(isDigit(c)){
            num.push(std::stod(c));
        }
        else{
            right = num.top();
            num.pop();
            left = num.top();
            num.pop();
            if (c[0] == '+') num.push(left+right);
            else if (c[0] == '-') num.push(left-right);
            else if (c[0] == '*') num.push(left*right);
            else if (c[0] == '/') num.push(left/right);
            //else if (op == '%') num.push(left%right);
        }
             
    }
    return num.top();
    /*else{
        cout<<"Error unbalance\n";
        exit(1);
    }*/
}

////////////////////////MAIN//////////////////////
int main(int argc, char **argv)
{
    int i;

    // checking if the equation has correct parenthesis
    if(isBalance(argv[1])){

        // for converting postfix to reverse polish
        queue<string> reversePolish = infixToPostfix(argv[1]);

        // evaluation
        cout<< evalPostfix(reversePolish)<< endl;
    }
    else
        cout<<"False\n";
}
/////////////////////////////////////////////////





bool isSymbol(const char& check)
{
    string symbolList = "()[]{}+-*/%";
    //check if the character is in the symbolList
    if(symbolList.find(check) != string::npos)
        return true;
    return false;
}

// check a character if it is digit
bool isDigit(const char& check)
{
    string digitList = "0123456789";
    //check if the character is in the symbolList
    if(digitList.find(check) != string::npos)
        return true;
    return false;
}

// check string if it is digit
bool isDigit(const string& str)
{
    int dotCount = 0;
    for (char const &c: str){
        if(!isdigit(c) && c != '.') return false;
        if(c =='.') dotCount++;
    }
    if (dotCount < 2) return true;
}

const int priorityNumber(const char& symbol)
{
    /*
        3 immediate )]}
        2 high      /*%
        1 low       + -
        0 wild      ([{
    */
    
    if (symbol == ')' || symbol == ']' || symbol == '}')
        return 3;
    if (symbol == '/' || symbol == '*' || symbol == '%')
        return 2;
    if (symbol == '-' || symbol == '+')
        return 1;
    if (symbol == '(' || symbol == '[' || symbol == '{')
        return 0;
}

//check if the braces, bracket or parenthesis matches
bool isOpenCloseMatch(const char& open, const char& close)
{
    if(open == '(' && close == ')')
        return true;
    if(open == '[' && close == ']')
        return true;
    if(open == '{' && close == '}')
        return true;
    return false;
}

// check if it is open parenthesis i.e "([{" 
bool isOpenParen(const char& symbol)
{
    if(symbol == '(' || symbol == '[' || symbol == '{')
        return true;
    return false;
}

// check if it is open parenthesis i.e ")]}" 
bool isCloseParen(const char& symbol)
{
    if(symbol == ')' || symbol == ']' || symbol == '}')
        return true;
    return false;
}