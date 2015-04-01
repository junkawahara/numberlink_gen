//
// Mate.cpp
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

#include <cstdlib>

#include "Mate.h"
#include "State.h"

using namespace std;

Mate::Mate(int number_of_vertices)
{
    if (number_of_vertices > 0) {
        mate_array_ = new mate_t[number_of_vertices + 1];
    } else {
        mate_array_ = NULL;
    }
}

Mate::~Mate()
{
    if (mate_array_ != NULL) {
        delete[] mate_array_;
    }
}

// initialize the mate (for only the root node)
void Mate::Initialize(int number_of_vertices)
{
    for (int i = 1; i <= number_of_vertices; ++i) {
        mate_array_[i] = i;
    }
}

void Mate::Copy(Mate* mate, int number_of_vertices)
{
    for (int i = 1; i <= number_of_vertices; ++i) {
        mate_array_[i] = mate->mate_array_[i];
    }
}

// src and dest are endpoints of the edge we are processing
void Mate::ConnectEdge(int src, int dest)
{
    int sm = mate_array_[src];
    int dm = mate_array_[dest];

    mate_array_[src] = 0;
    mate_array_[dest] = 0;
    mate_array_[sm] = dm;
    mate_array_[dm] = sm;
}

bool Mate::Equals(Mate* mate, State* state)
{
    // compare mate values for each vertex in the frontier
    for (unsigned int i = 0; i < state->frontier_list.size(); ++i) {
        mate_t v = state->frontier_list[i];
        if (mate_array_[v] != mate->mate_array_[v]) {
            return false;
        }
    }
    return true;
}

// This method returns 0 if the child should be 0,
// returns 1 if it should be 1. Otherwise returns -1.
// lo_or_hi == 0 (or 1) means the check for 0-child (or 1-child)
int Mate::CheckBadOrComplete(State* state, int lo_or_hi)
{
    if (lo_or_hi == 0) {
        return -1;
    } else {
        if (mate_array_[state->src] == 0 || mate_array_[state->dest] == 0) { // branch occurs
            return 0;
        }
        else {
            return -1;
        }
    }
}

// Check whether the node should be 0.
// If so, this method returns false.
bool Mate::CheckLeaving(State* state)
{
    for (uint i = 0; i < state->leaving_frontier_list.size(); ++i) {
        mate_t v = state->leaving_frontier_list[i];
        if (mate_array_[v] != 0 && mate_array_[v] != v) {
            return false;
        }
    }
    return true;
}

uint64 Mate::GetHashValue(vector<int>* frontier_list)
{
    uint64 hash_value = 0;
    for (uint i = 0; i < frontier_list->size(); ++i) {
        hash_value = hash_value * 15284356289ll + mate_array_[(*frontier_list)[i]];
    }
    return hash_value;
}
