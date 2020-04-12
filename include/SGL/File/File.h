#pragma once

#include <vector>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

namespace SGL
{
    class File
    {
        std::vector<std::string> contents;

    protected:
        void dump()
        {
            contents.clear();
        }
    
    public:
        const std::vector<std::string>& getContents() const
        {
            return contents;
        }

        void read(const std::string& file_location);

        std::vector<std::string> splitLine(const std::string& line, char delimeter) const;
    };

    void File::read(const std::string& file_location)
    {
        std::ifstream file(file_location);
        assert(file);

        for (std::string line; std::getline(file, line);)
            contents.push_back(line);

        file.close();
        contents.shrink_to_fit();
    }

    std::vector<std::string> File::splitLine(const std::string& line, char delimeter) const
    {
        std::vector<std::string> ret;
        std::stringstream stream(line);
        for (std::string section; std::getline(stream, section, delimeter);)
            ret.push_back(section);

        ret.shrink_to_fit();
        return ret;
    }
}