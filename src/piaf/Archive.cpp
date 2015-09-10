#include "Archive.h"
#include <cstring>
#include <cstdio>

using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::FileEntry;
using WalrusRPG::PIAF::FileType;

namespace
{
    template <typename T> T read_big_endian_value(void *ptr)
    {
        T result = 0;
        uint8_t *data = (uint8_t *) ptr;
        for (int i = 0; i < sizeof(T); i++)
            result |= data[i] << (8 * (sizeof(T) - 1 - i));
        return result;
    }
}

Archive::Archive(char *filepath)
{
    if (filepath == nullptr)
    {
    }
    // TODO : throw NPE
    FILE *file = fopen(filepath, "rb");
    if (file == nullptr)
    {
        // TODO : throw Couldn't open
    }


    // loading stuff happens NOW
    // ...
    // entries = new File_Entry[];
    // ...
}

Archive::~Archive()
{
    // delete[] entries;
}

FileEntry Archive::get_file_entry(char *filename)
{
    for (uint32_t index = 0; index < nb_files; index++)
    {
        if (strcmp(entries[index].filename, filename) == 0)
        {
            return entries[index];
        }
    }
    // throw exception
}

/*PFile*/ void /*FileEntry*/ get(char *filename)
{
    // return PFile(get_file_entry(filename));
}
