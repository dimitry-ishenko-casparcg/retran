////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "args.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace asio::ip;
namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////
#if !defined(VERSION)
#  define VERSION 0
#endif

#define TEXT(x) TEXT_(x)
#define TEXT_(x) #x

////////////////////////////////////////////////////////////////////////////////
std::string usage(const std::string& name)
{
    return "Usage: " + name + R"( [option] [path]

Where [path] is an optional path to an alternative config file.

Option is one or more of the following:

    --address=<addr>    Specify IP address to bind to. Default: 127.0.0.1.

    --help, -h          Print this help screen and exit.

    --port=<n>          Specify port to listen on. Default: 6250.

    --version, -v       Show version and exit.)";
}

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

        auto args = src::args::read_from(argc, argv);
        if(args.help)
        {
            std::cout << usage(name) << std::endl;
        }
        else if(args.version)
        {
            std::cout << version(name) << std::endl;
        }
        else
        {
            if(args.path.empty())
            {
                args.path = src::data_path() / name / "retran.conf";
                fs::create_directory(args.path.parent_path());

                if(!fs::exists(args.path)) std::fstream{ args.path, std::ios::out };
            }

            std::cout << "Reading configuration from " << args.path << std::endl;
            //

            if(args.address.is_unspecified()) args.address = make_address("127.0.0.1");
            if(args.port == 0) args.port = 6250;

            udp::endpoint local{ args.address, args.port };
            std::cout << "Listening on " << local << std::endl;

            asio::io_context io;
            //

            src::set_interrupt_callback([&](int signal)
            {
                std::cout << "Received signal " << signal << " - exiting" << std::endl;
                io.stop();
            });

            std::cout << "Starting event loop" << std::endl;
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
