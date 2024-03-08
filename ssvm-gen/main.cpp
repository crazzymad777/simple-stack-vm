#include <iostream>
#include <fstream>
#include "../includes/ssvm.h"
#include "ssvm-map.hpp"

int main(int argc, char* argv[]) {
    std::ofstream ofs;
    if (argc > 1) {
        std::string output_filename = argv[1];
        ofs.open(output_filename);
        if (!ofs.is_open()) {
            std::cerr << "Couldn't open file " << output_filename << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Use: " + std::string(argv[0]) + " [output filename]" << std::endl;
        return -2;
    }

    uint64_t stack_size = 0;
    ofs << "PROTECT1";
    ofs.write((char*)&stack_size, sizeof(stack_size));

    // COMMAND:
    // (label:) command (N,value|value)

    ofs.close();
    return 0;
}
