//
// Hash.h
//
// Copyright (c) 2012 - 2015 JST ERATO Minato project, Jun Kawahara, and other contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef HASH_H
#define HASH_H

#include <cstdlib>
#include <iostream>

#include "Global.h"

const int64 kHashSize = (1ll << 25);

class Hash {
private:
    mate_t* hash_time_table_;
    int64* hash_table_;
    int64 size_;

public:
    Hash() : size_(kHashSize)
    {
        hash_time_table_ = (mate_t*)calloc(size_, sizeof(mate_t));
        if (hash_time_table_ == NULL) {
            std::cerr << "malloc failed for hash_time_table_!" << std::endl;
            exit(1);
        }

        hash_table_ = (int64*)malloc(size_ * sizeof(int64));
        if (hash_table_ == NULL) {
            std::cerr << "malloc failed for hash_table_!" << std::endl;
            exit(1);
        }
    }

    ~Hash()
    {
        free(hash_table_);
        free(hash_time_table_);
    }

    void Set(uint64 hash_value, mate_t time, int64 value)
    {
        hash_value %= size_;
        for (uint64 i = hash_value; i < static_cast<uint64>(size_); ++i) {
            if (hash_time_table_[i] != time) {
                hash_table_[i] = value;
                hash_time_table_[i] = time;
                return;
            }
        }
        for (uint64 i = 0; i < hash_value; ++i) {
            if (hash_time_table_[i] != time) {
                hash_table_[i] = value;
                hash_time_table_[i] = time;
                return;
            }
        }
        std::cerr << "The hash is full!" << std::endl;
        exit(1);
    }

    friend class ZDDNodeSet;
};

#endif // HASH_H
