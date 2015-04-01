//
// ZDDNode.h
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

#ifndef ZDD_NODE_H
#define ZDD_NODE_H

#include "Global.h"
#include "Mate.h"
#include "Hint.h"

class State;

// Before using this class, ZDDNode::Initialize() must be called once.
class ZDDNode {
private:
    int64 serial_;
    Mate mate_;
    ZDDNode* lo_node_;
    ZDDNode* hi_node_;
    Hint hint_;

    static int64 total_serial_;

public:
    static ZDDNode* ZeroNode; // 0-terminal
    static ZDDNode* OneNode;  // 1-terminal

    // number_of_vertices: number of vertices of the universe graph
    ZDDNode(int number_of_vertices) : mate_(number_of_vertices)
    {
        lo_node_ = NULL;
        hi_node_ = NULL;
        serial_ = total_serial_;
        ++total_serial_;
    
        hint_.SetInit();
    }

    // We assume that only the latest ZDDNode will be deleted.
    ~ZDDNode()
    {
        --total_serial_;
    }

    Hint& hint()
    {
        return hint_;
    }

    int64 GetSerial()
    {
        return serial_;
    }

    void SetNode(ZDDNode* lo_node, ZDDNode* hi_node)
    {
        lo_node_ = lo_node;
        hi_node_ = hi_node;
    }

    // serial_ is not be cloned.
    ZDDNode* Clone(int number_of_nodes)
    {
        ZDDNode* node = new ZDDNode(number_of_nodes);
        node->mate_.Copy(&mate_, number_of_nodes);
        node->hint_ = hint_;
        return node;
    }

    void ConnectEdge(int src, int dest)
    {
        mate_.ConnectEdge(src, dest);
    }

    bool Equals(ZDDNode* node, State* state)
    {
        if (node == ZeroNode || node == OneNode || this == ZeroNode || this == OneNode) {
            return node == this;
        } else {
            return mate_.Equals(&node->mate_, state);
        }
    }

    int CheckBadOrComplete(State* state, int lo_or_hi)
    {
        return mate_.CheckBadOrComplete(state, lo_or_hi);
    }

    bool CheckLeaving(State* state)
    {
        return mate_.CheckLeaving(state);
    }

    uint64 GetHashValue(std::vector<int>* frontier_list)
    {
        return mate_.GetHashValue(frontier_list);
    }

    mate_t GetValue(int index)
    {
        return mate_.GetValue(index);
    }

    void SetValue(int index, mate_t v)
    {
        mate_.SetValue(index, v);
    }

    static ZDDNode* MakeInitialNode(int number_of_vertices)
    {
        ZDDNode* initial_node = new ZDDNode(number_of_vertices);
        initial_node->mate_.Initialize(number_of_vertices);
        return initial_node;
    }

    static void Initialize()
    {
        ZeroNode = new ZDDNode(0);
        ZeroNode->serial_ = 0;
        OneNode = new ZDDNode(0);
        OneNode->serial_ = 1;
        total_serial_ = 2;
    }
};

#endif // ZDD_NODE_H
