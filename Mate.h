//
// Mate.h
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

#ifndef MATE_H
#define MATE_H

#include <vector>

#include "Global.h"

class State;

class Mate {
private:
    mate_t* mate_array_;

public:
    Mate(int number_of_vertices);
    ~Mate();

    mate_t GetValue(int index)
    {
        return mate_array_[index];
    }

    void SetValue(int index, mate_t v)
    {
        mate_array_[index] = v;
    }

    void Initialize(int number_of_vertices);
    void Copy(Mate* mate, int number_of_vertices);
    void ConnectEdge(int src, int dest);
    bool Equals(Mate* mate, State* state);
    int CheckBadOrComplete(State* state, int lo_or_hi); // returns bad: 0, complete: 1, other: -1
    bool CheckLeaving(State* state);
    uint64 GetHashValue(std::vector<int>* frontier_list);
};

#endif // MATE_H
