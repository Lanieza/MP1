// //cases include int which is declared as '9' 
// //because the equivalent of this string in ascii is 57
// important to set up the staging of the state and the input type
// iapil og token ang mga delimeters



#include <iostream>
#include <sstream>  
#include <vector>
#include <cctype> // Include for isalnum function
using namespace std; 


enum TokenType {
    DATATYPE = 0,
    SPACE = 1,
    IDENTIFIER = 2,
    COMMA = 3,
    SEMICOLON = 4,
    EQUAL_SIGN = 5,
    UNEXPECTED = 6
};


bool isIdentifier(const std::string &str) {
    if (!str.empty() && (str[0] == '_' || isalpha(str[0]))) {
        for (char c : str.substr(1)) {
            if (!isalnum(c) && c != '_') {
                return false;
            }
        }
        return true; // All characters were checked
    }
    return false;
}



int getType(string str){
    if(str == "int" || str == "char" || str == "double" || str == "float"){
        return DATATYPE; // for datatype;
    }
    else if(str == " " ){
        return SPACE; //for space;
    }
    else if(isIdentifier(str))
        return IDENTIFIER;
    else if(str == ","){
        return COMMA; //for comma
    }
    else if(str == ";"){
        return SEMICOLON; //for semicolon
    }else if(str == "="){ 
        //i apil pa and make sure if naay equals kailangan int char ang assigned value maybe a function
        return EQUAL_SIGN; //for equal sign
    }else 
        return UNEXPECTED; //for something that is unexpected
        
}


bool check_Variable(const vector<string>& tokens) { // bool jud ni siya dapat
    //update the states:
    //stage 0 = 1 declaring funtion or 2 function declarations
    //stage 1 = variable type 
    //stage 2 = identifier
    //stage 3 = 
    //stage 4 =  check if theres a comma;


    //1 int x,y; 
    //  1 - stage 0
    //  int - stage 1
    // x - stage 2
    // , - stage 2 but its a different type of identifier its a delimiter should expect a identifier
    // y - stage 2
    // ; - stage 3 end but will not automatically return since need to check if it also the end of the vector array.

    int current_stage = 0, i=0;
    
    for (auto it = tokens.begin(); it != tokens.end(); ++it){
        int inputType = getType(*it);
        int prev_stage = current_stage;
        //2 possible cases:
        //if previos state is greater than the current state then something must be wrong
        //except if na reach na niya ang stage 3 tapos balik siyag stage 1 for other declaration
        
            if (inputType == DATATYPE){
                current_stage = 1;
            }else if(inputType == SPACE){
                
            }else if(inputType == IDENTIFIER){
                current_stage = 2;
                
            }else if(inputType == COMMA){
                
            }else if(inputType == SEMICOLON){
                current_stage = 3;
                
            }else if(inputType == EQUAL_SIGN){
                current_stage = 2;
                
            }else if(inputType == UNEXPECTED){
            
            }

            
        
        //cout<<*it<<" "<<"prev state: "<<prev_stage<<" current state: "<<current_stage<<endl;
        
        if(prev_stage > current_stage && prev_stage != 3){ 
            //it is possible that in one line there will be multiple declarations
            //so the state cannot go back to its previous state but it must be allowed when stage 3 is reached and then stage is reset to 1 for the declaration of other variable
        
            return false;
        }

        
        
    }

    if(current_stage == 3)
        return true;
    else
        return false; //assume first that its not valid



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

int main() {
    int cases;
    cin >> cases;

    while(cases--){
        string inputString;

        int testType;
        cin >> testType;  // Read the type of the test case (1 or 2)
        cin.ignore();  // i cite nig apil sa honor code 

        getline(cin, inputString);
        vector<string> tokens;

        // Define a set of delimiters (slashes and dots in this example)
        string delimiters = ",; ";

        // Tokenize the string using the custom function
        tokenize(inputString, tokens, delimiters);

        // Display the tokens and delimiters
        // cout << "Tokens: ";
        // for (const auto& token : tokens)
        //     cout << "[" << token << "] ";

        // cout<<endl;


        if(testType == 1){
            if (check_Variable(tokens)) {
                cout << "VALID VARIABLE DECLARATION" << endl;
            } else {
                cout << "INVALID VARIABLE DECLARATION" << endl;
            }
        }else if (testType == 2) {
            // if (check_Function(tokens)) {
            //     cout << "VALID FUNCTION DECLARATION" << endl;
            // } else {
            //     cout << "INVALID FUNCTION DECLARATION" << endl;
            // }
        }

    }

    return 0;
}
