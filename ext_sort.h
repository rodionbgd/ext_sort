#ifndef EXT_SORT_EXT_SORT_H
#define EXT_SORT_EXT_SORT_H

#include <cstdint>
#include <vector>
#include <string>

uint8_t extSort(const std::string& fname, int64_t ramByte);

typedef struct MinHeapNode {
    std::string str;
    uint16_t i;
} minHeapNode_t;

class MinHeap {
    std::vector<MinHeapNode> heap_;
    uint16_t heapSize_;

    public:
    explicit MinHeap(const std::vector<MinHeapNode>& heap);
    void minHeapify(uint16_t idx);

    minHeapNode_t getMin() {
        return heap_[0];
    }

    void replaceMin(const minHeapNode_t& x) {
        heap_[0] = x;
        minHeapify(0);
    }
};
#endif  // EXT_SORT_EXT_SORT_H
