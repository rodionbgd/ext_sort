#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <ctime>
#include <string>

#include "ext_sort.h"

#define R_OK    1
#define R_ERROR 0

__attribute__((unused)) uint8_t generateFile(int64_t ramByte) {
    FILE* fp;
    uint64_t fSize = ramByte * 4;
    fp             = fopen("test.txt", "w");
    srandom(std::time(nullptr));
    for(int64_t i = 0; i < fSize;) {
        uint8_t strLen = rand() % (15 - 3) + 3;  // NOLINT(cert-msc30-c, cert-msc50-cpp)
        if(i + strLen > fSize) {
            strLen = fSize - i + 1;
        }
        for(uint8_t j = 0; j < strLen - 1; j++) {
            const char c = (char) ((random() % (('z' - 'a') + 1)) + 'a');
            fputc(c, fp);
        }
        i += strLen;
        if(i < fSize) {
            fputc('\n', fp);
        }
    }
    fclose(fp);
    return 1;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        return R_ERROR;
    }
    std::string fname = argv[1];
    const char* endptr;
    const int64_t ramByte = strtol(argv[2], (char**) &endptr, 10);
    if(ramByte == 0) {
        if(errno == EINVAL) {
            return R_ERROR;
        }
    }
    if(ramByte == LONG_MIN || ramByte == LONG_MAX) {
        if(errno == ERANGE) {
            return R_ERROR;
        }
    }
    //    generateFile(ramByte);
    extSort(fname, ramByte);
    return R_OK;
}
