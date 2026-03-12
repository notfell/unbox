#include "vendor.hpp"
#include <cstdlib>

bool g_verbose = false;

int main(int argc, char* argv[])
{
    printf(APP_NAME);

    std::string output;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-o") && i + 1 < argc)
            output = argv[++i];
        
         if (!strcmp(argv[i], "-v"))
            g_verbose = true;
    }

    debug("on")

    if (output.empty())
    {
        error("usage: unbox -o <output>");
        return EXIT_FAILURE;
    }

    dumper unbox(output, "package");
    
    return EXIT_SUCCESS;
}