// FakefileParser.h
// By Chloe Walsh
// 11/7/21

#include <fstream>
#include "Object.h"

class Parser {
public:
    Parser(const char* filepath);
    std::vector<Variable> getVariables();
    std::vector<Rule> getRules();
private:
    std::vector<Variable> variables;
    std::vector<Rule> rules;

    std::string bufferA = "";
    std::string bufferB = "";
    std::string ruleBuffer = "";
    std::string depsBuffer = "";
    std::vector<std::string> commandBuffer;

    bool variableMode = false;
    bool valueMode = false;
    bool ruleMode = false;
    bool referenceMode = false;
    bool referenceDefinitionMode = false;

    int definitionLine = 0;
    int currentLine = 1;

    void parse(const char* filepath);
    void addCharToBuffer(char character);
    void storeBufferAinB();
    void beginVariable();
    void toggleValueMode();
    void endVariable();
    void beginRule();
    void endRule();
    void addCommand();
    void addDependencies();
    void beginReference();
    void toggleReferenceDefinitionMode();
    void endReference();
};
