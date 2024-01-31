// //cases include int which is declared as '9' 
// //because the equivalent of this string in ascii is 57
// important to set up the staging of the state and the input type
// iapil og token ang mga delimeters

#include <iostream>
#include <sstream>  
#include <vector>
using namespace std; 


// int getType(string str){
//     if(str == "int" || str == "char" || str == "double" || str == "float"){
//         return 0; // for datatype;
//     }
//     else if(str == " "){
//         return 1; //for space;
//     }
//     else if(isIdentifier(str))
//         return 2;
//     else if(str == ","){
//         return 3; //for space;
//     }
//     else if(str == ";"){
//         return 4; //for space;
//     }else 
//         return 5;
    
        
// }

// bool isIdentifier(string str){
//     //should start with underscore or any of the letter uppercase or lowercase letters
//     if(str[0] == '_' || str[0] >= 'A' && str[0] <= 'Z' && || str[0] >= 'a' && str[0] <= 'z' && ){
//         for(int i=1 ; i<str.length(); i++){
//             if (!(str[i] == '_' )|| (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z' ) || (str[i] >= '0' && str[i] <= '9' )){
//                 return false;
//             }
//         }

//         return true; //meaning all characters were checked
//     }else
//         return false;

// }


// bool check_Variable(const vector<string>& tokens) { // bool jud ni siya dapat
//     // I CANNOT MAKE THE STATE AS INDEX SINCE FOR SAMPLE
//     //INT        X; THERE WOULD BE MANY BLANK SPACES PA BEFORE X 
//     //SO UNYA RA MAG UPDATE OG STATE IF NANA KOS LETTERS EXCEPT SA MGA DELIMITERS
    
//     int state = 0, i=0;
    
//     for (auto it = tokens.begin(); it != tokens.end(); ++it){
//         int inputType = getType(*it);

//         if(state == 0 )
//     }

//     cout<<endl;


//     // if(state == 1) //meaning only the datype is the output
//     //     return false;
//     //another case is if naay comma dayon walay nisunod na variable
//     //
    
//     return true;
//     //if theres space mo continue 
//     //if theres comma mo continue 
//     //if theres ; then will stop
// }



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
    // int cases;
    // cin >> cases;

    // while(cases--){
        string inputString;
        // int testType;
        // cin >> testType;  // Read the type of the test case (1 or 2)

        // cin.ignore();  // i cite nig apil sa honor code 

        getline(cin, inputString);
        vector<string> tokens;

        // Define a set of delimiters (slashes and dots in this example)
        string delimiters = ",; ";

        // Tokenize the string using the custom function
        tokenize(inputString, tokens, delimiters);

        // Display the tokens and delimiters
        cout << "Tokens: ";
        for (const auto& token : tokens)
            cout << "[" << token << "] ";

        cout<<endl;

        // if (testType == 1) {
        //     // Handle variable declaration
        //     // check_Variable(tokens);
        //     if (check_Variable(tokens)) {
        //         cout << "VALID VARIABLE DECLARATION" << endl;
        //     } else {
        //         cout << "INVALID VARIABLE DECLARATION" << endl;
        //     }

        // } 
        //else if (testType == 2) {
        //     // Handle function declaration
        //     if (check_Function(tokens)) {
        //         cout << "VALID FUNCTION DECLARATION" << endl;
        //     } else {
        //         cout << "INVALID FUNCTION DECLARATION" << endl;
        //     }
        // }

    // }

    return 0;
}
