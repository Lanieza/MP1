//so take note that the int can be a number or single character char if and only if maconvert siya sa ascii
//so the char can be a number from 0 to 255 and then a single character char na ma qoute
//ang double or float kay any number basta usa ra kabuok ang dot

#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm> // Include the algorithm header for all_of used isidentifier function
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

// enum data_type{
//     INT, CHAR, FLOAT, DOUBLE,
// };

class Token {
public:
    string identifier; // for example int
    TokenType token_type = START; // Specify the type for TokenType
    State token_state = STAGE_1; //
    // string datatype = " ";

    
};

//----------------------------------------------------------------

bool isvalid_int(const std::string& str) {
    if (str.size() == 3 && str[0] == '\'' && str[2] == '\'') {
        int asciiValue = static_cast<int>(str[1]);
        return (asciiValue >= '0' && asciiValue <= '9');
    } else {
        // Try to convert the string to an integer
        try {
            size_t pos;
            int intValue = std::stoi(str, &pos);
            return pos == str.size(); // Check if the entire string was converted
        } catch (...) {
            return false; // Failed to convert to an integer
        }
    }
}

bool isvalid_float_double(const std::string &str) {
    try {
        std::size_t pos;
        std::stod(str, &pos);
        return pos == str.length();
    } catch (const std::invalid_argument &) {
        return false;
    } catch (const std::out_of_range &) {
        return false;
    }
}

bool isvalid_char(const std::string &str) {
    if (str.size() == 3 && str[0] == '\'' && str[2] == '\'') {
        char asciiChar = str[1];
        if (isdigit(asciiChar)) {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------


bool isIdentifier(const std::string &str) {
    if (!str.empty()) {
        // Check if the string is only a number
        if (all_of(str.begin(), str.end(), ::isdigit)) {
            return false;
        }

        // Check if the first character is an underscore or an alphabet
        if (str[0] == '_' || std::isalpha(str[0])) {
            // Check the rest of the characters
            for (char c : str.substr(1)) {
                if (!std::isalnum(c) && c != '_') {
                    return false;
                }
            }
            return true; // All characters were checked
        }
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

        // Move to the next token
        start = end + 1;
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
    string datatype;

    for (auto& token : tokens) { 

            if(token.token_type == SPACE){
                //do nothing
                //cout<<"hello"<<endl;
            }if(currentState == STAGE_3){
                currentState = STAGE_1;
            }else if(currentState == STAGE_1 && token.token_type == DATATYPE){
                //cout<<"hello"<<endl;
                currentState = STAGE_1;
                datatype = token.identifier; // i stored the datatype
            }else if(currentState == STAGE_1 && token.token_type == VARNAME){
                //cout<<"hello"<<endl;
                currentState = STAGE_2;
            }else if(currentState == STAGE_2 && token.token_type == COMMA){
                currentState = STAGE_1;
            }else if(currentState == STAGE_2 && token.token_type == SEMICOLON){
                currentState = STAGE_3;
                datatype = " ";
            }else if(currentState == STAGE_2 && token.token_type == EQUAL_SIGN){
                //2 cases that are valid: 
                //1. value assigned is a declared variable
                //2. value assigned is related to the datatype
                currentState = STAGE_0; //means we have to check first if the initialize valus is valid
        
            }else if(currentState == STAGE_0 && token.token_type == VARNAME || token.token_type == UNEXPECTED){
                
                if(token.token_type == VARNAME){

                    for (auto& t : tokens) {
                        //checking if its declared
                        if(token.token_type  == t.token_type && token.identifier == t.identifier){
                            currentState = STAGE_2;
                        }else 
                            break;
                    }
                
                }else if(token.token_type == UNEXPECTED){
                    

                    if(datatype == "int"){
                        if(isvalid_int(token.identifier)){
                            currentState = STAGE_2; 
                        }else 
                            return false;
                    }else if(datatype == "char"){
                        if(isvalid_char(token.identifier)){
                            currentState = STAGE_2; 
                        }else 
                            return false;
                    }else if(datatype == "float" || datatype == "double")
                        if(isvalid_float_double(token.identifier))
                            currentState = STAGE_2; 
                        else 
                            return false;
                }

            }

        }

    cout<<"STATE: "<<currentState<<endl;

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
        string delimiters = ",;= ";

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
