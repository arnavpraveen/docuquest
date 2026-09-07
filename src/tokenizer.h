#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "provided.h"
#include <string>

class Tokenizer : public TokenizerBase
{
public:
    Tokenizer();
    // Reset the tokenizer to process a new input string from the beginning
    virtual void tokenize(const std::string& input);
    // Extract the next token into token and return true or return false if none remain
    virtual bool next(std::string& token);

private:
    std::string toTokenize;
    int currentPos;
};

#endif // TOKENIZER_H
