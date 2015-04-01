//
// State.cpp
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

#include "State.h"
#include "Graph.h"

using namespace std;

State::State(Graph* graph)
{
    edge_list = graph->GetEdgeList();
    number_of_vertices = graph->GetNumberOfVertices();
}

void State::Update(int edge_number)
{
    this->edge_number = edge_number;
    src = edge_list[edge_number].src;
    dest = edge_list[edge_number].dest;
    
    leaving_frontier_list.clear();
    
    AddToList(&frontier_list, src);
    AddToList(&frontier_list, dest);
    
    if (!Find(edge_number, src)) {
        leaving_frontier_list.push_back(src);
        RemoveFromList(&frontier_list, src);
    }
    if (!Find(edge_number, dest)) {
        leaving_frontier_list.push_back(dest);
        RemoveFromList(&frontier_list, dest);
    }
}

void State::UpdateAdd(int edge_number)
{
    this->edge_number = edge_number;
    src = edge_list[edge_number].src;
    dest = edge_list[edge_number].dest;

    leaving_frontier_list.clear();

    AddToList(&frontier_list, src);
    AddToList(&frontier_list, dest);
}

void State::UpdateRemove(int vertex)
{
    leaving_frontier_list.push_back(vertex);
    RemoveFromList(&frontier_list, vertex);

    this->edge_number = vertex + 10000;
}

bool State::Find(int edge_number, int value)
{
    for (unsigned int i = edge_number + 1; i < edge_list.size(); ++i) {
        if (value == edge_list[i].src || value == edge_list[i].dest) {
            return true;
        }
    }
    return false;
}

void State::AddToList(vector<int>* list, int value)
{
    for (unsigned int i = 0; i < list->size(); ++i) {
        if ((*list)[i] == value) {
            return;
        }
    }
    list->push_back(value);
}

void State::RemoveFromList(vector<int>* list, int value)
{
    for (unsigned int i = 0; i < list->size(); ++i) {
        if ((*list)[i] == value) {
            for (unsigned int j = i; j < list->size() - 1; ++j) {
                (*list)[j] = (*list)[j + 1];
            }
            list->resize(list->size() - 1);
            return;
        }
    }
}
