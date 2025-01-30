#include <iostream>

#include "Driver.hpp"
#include "Options.hpp"

int main(int argc, char* argv[])
{
    try {
        Options options(argc, argv);

        if (options.show_help()) {
            options.RenderHelp(std::cout);
        }
        else {
            auto driver = Driver::Create(options);
            if (!driver->Run(options)) {
                return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

