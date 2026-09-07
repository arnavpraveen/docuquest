#include "agent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

Agent::Agent(const IndexBase& index) : m_index(index)
{
}

Agent::~Agent()
{
}

bool Agent::load_prompts(const std::string& terms_file, const std::string& summarize_file)
{
    // Try to open both template files
    std::ifstream tFile(terms_file);
    if (!tFile.is_open())
        return false;

    std::ifstream sFile(summarize_file);
    if (!sFile.is_open())
        return false;

    // Read entire file contents into member strings
    std::stringstream tBuf;
    std::stringstream sBuf;
    tBuf << tFile.rdbuf();
    sBuf << sFile.rdbuf();

    m_termsTemplate = tBuf.str();
    m_summarizeTemplate = sBuf.str();
    return true;
}

bool Agent::query(const std::string& question, std::string& answer)
{
    if (m_termsTemplate.empty() || m_summarizeTemplate.empty())
        return false;
    
    std::string termsPrompt = m_termsTemplate;
    std::string placeholder = "{query}";
    size_t pos = termsPrompt.find(placeholder);
    while (pos != std::string::npos) {
        termsPrompt.replace(pos, placeholder.size(), question);
        pos = termsPrompt.find(placeholder, pos + question.size());
    }
    
    // Send the prompt to the LLM to get suggested search terms
    std::string llmResponse;
    if (!query_llm("terms", termsPrompt, llmResponse))
        return false;
    
    TokenizerBase* tokenizer = create_tokenizer();
    std::vector<std::vector<std::string>> termGroups;
    std::istringstream stream(llmResponse);
    std::string line;
    
    while (std::getline(stream, line)) {
        tokenizer->tokenize(line);
        std::string token;
        std::vector<std::string> group;
        while (tokenizer->next(token))
            group.push_back(token);
        if (!group.empty())
            termGroups.push_back(group);
    }
    delete tokenizer;
    
    std::set<std::string> allDocs;
    for(int i = 0; i< termGroups.size(); i++) {
        std::vector<std::string> matches = m_index.query(termGroups[i]);
        for (int j = 0; j < matches.size(); j++) {
            allDocs.insert(matches[j]);
        }
    }
    // No relevant documents found
    if(allDocs.empty()) {
        return false;
    }
    
    // Sort and cap at 10 documents
    std::vector<std::string> sortedDocs(allDocs.begin(), allDocs.end());
    std::sort(sortedDocs.begin(), sortedDocs.end());
    if (sortedDocs.size() > 10)
        sortedDocs.resize(10);
    
    // Load the full text of each selected document
    std::string docContents = "";
    for (int i = 0; i < sortedDocs.size(); i++) {
        std::ifstream file(sortedDocs[i]);
        if (!file.is_open())
            continue;
        std::stringstream buf;
        buf << file.rdbuf();
        docContents += buf.str() + "\n";
    }
    
    // Copy the summarize template and replace {query} with the question
    std::string sumPrompt = m_summarizeTemplate;
    pos = sumPrompt.find(placeholder);
    while (pos != std::string::npos) {
        sumPrompt.replace(pos, placeholder.size(), question);
        pos = sumPrompt.find(placeholder, pos + question.size());
    }   
    
    // Replace {documents} with the loaded document text
    std::string docPlaceholder = "{documents}";
    pos = sumPrompt.find(docPlaceholder);
    while (pos != std::string::npos) {
        sumPrompt.replace(pos, docPlaceholder.size(), docContents);
        pos = sumPrompt.find(docPlaceholder, pos + docContents.size());
    }
    // Send the summarization prompt to the LLM for the final answer
    if (!query_llm("summarize", sumPrompt, answer))
        return false;

    return true;
}
