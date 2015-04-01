//
// Graph.cpp
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

#include "Graph.h"

using namespace std;

void Graph::LoadAdjacencyList(istream& ist)
{
    number_of_vertices_ = 0;

    string s;
    while (std::getline(ist, s)) {
        ++number_of_vertices_;
        istringstream iss(s);
        int x;
        while (iss >> x) {
            if (number_of_vertices_ < x) {
                edge_list_.push_back(Edge(number_of_vertices_, x));
            }
        }
    }
}

void Graph::LoadEdgeList(istream& ist)
{
    string s;
    std::getline(ist, s);
    istringstream issx(s);
    issx >> number_of_vertices_;

    while (std::getline(ist, s)) {
        istringstream iss(s);
        int src, dest;
        iss >> src >> dest;
        edge_list_.push_back(Edge(src, dest));
    }
}

uint64 Graph::GetNeighbor(int vertex)
{
    uint64 neighbor = 0;
    for (unsigned int i = 0; i < edge_list_.size(); ++i) {
        if (edge_list_[i].src == vertex) {
            neighbor |= (1u << (edge_list_[i].dest - 1));
        }
    }
    return neighbor;
}

vector<int> Graph::GetAdjacentList(int vertex, bool need_larger)
{
    vector<int> adjacent_list;
    
    for (unsigned int i = 0; i < edge_list_.size(); ++i) {
        if (edge_list_[i].src == vertex) {
            if (!need_larger || edge_list_[i].dest > vertex) {
                adjacent_list.push_back(edge_list_[i].dest);
            }
        }
        if (edge_list_[i].dest == vertex) {
            if (!need_larger || edge_list_[i].src > vertex) {
                adjacent_list.push_back(edge_list_[i].src);
            }
        }
    }
    return adjacent_list;
}
