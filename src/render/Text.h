#ifndef INCLUDE_TEXT_H
#define INCLUDE_TEXT_H

#include <string>

namespace WalrusRPG
{
    namespace Graphics
    {
        // TODO?: Rename it for something more related? It was first to draw debug
        // content, it's not even really that useful as there is the Font system now.
        namespace Text
        {
            void init();
            void deinit();

            void print_char(char c, unsigned x, unsigned y);

            void print_char(char c, unsigned x, unsigned y);

            void print_string(const char *str, unsigned x, unsigned y);

            void print_string(const std::string &str, unsigned x, unsigned y);

            void print_format(unsigned x, unsigned y, const char *format, ...);

            void print_format(unsigned x, unsigned y, const std::string &format, ...);
        }
    }
}


#endif
