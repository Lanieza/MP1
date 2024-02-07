// I declare, upon my honor, that I did this machine problem assignment by myself
// using online resources from the following:
//https://www.geeksforgeeks.org/static_cast-in-cpp/
// https://thispointer.com/c11-stdall_of-algorithm-tutorial-example/
// https://www.geeksforgeeks.org/unordered_set-in-cpp-stl
// Further, my solution is not a copy from the aforementioned sources.

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> //the algorithm header for all_of used isidentifier function
#include <unordered_set> // the unordered_set header for unordered_set<string> declaredVariables;
using namespace std;


enum State{ //the comments are for variable declaration
    STAGE_0 = 0,
    STAGE_1 = 1, //check for datatype
    STAGE_2 = 2, //check for variable name
    STAGE_3 = 3, //if equal - check if it is an identifier, comma - go back to stage 2, or semicolon - go back to stage 1
    STAGE_4 = 4, //if valid_init
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
    LEFT_PAREN, // 8
    RIGHT_PAREN, // 9

};



class Token {
public:
    string identifier; // for example int
    TokenType token_type = START; 
    State token_state; 

    
};

bool isvalid_int(const std::string& str) {
    if (str.size() == 3 && str[0] == '\'' && str[2] == '\'') {
        int asciiValue = static_cast<int>(str[1]); ////https://www.geeksforgeeks.org/static_cast-in-cpp/
        return (asciiValue >= '0' && asciiValue <= '9');
    } else {
        // Try to convert the string to an integer
        try {
            size_t pos;
            int intValue = stoi(str, &pos);
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
    } catch (const invalid_argument &) {
        return false;
    } catch (const out_of_range &) {
        return false;
    }
}

bool isvalid_char(const string &str) {

    // cout<<str<<endl;

    if (str.size() == 3 && str[0] == '\'' && str[2] == '\'') {
        return true; //means it is a single character
        // cout<<"here"<<endl;
    } else {
        try {
            size_t pos;
            int asciiValue = stoi(str, &pos);
            return pos == str.size() && asciiValue >= 0 && asciiValue <= 255;
        } catch (...) {
            return false; // Failed to convert to an integer
        }
    }
    return false;
}


bool isIdentifier(const string &str) {
    if (!str.empty()) {
        // Check if the string is only a number
        if (all_of(str.begin(), str.end(), ::isdigit)) { //https://thispointer.com/c11-stdall_of-algorithm-tutorial-example/
            return false;
        }

        // Check if the first character is an underscore or an alphabet
        if (str[0] == '_' || isalpha(str[0])) {
            // Check the rest of the characters
            for (char c : str.substr(1)) {
                if (!isalnum(c) && c != '_') {
                    return false;
                }
            }
            return true; // All characters were checked
        }
    }
    return false;
}


int getType(string str) {
    if (str == "int" || str == "char" || str == "double" || str == "float" || str == "void") {
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
    }else if(str == "("){
        return LEFT_PAREN;
    }else if(str == ")"){
        return RIGHT_PAREN;
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
    unordered_set<string> declaredVariables; //https://www.geeksforgeeks.org/unordered_set-in-cpp-stl/
    Token prevToken;
    
    
    // pushed the identifier first 
    for (const auto& identifier : parsed_string) {
        
        if(identifier != " "){ //ignoring white spaces
            t.identifier = identifier;
            // t.token_type = DATATYPE; not yet here since we want to have a switch case to get it type each
            tokens.push_back(t);
        }
    }
        
    //then pushed the tokentype 
    for (auto& token : tokens) { 
        int inputType = getType(token.identifier);
        token.token_type = static_cast<TokenType>(inputType);
        
        // cout<<token.identifier<<" "<<token.token_type<<endl;
        
            // Check for double declaration, considering the previous token
            if (token.token_type == VARNAME) {
                
                if (prevToken.token_type != EQUAL_SIGN && declaredVariables.find(token.identifier) != declaredVariables.end()) {
                    // Double declaration without an equal sign, return false
                    return false;
                }
        
                //Add the variable to the set of declared variables along with the previous element
                declaredVariables.insert({ token.identifier, prevToken.identifier });
            }
            
        prevToken = token;
    }
    
    
    // Initialize the set to keep track of seen identifiers
    unordered_set<string> seenIdentifiers;
    
    State currentState = STAGE_1;
    State previousState;
    string datatype;

    for (auto& token : tokens) {
        if ((prevToken.token_type != EQUAL_SIGN) && seenIdentifiers.find(token.identifier) != seenIdentifiers.end()) {
                //if it found a duplicate but the duplicate's previous element is not equal sign so it is not valid
                //previous element equal to sign means that the duplicate is not an identifier but instead a variable init that is a declared variable
                return false;
        }
        
        if (currentState == STAGE_3){
            currentState = STAGE_1;
            datatype = token.identifier; 
        }else if (currentState == STAGE_1 && token.token_type == DATATYPE){
            currentState = STAGE_1;
            datatype = token.identifier; // Store the datatype
        }else if (currentState == STAGE_1 && token.token_type == VARNAME)
            currentState = STAGE_2;
        else if (currentState == STAGE_2 && token.token_type == COMMA)
            currentState = STAGE_1;
        else if (currentState == STAGE_2 && token.token_type == SEMICOLON){
            currentState = STAGE_3;
            datatype = " ";
        }else if (currentState == STAGE_2 && token.token_type == EQUAL_SIGN)
            currentState = STAGE_0; // Check if the initialization value is valid
        else if (currentState == STAGE_0 && (token.token_type == VARNAME || token.token_type == UNEXPECTED)) {
            //ang mosulod diri na test case is the initialization of variable
            //so token.identifier likely previous stage is = or space
            int count = 0;
            for (auto& t : tokens) {

                if (token.identifier == t.identifier) {
                    count++;
                }

                if(count > 1){ // if it appears twice
                    currentState = STAGE_4;
                    break;
                }
            }


            if (currentState != STAGE_4) {

                if (datatype == "int") {
                    if (isvalid_int(token.identifier)) {
                        currentState = STAGE_4;
                    } else
                        return false;
                } else if (datatype == "char") {
                    if ((isvalid_char(token.identifier) == true)) {
                        // cout<<"hello"<<endl;
                        currentState = STAGE_4;
                    } else{
                        // cout<<"hi"<<endl;
                        return false;
                    }
                } else if (datatype == "float" || datatype == "double")
                    // cout<<"hello"<<endl;
                    if (isvalid_float_double(token.identifier))
                        currentState = STAGE_4;
                    else
                        return false;
            }

            seenIdentifiers.insert(token.identifier); // Add the identifier to the set

        }else if (token.token_type == UNEXPECTED)
            return false;
        else if (currentState != STAGE_3 && token.token_type == DATATYPE)
            return false;
        else if (previousState == STAGE_4 && token.token_type == SEMICOLON)
            currentState = STAGE_3;
        else if (previousState == STAGE_4 && token.token_type == COMMA)
            currentState = STAGE_1;
        else if(previousState == STAGE_4 && token.token_type == EQUAL_SIGN)
            currentState = STAGE_0;
        else if(currentState == STAGE_4 && token.token_type != SPACE)
            return false;
        else if(previousState == STAGE_1 && token.token_type == SEMICOLON)
            return false;
        
        
        // Update prevToken for the next iteration
        prevToken = token;
        previousState = currentState;
    }

    // Check if the last state is STAGE_3 then will return true otherwise will return false
    return currentState == STAGE_3;
}


bool check_Function(const vector<string>& parsed_string, vector<Token>& tokens){
        
    Token t;
    unordered_set<string> declaredVariables;
    Token prevToken;
    
    
    // pushed the identifier first 
    for (const auto& identifier : parsed_string) {
        
        if(identifier != " "){ //ignoring white spaces
            t.identifier = identifier;
            // t.token_type = DATATYPE; not yet assigned here since we want to loop the final tokens not the parsed string
            tokens.push_back(t); // storing the final tokens
        }
    }

    // for (auto& token : tokens) {
    //     cout<<token.identifier<<endl;
    // }
        
    //then pushed the tokentype 
    for (auto& token : tokens) { 
        int inputType = getType(token.identifier);
        token.token_type = static_cast<TokenType>(inputType);
        // static_cast <dest_type> (source); reference: https://www.geeksforgeeks.org/static_cast-in-cpp/
        
        // cout<<token.identifier<<" "<<token.token_type<<endl;
    }
    
    State currentState = STAGE_0;
    State previousState;
    string datatype;


    //take note that the varname here means a function name or a varname that is within the paren
    for (auto& token : tokens){

        if(currentState == STAGE_0 && token.token_type == VARNAME)
            return false;
        else if(currentState == STAGE_0 && token.token_type == DATATYPE)
            currentState = STAGE_1;
        else if(currentState == STAGE_1 && token.token_type == VARNAME)
            currentState = STAGE_2;
        else if(currentState == STAGE_1 && token.token_type == SEMICOLON)
            return false;
        else if (currentState == STAGE_2 && token.token_type == LEFT_PAREN)
            currentState = STAGE_3;
        else if(currentState == STAGE_3 && token.token_type == DATATYPE)
            currentState = STAGE_2;
        else if(currentState == STAGE_2 && token.token_type == COMMA)
            currentState = STAGE_3;
        else if(currentState == STAGE_2 && token.token_type == VARNAME)
            currentState = STAGE_2;
        else if(currentState == STAGE_2 && token.token_type == RIGHT_PAREN)
            currentState = STAGE_3;
        else if(currentState == STAGE_3 && token.token_type == SEMICOLON && prevToken.token_type == RIGHT_PAREN)
            currentState = STAGE_4;
        else if(currentState == STAGE_2 && token.token_type == DATATYPE && (prevToken.token_type != LEFT_PAREN || prevToken.token_type != COMMA))
            return false;
        else if(currentState == STAGE_3 && token.token_type == COMMA)
            currentState = STAGE_1;
        
        
        prevToken = token;
        previousState = currentState;

        // cout<<token.identifier<<"  :"<<"state: "<<currentState<<endl;
    }

    if(currentState == STAGE_4)
        return true;
    else 
        return false;

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
        string delimiters = ",;= ()";
        
        // Tokenize the string using the custom function
        tokenize(inputString, parsed_string, delimiters);
        
        vector<Token> tokens;
        

        if (testType == 1) {
            if (check_Variable(parsed_string, tokens))
                cout << "VALID VARIABLE DECLARATION" << endl;
            else 
                cout << "INVALID VARIABLE DECLARATION" << endl;
        } else{
            if (check_Function(parsed_string, tokens))
                cout << "VALID FUNCTION DECLARATION" << endl;
            else
                cout << "INVALID FUNCTION DECLARATION" << endl;
        }
        
        
    }

    return 0;
}
