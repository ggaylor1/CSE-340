//Gyllian Gaylor 1214978271

// lexer.cc file

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = {"END_OF_FILE",
                     "IF", "WHILE", "DO", "THEN", "PRINT",
                     "PLUS", "MINUS", "DIV", "MULT",
                     "EQUAL", "COLON", "COMMA", "SEMICOLON",
                     "LBRAC", "RBRAC", "LPAREN", "RPAREN",
                     "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
                     "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM",
                     "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = {"IF", "WHILE", "DO", "THEN", "PRINT"};

void Token::Print() {
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer() {
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace() {
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s) {
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s) {
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber() {
    char c;

    //newly added scanning variables 
    char newVar, newVari, newVaria; 

    int flag8 = 0;

    int flag0 = 0;

    char arr[100];

    input.GetChar(c);

    if (isdigit(c)) 
        //checks if its a digit at char c
    {
        if (c == '0') 
        {
            tmp.lexeme = "0";
            //performs lexeme op flags 1
            flag0 = 1;
        } 
        
        else 
        {
            tmp.lexeme = ""; //else operation
            while (!input.EndOfInput() && isdigit(c)) 
            {
                tmp.lexeme += c;
                input.GetChar(c);
                if (c == '8' || c == '9') 
                {
                    flag8 = 1;
                    // if we get the char c = 8 or 9 then we set flag 8 equal to 1
                        //it incriments a flag that is initially at 0
                }
            }

            if (!input.EndOfInput()) 
            {
                input.UngetChar(c);
            }

        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        input.GetChar(newVar);//uses the references we created to check for x's and 0's and 8's
        if (newVar == '.') 
        {

            //if it finds the "." then it is an empty strin and getchar input can find newVari

            int flagnotzero = 0; 
            string newstring = "";
            input.GetChar(newVari);
            if (isdigit(newVari)) 
                //if newVari = digit while.... no input at end of input then the next statements run for a check
            {
                while (!input.EndOfInput() && isdigit(newVari)) 
                {
                    if (newVari != '0') 
                        //not equal to 0
                    {
                        flagnotzero = 1; 
                        //flag zero bool goes to 1
                    }
                    //newString is now added to newVari and its char is grabbed 
                    newstring += newVari; 
                    
                
                    input.GetChar(newVari);

                }
                if (!input.EndOfInput()) 
                {
                    input.UngetChar(newVari);
                }
            } else 
            {
                input.UngetChar(newVari);
            }
            //flags respectively 0 and 1
            if (flagnotzero == 0 && flag0 == 1) 
                
            {
                //ungets newstring after use
                input.UngetString(newstring); 
                

            } 
            else 
            {
                input.UngetString(newstring);
                input.GetChar(newVari);
                if (isdigit(newVari)) 
                {
                    tmp.lexeme += '.';
                    while (!input.EndOfInput() && (isdigit(newVari))) 
                    {
                        tmp.lexeme += newVari;
                        input.GetChar(newVari);
                    }
                    if (!input.EndOfInput()) 
                    {
                        input.UngetChar(newVari);
                    }
                    //similar functions are executed as before for token relations 
                    tmp.token_type = REALNUM; 
                    tmp.line_no = line_no;
                    return tmp;
                } 
                else 
                {
                    input.UngetChar(newVari);
                }
            }


            input.UngetChar(newVar);

            //x must be x in order to pass the tests
        } 
        //if equal to 'x' then we get thechar  of newvar2 and when newvar2 is zero then else if
        else if (newVar == 'x') /// where is a initiallized
        {
            input.GetChar(newVari);
            if (newVari == '0') 
            {
                //get char newvar3 then if that is 8 and current 8 flag is --->

                input.GetChar(newVaria); 
                if (newVaria == '8' && flag8 == 0) {

                    tmp.lexeme = tmp.lexeme + newVar + newVari + newVaria; 
                    //the following token commands are executed and the tmp returns
                    tmp.token_type = BASE08NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } 
                else 
                {
                    //do this bc it will unget the latest newvar1 char which is newvar3

                    input.UngetChar(newVaria); 
                }
                //newvari is ungot after newvaria

                input.UngetChar(newVari); 
            }  
            //if newvari is equal to 1 instead of zero and newvaria is equal to six instead of 
            else if (newVari == '1') 
            {
                input.GetChar(newVaria);
                if (newVaria == '6')
                {
                    //this performs functions (tokens) base 16 instead of 8

                    tmp.lexeme = tmp.lexeme + newVar + newVari + newVaria;
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } 
                else 
                {
                    input.UngetChar(newVaria);
                }
                input.UngetChar(newVari);
            } 
            else 
            {
                input.UngetChar(newVari);
            }

            input.UngetChar(newVar);
        } 
        //checking letters that are currently associated with IFS for newVar1
        else if (newVar == 'A' || 
                   newVar == 'B' ||
                   newVar == 'C' ||
                   newVar == 'D' ||
                   newVar == 'E' ||
                   newVar == 'F') 
                   //this checks for strings that are visible
        {
            int counter = 0;
            int newcount = 0;
            char arr[100];
            //creates and array with elements equal to the current count of 0 (empty)

            arr[counter] = newVar; 
            while (isdigit(arr[counter]) ||
                   (arr[counter] == 'A'
                    || arr[counter] == 'B'
                    || arr[counter] == 'C'
                    || arr[counter] == 'D'
                    || arr[counter] == 'E' || arr[counter] == 'F')) 
            {
                counter++;
                input.GetChar(arr[counter]);
            }

            newcount = counter;
            newVar = arr[counter];

            //x????? if it is equal to x then getchar at newvar 2 and 3 and check if it is equal to 16

            if (newVar == 'x') 
            {
                input.GetChar(newVari);
                if (newVari == '1') 
                {
                    input.GetChar(newVaria);
                    if (newVaria == '6') 
                    {
                        for (counter = 0; counter < newcount; counter++) 
                        {
                            tmp.lexeme = tmp.lexeme + arr[counter];
                        }
                        tmp.lexeme = tmp.lexeme + newVar + newVari + newVaria;
                        tmp.token_type = BASE16NUM; 
                        tmp.line_no = line_no;
                        return tmp;

                    } 
                    else 
                    {
                        input.UngetChar(newVaria); 
                        input.UngetChar(newVari);
                    }
                } 
                else 
                {
                    input.UngetChar(newVari); 
                }

                input.UngetChar(newVar); 
            } 
            else 
            {
                input.UngetChar(newVar);
            }

            counter--; 
            while (counter > -1) 
            { 
                input.UngetChar(arr[counter]);
                counter--;

            }

        } 
        else 
{
            input.UngetChar(newVar);
        }


        tmp.token_type = NUM;
        tmp.line_no = line_no;

        return tmp;
    } 
else 
{
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }

        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no; 

        return tmp; 
    }
}  //end of changed code

Token LexicalAnalyzer::ScanIdOrKeyword() {
    char c;
    input.GetChar(c);

    if (isalpha(c)) {

        tmp.lexeme = "";

        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);

        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}


//
TokenType LexicalAnalyzer::UngetToken(Token tok) {
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken() {
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input

    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);


    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main() {
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE) {
        token = lexer.GetToken();
        token.Print();
    }
}