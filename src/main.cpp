////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "endpoints.hpp"
#include "pgm/args.hpp"
#include "server.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////
#if !defined(VERSION)
#  define VERSION 0
#endif

#define TEXT(x) TEXT_(x)
#define TEXT_(x) #x

////////////////////////////////////////////////////////////////////////////////
std::string version(const std::string& name)
{
    return name + " version " TEXT(VERSION);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int exit_code = 0;
    try
    {
        auto name = fs::path{ argv[0] }.filename();
        pgm::args args
        {
            { "-a", "--address", "addr", "Specify IP address to bind to. Default: 127.0.0.1." },
            { "-p", "--port", "N",       "Specify port # to listen on. Default: 6260." },
            { "-h", "--help",            "Print this help screen and exit."     },
            { "-v", "--version",         "Show version number and exit."        },
            { "endpoints?",              "Path to alternate endpoints file."    },
        };
        args.parse(argc, argv);

        if(args["--help"])
            std::cout << args.usage(name) << std::endl;

        else if(args["--version"])
            std::cout << version(name) << std::endl;

        else
        {
            fs::path path{ args["endpoints"].value_or("") };
            if(path.empty())
            {
                path = src::data_path() / name / "endpoints.conf";

                fs::create_directory(path.parent_path());
                if(!fs::exists(path)) std::fstream{ path, std::ios::out };
            }

            std::cout << "Reading endpoints from " << path << "." << std::endl;
            auto remote = src::endpoints::read_from(path);

            if(remote.size())
            {
                std::cout << "Remote endpoints: ";
                for(auto const& ep : remote) std::cout << ep << ", ";
                std::cout << std::endl;
            }

            auto s{ args["--address"].value_or("127.0.0.1") };
            auto address{ src::to_address(s) };

            if(address.is_unspecified())throw pgm::invalid_argument{ "Invalid IP address", s };

            s = args["--port"].value_or("6250");
            auto port{ src::to_port(s) };

            if(port == 0) throw pgm::invalid_argument{ "Invalid port #", s };

            asio::ip::udp::endpoint local{ address, port };
            std::cout << "Listening on " << local << "." << std::endl;

            asio::io_context io;
            src::server server{ io, local, remote };

            src::set_interrupt_callback([&](int signal)
            {
                std::cout << "Received signal " << signal << " - exiting." << std::endl;
                io.stop();
            });

            std::cout << "Starting event loop." << std::endl;
            io.run();
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit_code = 1;
    }
    catch(...)
    {
        std::cerr << "???" << std::endl;
        exit_code = 1;
    }
    return exit_code;
}
