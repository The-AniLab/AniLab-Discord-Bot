#include <iostream>

#include <db_create.h>

void db_load()
{
    try
    {
        {
            // Add all databases
            db_configuration();
        }
    }
    catch (const std::runtime_error& e) 
    { 
        std::cerr << e.what() << '\n'; 
    }
}