#include "Logger.h"
#include "Archive.h"
#include <cmath>
#include <cstring>
#include <cstdarg>

using WalrusRPG::PIAF::PIAFException;
using namespace WalrusRPG::PIAF;
using namespace WalrusRPG::Logger;

PIAFException::PIAFException(const char *format, ...) : msg("")
{
    va_list list;
    va_start(list, format);
    vsnprintf(msg, 1024, format, list);
    error(msg);
    va_end(list);
}

PIAFException::~PIAFException()
{
}

const char *PIAFException::what() const throw()
{
    return msg;
}
