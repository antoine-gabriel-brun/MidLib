#include "tutorials/runAllTutorials.hpp"
#include <iostream>

//
// To learn how to use the library, look for the files 'tutorial1.hpp' and following.
//

int main(int argc, char *argv[])
{
    // Run all tutorials
    runAllTutorials();

    // End
    return std::cout << "Press Enter to quit." << std::endl << "> ", 0 * std::getchar();
}
