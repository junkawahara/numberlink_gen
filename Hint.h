//
// Hint.h
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

#ifndef HINT_H
#define HINT_H

#include <vector>
#include <string>

#include "Global.h"
#include "SAPPOROBDD/ZBDD.h"

// Before using this class, Hint::Initialize() must be called once.
class Hint {
private:

#ifdef ZDD_ARRAY
    std::vector<ZBDD> live_zdd_array_;
    std::vector<ZBDD> dead_zdd_array_;
#else
    ZBDD live_zdd;
    ZBDD dead_zdd;
#endif

    static std::vector<ZBDD> k_of_n_zdd_list_;
    static int max_pair_num_;

public:
    ZBDD GetLiveZDD();
    ZBDD GetDeadZDD();
    void MultiplyVariable(ZBDD variable);
    void Merge(Hint* hint);
    void DisableAndMerge(Hint* hint);
    void MultiplyAndMerge(Hint* hint, ZBDD variable);
    void Disable();
    void SetInit();

    //static Hint Merge(ZBDD live1, ZBDD live2, ZBDD dead1, ZBDD dead2);
    static void Initialize(int n, ZBDD* zbdd_array, int max_pair_num);
};



#endif // HINT_H
