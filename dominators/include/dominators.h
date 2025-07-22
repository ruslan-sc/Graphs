#pragma once

#include "graph.h"

#include <boost/dynamic_bitset.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <vector>

namespace Graphs {

using boost::adjacency_list;
using boost::bidirectionalS;
using boost::listS;
using boost::num_vertices;
using boost::vertices;

template <class PropertyT>
using dominators_graph_base =
    graphviz_graph_t<adjacency_list<listS, listS, bidirectionalS, PropertyT>>;

template <class GraphT> class dominators_map_t final {
    using bitset_t = boost::dynamic_bitset<>;
    using blocks_t = std::vector<bitset_t>;

    blocks_t blocks_;
    const GraphT *graph_;

private:
    bool search_dominators_step()
    {
        bool changed = false;
        for (auto [b_vi, b_vend] = vertices(*graph_); b_vi != b_vend; ++b_vi)
        {
            bitset_t tmp{num_vertices(*graph_), 1};
            for (auto [e_vi, e_vend] = boost::in_edges(*b_vi, *graph_);
                 e_vi != e_vend; ++e_vi)
            {
                auto pred = boost::source(*e_vi, *graph_);
                bitset_t pred_dominators = blocks_[graph_->get_id_map()[pred]];
                tmp &= pred_dominators;
            }
            auto cur_b_id = graph_->get_id_map()[*b_vi];
            tmp[cur_b_id] = 1;
            bool res = (blocks_[cur_b_id] != tmp);
            changed |= res;
            if (res)
                blocks_[cur_b_id] = tmp;
        }
        return changed;
    }

    void search_dominators()
    {
        while (search_dominators_step())
            continue;
    }

public:
    dominators_map_t(const GraphT &graph)
        : graph_(&graph),
          blocks_(num_vertices(graph), bitset_t{num_vertices(graph), 1})
    {
        if (!std::is_same_v<typename GraphT::directed_selector,
                            boost::bidirectionalS>)
            throw std::runtime_error(
                "dominators_graph is supporting only bidirectionalS graphs.");
        if (graph.get_id_map()[*vertices(graph).first] != 0)
            throw std::runtime_error("dominators_graph expects entry in the "
                                     "begin of vertices range.");
        blocks_[0] = bitset_t{num_vertices(graph), 0};
        blocks_[0][0] = 1;
        search_dominators();
    }

    auto begin() { return blocks_.begin(); }
    auto end() { return blocks_.end(); }

    blocks_t &get() { return blocks_; }

    bitset_t &operator[](size_t ind) { return blocks_[ind]; }
};

} // namespace Graphs