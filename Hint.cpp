//
// Hint.cpp
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

#include "Hint.h"

using namespace std;

vector<ZBDD> Hint::k_of_n_zdd_list_;
int Hint::max_pair_num_;

ZBDD Hint::GetLiveZDD()
{
#ifdef ZDD_ARRAY
    return live_zdd_array_[max_pair_num_];
#else
    return live_zdd;
#endif
}

ZBDD Hint::GetDeadZDD()
{
#ifdef ZDD_ARRAY
    return dead_zdd_array_[max_pair_num_];
#else
    return dead_zdd;
#endif
}

void Hint::MultiplyVariable(ZBDD variable)
{
#ifdef ZDD_ARRAY
    for (int i = max_pair_num_; i > 0; --i) {
        live_zdd_array_[i] = live_zdd_array_[i - 1] * variable;
        dead_zdd_array_[i] = dead_zdd_array_[i - 1] * variable;
    }
#else
    live_zdd *= variable;
    dead_zdd *= variable;

    if (max_pair_num_ >= 0) {
        live_zdd -= k_of_n_zdd_list_[max_pair_num_ + 1];
        dead_zdd -= k_of_n_zdd_list_[max_pair_num_ + 1];
    }
#endif
}

void Hint::Merge(Hint* hint)
{
#ifdef ZDD_ARRAY
    for (int i = 0; i <= max_pair_num_; ++i) {
        ZBDD temp_live_zdd = live_zdd_array_[i];
        ZBDD temp_dead_zdd = dead_zdd_array_[i];

        live_zdd_array_[i] = ((temp_live_zdd - hint->live_zdd_array_[i]) + (hint->live_zdd_array_[i] - temp_live_zdd)) - (temp_dead_zdd + hint->dead_zdd_array_[i]);
        dead_zdd_array_[i] = (temp_live_zdd & hint->live_zdd_array_[i]) + (temp_dead_zdd + hint->dead_zdd_array_[i]);
    }
#else
    ZBDD temp_live_zdd = live_zdd;
    ZBDD temp_dead_zdd = dead_zdd;

    live_zdd = ((temp_live_zdd - hint->live_zdd) + (hint->live_zdd - temp_live_zdd)) - (temp_dead_zdd + hint->dead_zdd);
    dead_zdd = (temp_live_zdd & hint->live_zdd) + (temp_dead_zdd + hint->dead_zdd);
#endif
}

void Hint::DisableAndMerge(Hint* hint)
{
#ifdef ZDD_ARRAY
    for (int i = 0; i <= max_pair_num_; ++i) {
        ZBDD temp_dead_zdd = dead_zdd_array_[i];

        live_zdd_array_[i] -= hint->live_zdd_array_[i] + temp_dead_zdd + hint->dead_zdd_array_[i];
        dead_zdd_array_[i] += hint->live_zdd_array_[i] + hint->dead_zdd_array_[i];
    }
#else
    ZBDD temp_dead_zdd = dead_zdd;

    live_zdd -= hint->live_zdd + temp_dead_zdd + hint->dead_zdd;
    dead_zdd += hint->live_zdd + hint->dead_zdd;
#endif
}

void Hint::MultiplyAndMerge(Hint* hint, ZBDD variable)
{
    Hint temp_hint(*hint);
    temp_hint.MultiplyVariable(variable);

    Merge(&temp_hint);
}

void Hint::Disable()
{
#ifdef ZDD_ARRAY
    for (int i = 0; i <= max_pair_num_; ++i) {
        dead_zdd_array_[i] += live_zdd_array_[i];
        live_zdd_array_[i] = ZBDD(0);
    }
#else
    dead_zdd += live_zdd;
    live_zdd = ZBDD(0);
#endif
}

void Hint::SetInit()
{
#ifdef ZDD_ARRAY
    for (int i = 0; i <= max_pair_num_; ++i) {
        live_zdd_array_.push_back(ZBDD(0));
        dead_zdd_array_.push_back(ZBDD(0));
    }
    live_zdd_array_[0] = ZBDD(1);
#else
    live_zdd = ZBDD(1);
    dead_zdd = ZBDD(0);
#endif
}

void Hint::Initialize(int n, ZBDD* zbdd_array, int max_pair_num)
{
    max_pair_num_ = max_pair_num;
    k_of_n_zdd_list_.resize(n * (n - 1) / 2 + 1);
    k_of_n_zdd_list_[0] = ZBDD(1);
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            for (int k = n * (n - 1) / 2; k > 0; --k) {
                k_of_n_zdd_list_[k] += k_of_n_zdd_list_[k - 1] * zbdd_array[i * n + j];
            }
        }
    }
}
