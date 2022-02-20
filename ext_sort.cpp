#include "ext_sort.h"

#include <fstream>
#include <vector>
#include <iostream>

MinHeap::MinHeap(const std::vector<MinHeapNode>& heap): heapSize_(heap.size()), heap_(heap) {
    int i = (heapSize_ - 1) / 2;
    while(i >= 0) {
        minHeapify(i);
        i--;
    }
}

void MinHeap::minHeapify(uint16_t i) {
    uint16_t left  = 2 * i + 1;
    uint16_t right = left + 1;
    int smallest   = i;
    if(left < heapSize_ && heap_[left].str < heap_[i].str)
        smallest = left;
    if(right < heapSize_ && heap_[right].str < heap_[smallest].str)
        smallest = right;
    if(smallest != i) {
        std::swap(heap_[i], heap_[smallest]);
        minHeapify(smallest);
    }
}

uint8_t mergeFiles(uint16_t filesNumber) {
    std::ifstream inFiles[filesNumber];
    std::string outFileName = "out.txt";
    std::ofstream outFile(outFileName);
    std::vector<std::string> fnames;
    for(int i = 0; i < filesNumber; i++) {
        std::string fname = std::to_string(i) + ".txt";
        inFiles[i].open(fname);
        fnames.push_back(fname);
        if(!inFiles[i].is_open()) {
            std::cout << "Error opening temporary file: " << fname << std::endl;
            return 0;
        }
    }
    if(!outFile.is_open()) {
        std::cout << "Error opening output file: " << outFileName << std::endl;
        return 0;
    }
    std::vector<minHeapNode_t> heap;
    std::string heapStr;
    uint16_t i;
    for(i = 0; i < filesNumber; i++) {
        if(!inFiles[i].eof()) {
            std::getline(inFiles[i], heapStr);
            minHeapNode_t heapNode = {heapStr, i};
            heap.push_back(heapNode);
        }
    }
    MinHeap hp(heap);

    int count = 0;

    while(count != i) {
        minHeapNode_t root = hp.getMin();
        outFile << root.str;
        std::string str;
        if(!std::getline(inFiles[root.i], root.str)) {
            root.str = "\xff";
            count++;
        }
        if(count != i) {
            outFile << std::endl;
        }
        hp.replaceMin(root);
    }

    for(uint16_t j = 0; j < filesNumber; j++) {
        inFiles[j].close();
        std::remove(fnames[j].c_str());
    }
    outFile.close();

    return 1;
}

bool writeFile(std::ofstream& outFile, const std::string& outFileName, std::vector<std::string> stringsBuf,
               uint8_t offset) {
    outFile.open(outFileName);
    if(!outFile.is_open()) {
        std::cout << "Error opening temporary file: " << outFileName << std::endl;
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
        std::cout << "Error opening file: " << fname << std::endl;
        return 0;
    }
    while(!inFile.eof()) {
        std::getline(inFile, str);
        strLen = str.size();
        if(chunkLen + 1 + strLen > ramByte) {
            outFileName = std::to_string(filesCounter) + ".txt";
            if(!writeFile(outFile, outFileName, stringsBuf, 0)) {
                std::cout << "Error writing temporary file: " << outFileName << std::endl;
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
    inFile.close();

    if(!mergeFiles(filesCounter + 1)) {
        std::cout << "Error merging files" << std::endl;
        return 0;
    }
    return 1;
}
