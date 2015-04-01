//
// State.h
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

#ifndef STATE_H
#define STATE_H

#include <vector>

#include "Global.h"
#include "Edge.h"

class Graph;

struct State {
private:
    std::vector<Edge> edge_list;
    int edge_number;
    int src;
    int dest;
    int number_of_vertices;
    std::vector<int> frontier_list;
    std::vector<int> leaving_frontier_list;

public:
    State(Graph* graph);
    void Update(int edge_number);
    void UpdateAdd(int edge_number);
    void UpdateRemove(int edge_number);

private:
    bool Find(int edge_number, int value);
    void AddToList(std::vector<int>* list, int value);
    void RemoveFromList(std::vector<int>* list, int value);

    friend class Mate;
    friend class ZDDNodeSet;
    friend class FrontierAlgorithm;
};

#endif // STATE_H
