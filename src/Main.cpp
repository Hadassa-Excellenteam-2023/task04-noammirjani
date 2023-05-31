#include <iostream>
#include <exception>
#include "MapCalculator.h"

int main(){

    try{
        MapCalculator().run();
    }
    catch(const std::exception& e){
        std::cout << e.what();
    }
    return 0;
}
