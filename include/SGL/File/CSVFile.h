#pragma once

#include "File.h"

#define TWO_D_VEC_STR std::vector<std::vector<std::string>>

namespace SGL
{
    class CSVFile : public File
    {
        TWO_D_VEC_STR contents;

        void parse();

    public:
        CSVFile(const std::string& filename)
        {
            read(filename);
            parse();
            dump();
        }

        const TWO_D_VEC_STR& getData() const
        {
            return contents;
        }
    };

    void CSVFile::parse() 
    {
        contents.resize(getContents().size());

        for (int i = 0; i < getContents().size(); i++)
            contents[i] = splitLine(getContents()[i], ',');
    }
}