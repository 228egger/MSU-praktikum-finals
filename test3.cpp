#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <stack>
#include <algorithm>
#include <string>


/*
	S -> <name>(K)
	K -> S{,S} | eps
*/





enum LexType{
    IDENT,
    COMMA,
    LEFT_BR,
    RIGTH_BR, 
    END
};

class Parser{
	public:
		Parser() = default;
		void parse(std::string & stream);
	private:
		void S();
		void K();
		void gl();
		LexType cur_lex{};
		int pos;
		std::string s;
		std::string * stream;
};


void
Parser::gl(){
	cur_lex = END;
	if (pos == int(stream->size())) {
		cur_lex = END;
        return;
    }
    int test = (*stream)[pos];
    if ((!isspace(test)) && (!isalpha(test)) && (!isdigit(test)) && (test != '(') && (test != ')') && (test != ',')) {
        throw std::exception();
    }
    while (isspace((*stream)[pos])) {
        pos++;
        if (pos == int(stream->size())) {
            return;
        }
    }
    switch((*stream)[pos]){
        case ('('):
            cur_lex = LEFT_BR;
            pos++;
            break;
        case (')'):
            cur_lex = RIGTH_BR;
            pos++;
            break;
        case (','):
            cur_lex = COMMA;
            pos++;
            break;
        default:
            cur_lex = IDENT;
            if (isdigit((*stream)[pos])){
                throw std::exception();
            }
            while ((isalpha((*stream)[pos])) || (isdigit((*stream)[pos]))) {
                pos++;
                if (pos == int(stream->size())) {
                    return;
                }
            }
            break;
    }
}


void 
Parser::S(){

	if (cur_lex == IDENT){
		gl();
		if (cur_lex == LEFT_BR){
            gl();
			K();
			if (cur_lex == RIGTH_BR){
				gl();
			}
			else{

				throw std::exception();
			}
		}
		else{
			throw std::exception();
		}
	}
	else{
		throw std::exception();
	}
}

void 
Parser::K(){
	if (cur_lex == IDENT){
		S();
		while(cur_lex == COMMA){
			gl();
			S();
		}
	}
	else{
	}
}

void
Parser::parse(std::string & arg){
	stream = &arg;
	pos = 0;
	gl();
	S();
	if (cur_lex != END){
		throw std::exception();
	}
}



int
main()
{
     Parser parser;
    try {
        std::string str = "h()";
        parser.parse(str);
    }
    catch (...) {
        std::cout << "NO" << std::endl;
        return 0;
    }
    std::cout << "YES" << std::endl;
    return 0;
}