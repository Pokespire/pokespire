#include "Archive.h"
#include <cmath>
#include <cstring>
#include <cstdarg>

using WalrusRPG::PIAF::PIAFException;
using namespace WalrusRPG::PIAF;

PIAFException::PIAFException(const char *format, ...)
	: msg("")
{
	va_list list;
	va_start(list, format);
	vsnprintf(msg, 1024, format, list);
	va_end(list);
}

PIAFException::~PIAFException()
{
	if(msg != nullptr)
		delete msg;
}

const char* PIAFException::what() const throw()
{
	return msg;
}