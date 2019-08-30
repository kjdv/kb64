#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace kb64 {
// convention here is that 'char' and 'std::string' are used to represent human-readable or printable data, and
// uint8_t and std::vector<uint8_t> is used for binary data.

// returns the number of bytes in t
size_t decode(char const s[4], uint8_t t[3]);
// l should be 1, 2 or 3
void encode(uint8_t const s[], size_t l, char t[4]);

std::vector<uint8_t> decode(std::string const& s);
std::string          encode(std::vector<uint8_t> const& v);

void inplace_encode(std::string& s);
void inplace_decode(std::string& s);

// utility to ensure proper trailing '=' are present, if you want to be robust
void sanitize(std::string& s);
} // namespace kb64
