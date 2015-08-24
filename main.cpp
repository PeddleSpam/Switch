#include <iostream>
#include <string>

#include "Switch.h"

int main( int argc, char ** argv )
{
    typedef util::Switch< int, void ( std::string ) > SwitchType;
    
    SwitchType switch = SwitchType
    (
        SwitchType::Case( 1, []( std::string str )
            {
                std::cout << "Case 1: " << str << "\n";
        }   ),
        SwitchType::Case( 2, []( std::string str )
            {
                std::cout << "Case 2: " << str << "\n";
        }   ),
        SwitchType::Case( 4, []( std::string str )
            {
                std::cout << "Case 4: " << str << "\n";
        }   ),
        SwitchType::Default( []( std::string str )
            {
                std::cout << "Unknown case: " << str << "\n";
        }   )
    );
    
    switch( 1, "Hello Switch!" );
};
