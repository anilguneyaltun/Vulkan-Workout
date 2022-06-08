#include "Application.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    
    Photonic::Application app{};
    
    try{
        app.run();
    }catch(std::exception &e){
        std::cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
