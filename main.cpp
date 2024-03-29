#include <iostream>
#include <fstream>
#include "frontend/driver.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char* argv[]) {

    try {
        auto dr_config = std::make_unique<yy::driver_config_t>(argc, argv);
        auto lexer =  std::make_unique<my_lexer_t>();
        yy::driver_t driver(std::move(lexer), std::move(dr_config));
        driver.parse();
    } catch (const std::exception& err) {
        std::cerr << err.what() << "\n";
    } catch (...) {
        std::cerr << "Unexpected error\n";
    }

    return 0;

}
