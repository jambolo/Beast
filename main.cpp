#include "Beast.h"

#include <json.hpp>

using json = nlohmann::json;

int main(int argc, char ** argv)
{
    char * expression_text = nullptr;

    if (--argc > 0)
    {
        expression_text = *++argv;
    }

    json expression_json;

    try
    {
        if (expression_text)
        {
            expression_json = json::parse(expression_text);
        }
        else
        {
            expression_json << std::cin;
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    Beast beast;
    Beast::Expression simplified = beast.simplify(expression_json);
    std::cout << simplified.toJson() << std::endl;

    return 0;
}
