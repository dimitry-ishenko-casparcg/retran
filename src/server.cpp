////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "server.hpp"

#include <exception>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
server::server(asio::io_context& io, const udp::endpoint& local, endpoints remote) :
    recv_{ io }, send_{ io }, remote_{ std::move(remote) }
{
    recv_.open(udp::v4());
    recv_.bind(local);

    send_.open(udp::v4());

    async_recv();
}

////////////////////////////////////////////////////////////////////////////////
void server::async_recv()
{
    recv_.async_wait(udp::socket::wait_read, [=](const asio::error_code& ec)
    {
        if(!ec)
        {
            std::vector<char> data(recv_.available());
            auto buffer{ asio::buffer(data) };
            udp::endpoint ep;

            try
            {
                recv_.receive_from(buffer, ep);
                for(auto const& ep : remote_) send_.send_to(buffer, ep);
            }
            catch(std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            async_recv();
        }
    });

}

////////////////////////////////////////////////////////////////////////////////
}
