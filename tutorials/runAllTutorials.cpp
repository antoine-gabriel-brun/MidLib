#include "runAllTutorials.hpp"

//
// To learn how to use the library, look for the files 'tutorial1.hpp' and following.
//

#include <string>
#include <iostream>

#include "tutorial1.hpp" // this directive #includes all tutorials
#include "PROV_sandbox.hpp" // PROV DEL, and delete file

namespace
{
    typedef void(*tutorialFctType)(std::string const&); // tutorialFctType is a pointer to a function like tutorial1()
    void runTutorial(tutorialFctType tutorialFct, int index)
    {
        std::cout << "Tutorial " << index << ": ";

        try {
            std::string fileName = std::to_string(index) + ".mid";
            tutorialFct(fileName);
            std::cout << "Success. Output saved as '" << fileName << "'.";
        } catch (MID::Exception const& e) {
            std::cout << "Failure: '" << e.msg() << "'";
        } catch (std::exception const& e) {
            std::cout << "Failure: '" << e.what() << "'";
        } catch (...) {
            std::cout << "Failure (unknown)";
        }
    
        std::cout << std::endl;
    }
}

void runAllTutorials()
{
    runTutorial(tutorial1, 1);
    runTutorial(tutorial2, 2);
    runTutorial(tutorial3, 3);
    runTutorial(tutorial4, 4);
    runTutorial(tutorial5, 5);
    runTutorial(tutorial6, 6);
    runTutorial(tutorial7, 7);
    runTutorial(tutorial8, 8);
    runTutorial(tutorial9, 9);
    runTutorial(tutorial10, 10);
    runTutorial(tutorial11, 11);
    runTutorial(sandbox, 1000); // PROV DEL
}
