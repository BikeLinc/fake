#include "Fakefile.h"

int main(const int argc, const char** argv) {
    // Generate Symbols
    Parser parser("Fakefile");

    if(parser.getRules().size() == 0) {
        std::cout << "fake: *** No targets. Stop." << std::endl;
        exit(2);
    }

    // Execute target if arguments
    if(argc == 2) {
        Compiler compiler(parser.getRules());
        compiler.executeRule(argv[1]);
    // Execute all rules and dependencies
    } else {
        Compiler compiler(parser.getRules());
        compiler.executeAllRules();
    }

    return 0;
}
