#include <iostream>
#include <string>

#include <omp.h>

int main() {
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        std::string out = "hello from " + std::to_string(id) + "\n";
        std::cout << out;
    }
}

