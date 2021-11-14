// FakefileParser.cpp
// By Chloe Walsh
// 11/7/21

#include "Parser.h"

Parser::Parser(const char* filepath) {
    parse(filepath);
    }

void Parser::parse(const char* filepath) {
    std::ifstream file(filepath);
    std::string str;

    if(!file.is_open()) {
        std::cout << "fake: *** No targets specified and no Fakefile found. Stop." << std::endl;
        exit(2);
    }

    // Loops Every Line
    while (std::getline(file, str)) {

        if(str == "") {
            endRule();
        } else {
            for(auto character : str) {
                if(isprint(character)) {
                    if(character == '=' && !variableMode && !ruleMode) {
                        endRule();
                        beginVariable();
                    } else if(character == 34 && variableMode) {
                        toggleValueMode();
                    } else if(character == ':' && !ruleMode) {
                        endRule();
                        beginRule();
                    } else if((character == ' ' && ruleMode) || (character == ' ' && valueMode)) {
                        addCharToBuffer(character);
                    }
                    /*
                    else if(character == '$') {
                        beginReference();
                    } else if (character == '(' || character == ')' || character == '{' || character == '}') {
                        if(referenceMode) {
                            toggleReferenceDefinitionMode();
                        } else {
                            addCharToBuffer(character);
                        }
                    }
                    */
                    else {
                        if(character != ' ') {
                            addCharToBuffer(character);
                        }
                    }
                    addDependencies();
                }
            }
        }
        addCommand();
        currentLine++;
    }
    endRule();
}

std::vector<Variable> Parser::getVariables() {
    return variables;
}

std::vector<Rule> Parser::getRules() {
    return rules;
}

// ==== Buffer Manipulation ====

void Parser::addCharToBuffer(char character) {
    bufferA += character;
}

void Parser::storeBufferAinB() {
    bufferB = bufferA;
    bufferA = "";
}

// ==== Variable Creation ====

void Parser::beginVariable() {
    storeBufferAinB();
    variableMode = true;
}

void Parser::toggleValueMode() {
    if(!valueMode) {
        // Begin defining value
        valueMode = true;
    // If currently defining value
    } else if(valueMode) {
        // Stop variable building mode
        // Stop defining value
        variableMode = false;
        valueMode = false;
        // Create Variable
        endVariable();
    }
}

void Parser::endVariable() {
    Variable variable(bufferB, bufferA);
    // Add to list
    variables.push_back(variable);
    // Empty buffers
    bufferB = "";
    bufferA = "";
}

// ==== Rule Creation ====

void Parser::beginRule() {
    ruleMode = true;
    // Set this line as definition line
    definitionLine = currentLine;
    // Move A to RuleBuffer and free A
    ruleBuffer = bufferA;
    bufferA = "";
}

void Parser::endRule() {
    if(ruleMode) {
        ruleMode = false;
        std::vector<std::string> deps;
        std::string dep = "";
        for(int i = 0; i < depsBuffer.size(); i++) {
            char character = depsBuffer[i];
            if(character != ' ') {
                dep += character;
            } else {
                if(dep != "") {
                    deps.push_back(dep);
                    dep = "";
                }
            }
        }
        if(dep != "") {
            deps.push_back(dep);
            dep = "";
        }
        Rule rule(ruleBuffer, definitionLine, deps, commandBuffer, variables);
        rules.push_back(rule);
        ruleBuffer = "";
        depsBuffer = "";
        commandBuffer.clear();
    }
}

void Parser::addCommand() {
    if(ruleMode) {
            std::string filteredCommand = "";
            for(int i = 0; i < bufferA.size(); i++) {
                char character = bufferA[i];
                if(filteredCommand == "" && character == ' ') {
                    // Do Nothing
                } else {
                    filteredCommand += character;
                }
            }
            if(filteredCommand != "") {
                commandBuffer.push_back(filteredCommand);
            }
            bufferA = "";
        }
}

void Parser::addDependencies() {
    if(ruleMode && (definitionLine == currentLine)){
        if(depsBuffer == "" && bufferA == " ") {
            bufferA = "";
        } else {
            depsBuffer += bufferA;
            bufferA = "";
        }
    }
}

// ==== Reference Creation ====

void Parser::beginReference() {
    referenceMode = true;
    storeBufferAinB();
}

void Parser::toggleReferenceDefinitionMode() {
    if(referenceDefinitionMode) {
        referenceDefinitionMode = false;
        endReference();
    } else {
        referenceDefinitionMode = true;
    }
    
}

void Parser::endReference() {
   referenceMode = false;
   std::string refValue = "";
   
   for(auto& var : variables) {
       if(bufferA == var.name) {
           refValue = var.value;
       }
   }
   
   bufferA = bufferB;
   bufferA += refValue;
   bufferB = "";
}
