#include <vector>
#include <string>
#include <algorithm>
#include <iostream>


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
    std::string identifier; // for example int
    TokenType token_type = START; // Specify the type for TokenType
	Token(std::string id, TokenType type) : identifier(id), token_type(type) {};
	Token(){};
};

bool isvalid_int(const std::string& str);
bool isvalid_float_double(const std::string &str);
bool isvalid_char(const std::string& str);

bool isIdentifier(const std::string &str) {
	if (str.empty()) return false;

    // Check if the string is only a number
    if (all_of(str.begin(), str.end(), ::isdigit)) {
        return false;
    }

    // Check if the first character is an underscore or an alphabet
    if (str[0] != '_' && !std::isalpha(str[0])) return false;

    // Check the rest of the characters
    for (char c : str.substr(1)) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
	
    return true; // All characters were checked

}

TokenType getType(std::string str);

bool check_Variable(const std::vector<std::string>& parsed_string, std::vector<Token>& tokens) {
	for (const auto& identifier : parsed_string) {
		auto type = getType(identifier);
		tokens.push_back( Token(identifier, type) );
	}

	State currentState = STAGE_1, previousState;
	std::string datatype;
	Token prevToken;

	for (auto& token : tokens) {
		switch (currentState)
		{
		case STAGE_1:
			if (token.token_type == DATATYPE) {
				currentState = STAGE_1;
				datatype = token.identifier;
			} else if (token.token_type == VARNAME) {
				currentState = STAGE_2;
			}
			break;
		
		case STAGE_2:
			if (token.token_type == COMMA) {
				currentState = STAGE_1;
			} else if (token.token_type == SEMICOLON) {
				currentState = STAGE_3;
				datatype = "";
			} else if (token.token_type == EQUAL_SIGN) {
				currentState = STAGE_0;
			}
			break;

		case STAGE_3:
			currentState = STAGE_1;
			break;

		case STAGE_0:
			// Since this is checking after EQUAL, the only tokens should only be VARNAME or UNEXPECTED
			// otherwise, it should throw an error (this is a guard clause)
			// Since literals are classified as UNEXPECTED
			if (token.token_type != VARNAME && token.token_type != UNEXPECTED)
				return false;

            // cout<<token.identifier<<endl;

            for (auto& t : tokens) {
                //checking if its declared
                if(token.identifier == t.identifier){
                    currentState = STAGE_2;
                    break;
                }
            }

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
			break;

		default:
			if (token.token_type == SPACE) break;
			if (token.token_type == UNEXPECTED)
				return false;
			if (currentState != STAGE_3 && token.token_type == DATATYPE)
				return false;
			break;
		}
        std::cout << token.identifier << "   STATE: " << currentState << std::endl;


        // Update prevToken for the next iteration
        prevToken = token;
        previousState = currentState;
	}
}