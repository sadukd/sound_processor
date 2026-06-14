#include "application.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        Application application;
        application.configure();
        return application.start(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch(...)
    {
        std::cerr << "Unknown exception\n";
        return -2;
    }
}
