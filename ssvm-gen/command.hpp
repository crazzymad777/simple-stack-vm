#include <vector>

class ssvm_command {
public:
    enum type {
        SINGLE_OPCODE,
        OPCODE_WITH_CONST,
        VECTOR_OPCODE,
        LINKED_OPCODE // relative position
    };

    ssvm_command() {
        this->kind = SINGLE_OPCODE;
        linked_label = "";
    }

    ssvm_command(int opcode, uint64_t offset) {
        this->opcode = opcode;
        this->kind = SINGLE_OPCODE;
        this->offset = offset;
    }

    ssvm_command(int opcode, int64_t const_value, uint64_t offset) {
        this->opcode = opcode;
        this->kind = OPCODE_WITH_CONST;
        this->const_value = const_value;
        this->offset = offset;
    }

    ssvm_command(int opcode, std::vector<char> vec, uint64_t offset) {
        this->opcode = opcode;
        this->kind = VECTOR_OPCODE;
        this->vec = vec;
        this->offset = offset;
    }

    ssvm_command(int opcode, std::string label, uint64_t offset) {
        this->opcode = opcode;
        this->kind = LINKED_OPCODE;
        this->linked_label = label;
        this->offset = offset;
        this->const_value = -1;
    }

    int length() {
        if (this->kind == SINGLE_OPCODE) {
            return 1;
        } else if (this->kind == OPCODE_WITH_CONST || this->kind == LINKED_OPCODE) {
            return 9;
        } else if (this->kind == VECTOR_OPCODE) {
            return 9 + vec.size();
        }
        return 0;
    }

    void write_command(std::ofstream& ofs) {
        if (this->kind == SINGLE_OPCODE) {
            ofs.write(&opcode, 1);
        } else if (this->kind == OPCODE_WITH_CONST || this->kind == LINKED_OPCODE) {
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
    uint64_t offset;
};
