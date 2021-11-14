#ifndef COMPILER_H
#define COMPILER_H

#include "Object.h"

class Compiler {
    
    public:
        Compiler(std::vector<Rule> rules);
        void executeAllRules();
        void executeRule(std::string rule);
    private:
        std::vector<Rule> rules;
        std::vector<Rule> completed; 
        bool isDependenciesResolved(Rule rule);
        bool checkRuleExists(std::string rule, std::string caller);
        bool isRuleCompleted(std::string rule);
        bool isReady(Rule rule);
};

#endif /* COMPILER_H */
