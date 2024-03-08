#include <vector>

class ssvm_command {
public:
    enum type {
        SINGLE_OPCODE,
        OPCODE_WITH_CONST,
        VECTOR_OPCODE,
        LINKED_OPCODE // relative position
    };

    ssvm_command(int opcode) {
        this->opcode = opcode;
        this->kind = SINGLE_OPCODE;
    }

    ssvm_command(int opcode, int64_t const_value) {
        this->opcode = opcode;
        this->kind = OPCODE_WITH_CONST;
        this->const_value = const_value;
    }

    ssvm_command(int opcode, std::vector<char> vec) {
        this->opcode = opcode;
        this->kind = VECTOR_OPCODE;
        this->vec = vec;
    }

    ssvm_command(int opcode, std:string label) {
        this->opcode = opcode;
        this->kind = LINKED_OPCODE;
        this->linked_label = label;
    }

    void write_command(std::ofstream ofs) {
        if (this->kind == SINGLE_OPCODE) {
            ofs.write(&opcode, 1);
        } else if (this->kind == OPCODE_WITH_CONST) {
            ofs.write(&opcode, 1);
            ofs.write((char*)&const_value, 8);
        } else if (this->kind == VECTOR_OPCODE) {
            ofs.write(&opcode, 1);
            this->const_value = vec.size();
            ofs.write((char*)&const_value, 8);
            for (auto x : vec) {
                ofs.write(&x, 1);
            }
        }
    }

    char opcode;
    type kind;
    int64_t const_value;
    std::vector<char> vec;
    std::string linked_label;
};
