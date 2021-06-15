#ifndef __ASS_EMIT_HPP__
#define __ASS_EMIT_HPP__

#include <string>

namespace ass {
    class emitter {
    public:
        emitter(std::string filename);
        void emit(std::string code);
        void emit_line(std::string code);
        void header_line(std::string code);
        void write_file();
    private:
        std::string filename, header, code;
    };
}

#endif // __ASS_EMIT_HPP__