#pragma once

#include <errno.h>
#include <string>

namespace File
{
    inline std::string readFileContent(const char* filePath)
    {
        FILE* file = fopen(filePath, "rt");

        if (!file)
        {
            printf("\n[%s] - %s", filePath, strerror(errno));
            return strerror(errno);
        }

        fseek(file, 0, SEEK_END);
        unsigned long length = ftell(file);
        char* contents = new char[length + 1];
        memset(contents, 0, length + 1);
        fseek(file, 0, SEEK_SET);
        fread(contents, 1, length, file);
        fclose(file);

        std::string result(contents);
        delete[] contents;
        return result;
    }
};