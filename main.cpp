#include "Beast/Beast.h"

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char ** argv)
{
    char * expression_text = nullptr;

    if (--argc > 0)
        expression_text = *++argv;

    json expression_json;

    try
    {
        if (expression_text)
            expression_json = json::parse(expression_text);
        else
            std::cin >> expression_json;
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    Beast::Expression simplified = Beast::simplify(expression_json);
    std::cout << simplified.toJson() << std::endl;

    return 0;
}
