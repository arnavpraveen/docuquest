#include "tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer() : toTokenize(""), currentPos(0)
{
}

void Tokenizer::tokenize(const std::string& input)
{
    // Store the new input and reset the scan position
    toTokenize = input;
    currentPos = 0;
}

bool Tokenizer::next(std::string& token)
{
    std::string current = "";
    for(; currentPos < toTokenize.size(); currentPos++) {
        if(std::isalnum(toTokenize[currentPos])) {
            // Append lowercase version of the character
            current += std::tolower(toTokenize[currentPos]);
        } else {
            if(current == "") {
                continue;
            } else {
                break;
            }
        }
    }
    if(current != "") {
        token = current;
        return true;
    }
	return false; // no more tokens
}
