#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

namespace Graphs {

using boost::dynamic_properties;
using boost::graph_traits;
using boost::property_map;

template <class AdjacencyListT> class graphviz_graph_t : public AdjacencyListT {
    using id_map_t = property_map<AdjacencyListT, boost::vertex_index_t>::type;
    using vert_iterator = graph_traits<AdjacencyListT>::vertex_iterator;
    using vert_size_t = graph_traits<AdjacencyListT>::vertices_size_type;

    dynamic_properties dp_;
    id_map_t id_map_;

private:
    void initialize_dynamic_properties()
    {
        id_map_ = boost::get(boost::vertex_index, *this);
        dp_.property("node_id", id_map_);
    }

    template <class DscT> bool has_in_edges(DscT &vert) const
    {
        auto [fst, lst] = in_edges(vert, *this);
        return fst != lst;
    }

public:
    graphviz_graph_t() : AdjacencyListT(), dp_()
    {
        initialize_dynamic_properties();
    }

    graphviz_graph_t(vert_size_t sz) : AdjacencyListT(sz), dp_()
    {
        initialize_dynamic_properties();
    }

    template <class PropertyMapT>
    dynamic_properties &property(const std::string &name,
                                 PropertyMapT property_map)
    {
        return dp_.property(name, property_map);
    }

    template <class PropertyMapT>
    dynamic_properties property(const std::string &name,
                                PropertyMapT property_map) const
    {
        return dp_.property(name, property_map);
    }

    bool read_graphviz(std::istream &in)
    {
        if (!boost::read_graphviz(in, *this, dp_))
            return false;

        vert_iterator begin, end;
        boost::tie(begin, end) = boost::vertices(*this);

        // 0 is a entry.
        auto it = std::find_if(begin, end,
                               [&](auto &id) { return id_map_[id] == 0; });
        if (it == end)
            throw std::runtime_error("No entry in graphviz file.");
        if (has_in_edges(*it))
            throw std::runtime_error("Entry can't have predcessors.");

        std::swap(*it, *begin);
        return true;
    }

    void write_graphviz(std::ostream &out)
    {
        boost::write_graphviz(out, *this);
    }

    std::pair<vert_iterator, vert_iterator> vertices() const
    {
        return boost::vertices(*this);
    }

    const id_map_t &get_id_map() const { return id_map_; }

public:
    graphviz_graph_t &operator=(const graphviz_graph_t &) = default;
    graphviz_graph_t(const graphviz_graph_t &) = default;

    virtual ~graphviz_graph_t() = default;
};

} // namespace Graphs