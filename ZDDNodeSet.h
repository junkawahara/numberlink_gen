//
// ZDDNodeSet.h
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

#ifndef ZDD_NODE_SET_H
#define ZDD_NODE_SET_H

#include <vector>

#include "Global.h"

class ZDDNode;
class Hash;
class State;

class ZDDNodeSet {
private:
    std::vector<ZDDNode*> node_list_;
    Hash* global_hash_;

public:
    ZDDNodeSet(Hash* global_hash) : global_hash_(global_hash)
    {
        return;
    }

    int64 GetSize()
    {
        return static_cast<int64>(node_list_.size());
    }

    ZDDNode* Get(int64 index)
    {
        return node_list_[index];
    }

    // returns -1 if not found
    int64 Find(ZDDNode* node, State* state)
    {
        if (node == ZDDNode::ZeroNode || node == ZDDNode::OneNode) {
            return -1;
        }

        uint64 hash_value = node->GetHashValue(&state->frontier_list) % global_hash_->size_;

        while (global_hash_->hash_time_table_[hash_value] == state->edge_number + 1) {
            int64 address = global_hash_->hash_table_[hash_value];
            if (node->Equals(node_list_[address], state)) {
                return address;
            }
            hash_value = (hash_value + 1) % global_hash_->size_;
        }
        return -1ll;
    }

    void Add(ZDDNode* node, State* state)
    {
        node_list_.push_back(node);
        global_hash_->Set(node->GetHashValue(&state->frontier_list), state->edge_number + 1, node_list_.size() - 1);
    }

    void Clear()
    {
        for (uint64 i = 0; i < node_list_.size(); ++i) {
            delete node_list_[i];
        }
        node_list_.clear();
    }
};

#endif // ZDD_NODE_SET_H
