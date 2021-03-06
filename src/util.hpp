////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SRC_UTIL_HPP
#define SRC_UTIL_HPP

////////////////////////////////////////////////////////////////////////////////
#include <asio.hpp>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>

namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
asio::ip::address to_address(const std::string&);
std::uint16_t to_port(const std::string&);

////////////////////////////////////////////////////////////////////////////////
using interrupt_callback = std::function<void (int)>;

void set_interrupt_callback(interrupt_callback);

////////////////////////////////////////////////////////////////////////////////
fs::path data_path();

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
