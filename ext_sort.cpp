#include <vector>
#include <string>
#include <fstream>
#include "ext_sort.h"

bool writeFile(std::ofstream& outFile, const std::string& outFileName, std::vector<std::string> stringsBuf,
               uint8_t offset) {
    outFile.open(outFileName);
    if(!outFile.is_open()) {
        return false;
    }
    std::sort(stringsBuf.begin(), stringsBuf.end());
    std::ostream_iterator<std::string> output_iterator(outFile, "\n");
    std::ostream_iterator<std::string> noDelimIterator(outFile);
    std::copy(stringsBuf.begin(), stringsBuf.end() - offset, output_iterator);
    std::copy(stringsBuf.end() - offset, stringsBuf.end(), noDelimIterator);
    outFile.close();
    stringsBuf.clear();
    return true;
}
uint8_t extSort(const std::string& fname, int64_t ramByte) {
    std::ifstream inFile(fname);
    std::ofstream outFile;

    std::vector<std::string> stringsBuf;
    std::string str;
    std::string outFileName;

    uint16_t filesCounter = 0;
    uint64_t chunkLen     = 0;
    size_t strLen;

    if(!inFile.is_open()) {
        return 0;
    }
    while(!inFile.eof()) {
        std::getline(inFile, str);
        strLen = str.size();
        if(chunkLen + 1 + strLen > ramByte) {
            outFileName = std::to_string(filesCounter) + ".txt";
            if(!writeFile(outFile, outFileName, stringsBuf, 0)) {
                return 0;
            }
            chunkLen = 0;
            stringsBuf.clear();
            filesCounter++;
        }
        stringsBuf.push_back(str);
        chunkLen += strLen + 1;
    }
    outFileName = std::to_string(filesCounter) + ".txt";
    writeFile(outFile, outFileName, stringsBuf, 1);
    return 1;
}
