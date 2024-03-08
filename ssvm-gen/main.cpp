#include <iostream>
#include <fstream>
#include "../includes/ssvm.h"
#include "ssvm-map.hpp"
#include "command.hpp"
#include <list>

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
    const char* protect_field = "PROTECT1";
    ofs << protect_field;
    ofs.write((char*)&stack_size, sizeof(stack_size));


    std::list<ssvm_command> commands;
    std::map<std::string, int> label_to_command_index;
    std::string word;
    // COMMAND:
    // (label:) command (N,value|value)
    std::string label;
    bool has_label;
    bool ignore_unknown_word;
    uint64_t offset = 0;

    while (!std::cin.eof()) {
        word = "";
        ignore_unknown_word = false;
        label = "";
        has_label = false;
        std::cin >> word;
        if (word == ".stack") {
            uint64_t s;
            if (std::cin >> s) {
                std::streampos p = ofs.tellp();
                ofs.seekp(8);
                ofs.write((char*)&s, sizeof(uint64_t));
                ofs.seekp(p);
            }
            ignore_unknown_word = true;
        }

        if (word.length() > 0) {
            if (word[word.length()-1] == ':') {
                has_label = true;
                label = word.substr(0, word.length()-1);
            }
        }

        if (has_label) {
            label_to_command_index[label] = commands.size();
            std::cin >> word;
        }

        if (opcodes.count(word) > 0) {
            int opcode = opcodes[word];
            int optype = opcodes_types[opcode];

            ssvm_command command;
            if (optype == 0) {
                command = ssvm_command(opcode, offset);
            } if (optype == 1) { // const
                int64_t const_value;
                std::cin >> const_value;
                command = ssvm_command(opcode, const_value, offset);
            } else if (optype == 2) { // vector
                std::cerr << "Not implemented: " + word << std::endl;
                return -43;
            } else if (optype == 3) { // relative to other command (probably label)
                std::cerr << "Not implemented: " + word << std::endl;
                return -43;
            }
            commands.push_back(command);
            command.write_command(ofs);
            offset += command.length();
        } else if (!ignore_unknown_word && word != "") {
            std::cerr << "Unknown word: " + word << std::endl;
            return -42;
        }
    }

    ofs.close();
    return 0;
}
