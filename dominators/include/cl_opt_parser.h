#pragma once

#include <boost/program_options.hpp>
#include <stdexcept>

namespace CLOptions {

using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;

class cl_parser_t final : private variables_map {
    using opts_desc = boost::program_options::options_description;

private:
    int argc_;
    char **argv_;

    opts_desc od_;

public:
    cl_parser_t(int argc, char **argv, const opts_desc &od)
        : boost::program_options::variables_map(), argc_(argc), argv_(argv),
          od_(od)
    {
        if (argc < 0)
            throw std::runtime_error("Invalid cl_parser_t argc argument.");
        if (argc && (!argv || !*argv))
            throw std::runtime_error("Invalid cl_parser_t argv argument.");

        boost::program_options::store(
            boost::program_options::parse_command_line(argc_, argv_, od_),
            *this);
    }

    using variables_map::count;
    using variables_map::operator[];
};

} // namespace CLOptions