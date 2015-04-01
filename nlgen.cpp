//
// nlgen.cpp
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

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "SAPPOROBDD/ZBDD.h"
#include "Global.h"
#include "Edge.h"
#include "Graph.h"
#include "State.h"
#include "Mate.h"
#include "Hint.h"
#include "ZDDNode.h"
#include "Hash.h"
#include "ZDDNodeSet.h"
#include "TimeCount.h"

using namespace std;

int64 ZDDNode::total_serial_;
ZDDNode* ZDDNode::ZeroNode;
ZDDNode* ZDDNode::OneNode;


class FrontierAlgorithm {
public:
    static void Construct(Graph* graph, int max_pair_num);

private:
    static ZDDNode* MakeChildNode(ZDDNode* node, State* state, int lo_or_hi);
    static void PrintMate(ZDDNode* node);
};

void FrontierAlgorithm::Construct(Graph* graph, int max_pair_num)
{
    int n = graph->GetNumberOfVertices();
    vector<Edge> edge_list = graph->GetEdgeList();

    ZBDD* zbdd_array = new ZBDD[(n + 1) * (n + 1)];

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            int var_index = BDD_NewVar();
            zbdd_array[i * n + j] = ZBDD(1).Change(var_index);
        }
    }
    Hint::Initialize(n, zbdd_array, max_pair_num);

    Hash global_hash;
    State state(graph);
    ZDDNodeSet* current_node_set = new ZDDNodeSet(&global_hash);
    ZDDNodeSet* next_node_set = new ZDDNodeSet(&global_hash);

    current_node_set->Add(ZDDNode::MakeInitialNode(n), &state);

    int state_counter = 0;

    for (int v = 1; v <= n; ++v) {
        vector<int> adj_list = graph->GetAdjacentList(v, true);
        for (uint x = 0; x < adj_list.size(); ++x) {
            int w = adj_list[x];
            state.UpdateAdd(state_counter);
            ++state_counter;

            for (int64 j = 0; j < current_node_set->GetSize(); ++j) {
                ZDDNode* node = current_node_set->Get(j);
                ZDDNode* child_node[2];

                for (int lo_or_hi = 0; lo_or_hi <= 1; ++lo_or_hi) {
                    child_node[lo_or_hi] = MakeChildNode(node, &state, lo_or_hi);

                    if (lo_or_hi == 1 && child_node[lo_or_hi] != ZDDNode::ZeroNode) {
                        int ms = node->GetValue(state.src);
                        int me = node->GetValue(state.dest);
                        if (ms < v && me < v) {
                            child_node[lo_or_hi]->hint().MultiplyVariable(                                                                          zbdd_array[min(ms, me) * n + max(ms, me)]);
                        }
                    }

                    int64 index = next_node_set->Find(child_node[lo_or_hi], &state);
                    if (index >= 0) {
                        ZDDNode* found_node = next_node_set->Get(index);

                        found_node->hint().Merge(&child_node[lo_or_hi]->hint());

                        delete child_node[lo_or_hi];
                        child_node[lo_or_hi] = next_node_set->Get(index);

                    } else if (child_node[lo_or_hi] != ZDDNode::ZeroNode
                            && child_node[lo_or_hi] != ZDDNode::OneNode) {
                        next_node_set->Add(child_node[lo_or_hi], &state);
                    }
                }
                node->SetNode(child_node[0], child_node[1]);
            }
            current_node_set->Clear();
            delete current_node_set;
            current_node_set = next_node_set;
            next_node_set = new ZDDNodeSet(&global_hash);
        }

        state.UpdateRemove(v);
        for (int j = 0; j < current_node_set->GetSize(); ++j) {
            ZDDNode* node = current_node_set->Get(j);
            int w = node->GetValue(v);
            
            int64 index = next_node_set->Find(node, &state);
            if (index >= 0) {
                ZDDNode* found_node = next_node_set->Get(index);
                if (w == 0 || w > v) {
                    found_node->hint().Merge(&node->hint());
                } else if (w == v) {
                    found_node->hint().DisableAndMerge(&node->hint());
                } else {
                    found_node->hint().MultiplyAndMerge(&node->hint(), zbdd_array[w * n + v]);
                }
            } else {
                if (w == 0 || w > v) {
                    // nothing
                } else if (w == v) {
                    node->hint().Disable();
                } else {
                    node->hint().MultiplyVariable(zbdd_array[w * n + v]);
                }
                next_node_set->Add(node, &state);
            }
        }
        delete current_node_set;
        current_node_set = next_node_set;
        next_node_set = new ZDDNodeSet(&global_hash);
    }

    ZBDD fa = current_node_set->Get(0)->hint().GetLiveZDD();
    ZBDD fb = current_node_set->Get(0)->hint().GetDeadZDD();

    ZBDD f = fa - fb;

    cerr << "# of solutions = " << f.Card() << endl;
    cerr << "# of ZDD nodes = " << f.Size() << endl;
}

ZDDNode* FrontierAlgorithm::MakeChildNode(ZDDNode* node, State* state, int lo_or_hi)
{
    int c = node->CheckBadOrComplete(state, lo_or_hi);
    if (c == 0) {
        return ZDDNode::ZeroNode;
    } else if (c == 1) {
        return ZDDNode::OneNode;
    }
    ZDDNode* child_node = node->Clone(state->number_of_vertices);
    
    if (lo_or_hi == 1) {
        child_node->ConnectEdge(state->src, state->dest);
    }
    
    return child_node;
}

void FrontierAlgorithm::PrintMate(ZDDNode* node)
{
    int n = 4;
    cout << "[";
    for (int i = 1; i <= n; ++i) {
        cout << node->GetValue(i);
        if (i < n) {
            cout << ", ";
        }
    }
    cout << "]";
}


int main(int argc, char** argv)
{
#ifdef B_64
    BDD_Init(1000000ll, 3000000000ll);
#else
    BDD_Init(1000000, 800000000);
#endif

    bool use_edge_list = false;
    int max_pair_num = -1;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-c") {
            use_edge_list = true;
        } else if (string(argv[i]) == "-m") {
            if (i + 1 < argc) {
                max_pair_num = atoi(argv[i + 1]);
            }
            ++i;
        }
    }
#ifdef ZDD_ARRAY
    if (max_pair_num < 0) {
        std::cerr << "max_pair_num must be at least 1." << std::endl;
        return 1;
    }
#endif

    Graph graph;
    if (use_edge_list) {
        graph.LoadEdgeList(cin);
    } else {
        graph.LoadAdjacencyList(cin);
    }

    cerr << "# of vertices = " << graph.GetNumberOfVertices()
         << ", # of edges = " << graph.GetEdgeList().size() << endl;

    ZDDNode::Initialize();
    cerr << "begin enumeration..." << endl;

    TimeCount start_tc, end_tc;
    start_tc = GetProcessTime();

    FrontierAlgorithm::Construct(&graph, max_pair_num); // run the algorithm

    end_tc = GetProcessTime();

    cerr << "end enumeration" << endl;
    cerr << "time = " << GetUserTime(start_tc, end_tc) << " sec." << endl;
    
    return 0;
}
