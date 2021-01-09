////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "endpoints.hpp"
#include "util.hpp"

#include <fstream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
endpoints endpoints::read_from(const fs::path& file)
{
    std::fstream fs(file, std::ios::in);
    if(!fs.good()) throw std::invalid_argument("Can't open file");

    endpoints eps;

    std::string line;
    for(int n = 1; std::getline(fs, line); ++n)
    {
        std::stringstream ss(line);

        std::string ep;
        ss >> std::ws >> ep >> std::ws;
        if(ep.empty() || ep[0] == '#') continue;

        asio::ip::address address { };
        std::uint16_t port = 5250;

        auto p = ep.find(':');
        if(p != std::string::npos)
        {
            address = to_address(ep.substr(0, p));

            port = to_port(line.substr(p + 1));
            if(port == 0) throw invalid_line(n, "Invalid port #");
        }
        else address = to_address(ep);

        if(address.is_unspecified()) throw invalid_line(n, "Invalid address");

        if(!ss.eof()) throw invalid_line(n, "Garbage at end");

        eps.emplace_back(address, port);
    }

    return eps;
}

////////////////////////////////////////////////////////////////////////////////
}
