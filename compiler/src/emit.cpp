#include <emit.hpp>
#include <fstream>

ass::emitter::emitter(std::string filename) {
    this->filename = filename;
    this->header   = "";
    this->code     = "";
}

void ass::emitter::emit(std::string code) {
    this->code += code;
}

void ass::emitter::emit_line(std::string code) {
    this->code += code + '\n';
}

void ass::emitter::header_line(std::string code) {
    this->header += code + '\n';
}

void ass::emitter::write_file() {
    std::ofstream file;
    file.open(this->filename);
    file << this->header + this->code;
    file.close();
}