
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;


enum State{ //for variable
    STAGE_0 = 0,
    STAGE_1 = 1, //check for datatype
    STAGE_2 = 2, //check for variable name
    STAGE_3 = 3, //if equal - check if it is an identifier, comma - go back to stage 2, or semicolon - go back to stage 1
};

enum TokenType {
    START,      //0
    DATATYPE,   //1
    VARNAME,    //2
    SPACE,      //3
    COMMA,      //4
    SEMICOLON,  //5
    EQUAL_SIGN, //6
    UNEXPECTED, //7
};

class Token {
public:
    string identifier; // for example int
    TokenType token_type = START; // Specify the type for TokenType
    State token_state = STAGE_1; //
    
};

// bool isIdentifier(const std::string &str) {
//     if (!str.empty() && (str[0] == '_' || isalpha(str[0]))) {
//         for (char c : str.substr(1)) {
//             if (!isalnum(c) && c != '_') {
//                 return false;
//             }
//         }
//         return true; // All characters were checked
//     }
//     return false;
// }

bool isIdentifier(const std::string &str) {
    if (!str.empty() && (str[0] == '_' || isalnum(str[0]))) {
        for (char c : str.substr(1)) {
            if (!isalnum(c) && c != '_') {
                return false;
            }
        }
        return true; // All characters were checked
    }
    return false;
}


int getType(string str) {
    if (str == "int" || str == "char" || str == "double" || str == "float") {
        return DATATYPE;
    } else if (isIdentifier(str))
        return VARNAME;
    else if (str == ",") {
        return COMMA;
    } else if (str == ";") {
        return SEMICOLON;
    } else if (str == "=") {
        return EQUAL_SIGN; // check if it the var_init is ok and if it is ok then assign it to a var_init : means variable initialization
    } else if(str == " "){
        return SPACE;
    }else
        return UNEXPECTED; //CAN BE VAR.INIT
    
}

void tokenize(const string& input, vector<string>& tokens, const string& delimiters) {
    size_t start = 0;
    size_t end = input.find_first_of(delimiters);

    while (end != string::npos) {
        // Extract the token and add it to the vector
        string token = input.substr(start, end - start);
        if (!token.empty()) {
            tokens.push_back(token);
        }

        // Extract the delimiter and add it to the vector
        string delimiter = input.substr(end, 1);
        tokens.push_back(delimiter);

        // Move to the next token (skip delimiters)
        start = end + 1;
        while (start < input.length() && delimiters.find(input[start]) != string::npos) {
            start++;
        }

        // Find the next delimiter
        end = input.find_first_of(delimiters, start);
    }

    // Add the last token
    string lastToken = input.substr(start);
    if (!lastToken.empty()) {
        tokens.push_back(lastToken);
    }
}


bool check_Variable(const vector<string>& parsed_string, vector<Token>& tokens) {
    
    Token t;
    
    //pushed the identifier first 
    for (const auto& identifier : parsed_string) {
        t.identifier = identifier;
        // t.token_type = DATATYPE; not yet here since we want to have a switch case to get it type each
        tokens.push_back(t);
    }
    
    //then pushed the tokentype 
    for (auto& token : tokens) { 
        int inputType = getType(token.identifier);
        token.token_type = static_cast<TokenType>(inputType);
        
        // cout << token.identifier << " , " << token.token_type << endl;
        
    }
    
    //for debugging purposes:
    
    for (auto& token : tokens) { 
        switch (token.token_type) {
            case DATATYPE:
                cout << "DATATYPE: " << token.identifier << endl;
                break;
            case VARNAME:
                cout << "VARNAME: " << token.identifier << endl;
                break;
            case SPACE:
                cout << "SPACE: " << token.identifier << endl;
                break;
            case COMMA:
                cout << "COMMA: " << token.identifier << endl;
                break;
            case SEMICOLON:
                cout << "SEMICOLON: " << token.identifier << endl;
                break;
            case EQUAL_SIGN:
                cout << "EQUAL_SIGN: " << token.identifier << endl;
                break;
            case UNEXPECTED:
                cout << "UNEXPECTED: " << token.identifier << endl;
                break;
            default:
                break;
        }
    }
    
    State currentState = STAGE_1;

    for (auto& token : tokens) { 

            if(token.token_type == SPACE){
                //do nothing
                //cout<<"hello"<<endl;
            }if(currentState == STAGE_3){
                currentState = STAGE_1;
            }else if(currentState == STAGE_1 && token.token_type == DATATYPE){
                //cout<<"hello"<<endl;
                currentState = STAGE_1;
            }else if(currentState == STAGE_1 && token.token_type == VARNAME){
                //cout<<"hello"<<endl;
                currentState = STAGE_2;
            }else if(currentState == STAGE_2 && token.token_type == COMMA){
                currentState = STAGE_1;
            }else if(currentState == STAGE_2 && token.token_type == SEMICOLON){
                currentState = STAGE_3;
            }else if(currentState == STAGE_2 && token.token_type == EQUAL_SIGN){
                //2 cases that are valid: 
                //1. value assigned is a declared variable
                //2. value assigned is related to the datatype
                currentState = STAGE_0; //means we have to check first if the initialize valus is valid
        
            }
            else if(currentState == STAGE_0 && token.token_type == VARNAME || token.token_type == UNEXPECTED){
                

            }
    }

    cout<<"state: "<<currentState<<endl;

    // Check if the last state is STAGE_3
    return currentState == STAGE_3;
    
    
}


int main() {
    int cases;
    cin >> cases;

    while (cases--) {
        string inputString;

        int testType;
        cin >> testType;
        cin.ignore();

        getline(cin, inputString);
        vector<string> parsed_string;

        // Define a set of delimiters
        string delimiters = ",; =";

        // Tokenize the string using the custom function
        tokenize(inputString, parsed_string, delimiters);

        // Display the tokens and delimiters
        cout << "parsed string: ";
        for (const auto& individual_string : parsed_string)
            cout << "[" << individual_string << "] ";

        cout << endl;

        vector<Token> tokens;
        
        //ASSEMBLIN THE TOKENS WITH IDENTIFIER AND TOKENTYPE
        
        // for (const auto& identifier : parsed_string) {
        //     Token t;
        //     t.identifier = identifier;
        //     t.token_type = DATATYPE;
        //     Tokens.push_back(t);
        // }
        
        
        // // Print the identifiers and their token types
        // cout << "Identifiers and Token Types:" << endl;
        // for (const auto& token : tokens) {
        //     cout << "Identifier: " << token.identifier << "     , TokenType: " << token.token_type << endl;
        // }
        

        if (testType == 1) {
            if (check_Variable(parsed_string, tokens)) {
                cout << "VALID VARIABLE DECLARATION" << endl;
            } else {
                cout << "INVALID VARIABLE DECLARATION" << endl;
            }
        }
    }

    return 0;
}
