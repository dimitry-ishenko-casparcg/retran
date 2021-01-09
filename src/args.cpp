////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "args.hpp"
#include "util.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

bool starts_with(const std::string& s, const std::string& prefix)
{
    return s.compare(0, prefix.size(), prefix) == 0;
}

}

////////////////////////////////////////////////////////////////////////////////
args args::read_from(int argc, char* argv[])
{
    src::args args;

    for(int n = 1; n < argc; ++n)
    {
        std::string arg = argv[n];
        if(arg.empty()) continue;

        if(arg == "--address")
        {
            throw missing_value(arg);
        }
        else if(starts_with(arg, "--address="))
        {
            if(args.address.is_unspecified())
            {
                auto s = arg.substr(sizeof("--address=") - 1);
                args.address = to_address(s);
                if(args.address.is_unspecified()) throw invalid_value(arg);
            }
            else throw duplicate_option(arg);
        }
        else if(arg == "-h" || arg == "--help")
        {
            args.help = true;
            return args;
        }
        else if(arg == "--port")
        {
            throw missing_value(arg);
        }
        else if(starts_with(arg, "--port="))
        {
            if(args.port == 0)
            {
                auto s = arg.substr(sizeof("--port=") - 1);
                args.port = to_port(s);
                if(args.port == 0) throw invalid_value(arg);
            }
            else throw duplicate_option(arg);
        }
        else if(arg == "-v" || arg == "--version")
        {
            args.version = true;
            return args;
        }
        else if(arg[0] == '-')
        {
            throw invalid_option(arg);
        }
        else
        {
            if(args.path.empty()) args.path = arg;
            else throw extra_param(arg);
        }
    }

    return args;
}

////////////////////////////////////////////////////////////////////////////////
}
