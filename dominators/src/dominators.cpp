#include "dominators.h"
#include "cl_opt_parser.h"
#include "graph.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    try
    {
        CLOptions::options_description od("Allowed options");
        std::string src_file_name, dst_file_name = "res.dot";
        od.add_options()("source,s", CLOptions::value(&src_file_name),
                         "graphviz file that is containing a graph")(
            "output,o", CLOptions::value(&dst_file_name),
            "file to output result graph");

        CLOptions::cl_parser_t parser(argc, argv, od);
        if (!parser.count("source"))
        {
            std::cerr << "Error: source file is not specified.\n";
            return 1;
        }
        src_file_name = parser["source"].as<std::string>();
        if (parser.count("output"))
            dst_file_name = parser["output"].as<std::string>();

        using namespace boost;
        using graph_t = adjacency_list<listS, listS, bidirectionalS,
                                       property<vertex_index_t, int>>;
        Graphs::graphviz_graph_t<graph_t> g;
        std::ifstream stream(src_file_name);
        if (!stream.good())
        {
            std::cerr << "Error: can't open " << src_file_name << ".\n";
            return 1;
        }
        g.read_graphviz(stream);
        boost::graph_traits<graph_t>::vertex_iterator vi, ve;
        for (boost::tie(vi, ve) = vertices(g); vi != ve; ++vi)
            std::cout << g.get_id_map()[*vi] << " ";
        std::cout << std::endl;

        Graphs::dominators_map_t dm(g);
        for (size_t i = 0; i < num_vertices(g); ++i)
        {
            std::cout << "Vertex " << i << " dominators: ";
            for (size_t j = 0; j < dm[i].size(); ++j)
                if (dm[i][j])
                    std::cout << j << " ";
            std::cout << std::endl;
        }
        return 0;
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
}