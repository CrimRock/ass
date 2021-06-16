#include <iostream>
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <parse.hpp>
using namespace std;

int main(int argc, char* argv[])
{
    cout << "ASSC v0.1\n";

    std::string output = "out.cpp", filename;

    if (argc < 2) {
        cout << "Error: Compiler needs source file as argument.\n";
        exit(1);
    } else if (argc == 2) {
        filename = argv[1];
    } else if (argc == 4) {
        filename = argv[1];
        if (std::string(argv[2]) == "-o" || std::string(argv[2]) == "--output") {
            output = argv[3];
        } else {
            cout << "Error: Compiler needs to specify output flag as second argument.\n";
            exit(1);
        }
    }

    ifstream file(filename);
    std::string source((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    
    ass::lexer lexer(source);
    ass::emitter emitter(output);
    ass::parser parser(&lexer, &emitter);

    parser.program();
    emitter.write_file();

    cout << "Compiling completed.\n";

    return 0;
}