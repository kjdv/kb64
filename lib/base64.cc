#include <base64.hh>

#include <algorithm>
#include <cassert>

namespace kb64 {

using namespace std;

namespace {

const char g_base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

uint8_t inverse(char c)
{
  if(c >= 'A' && c <= 'Z')
  {
    return c - 'A';
  }
  else if(c >= 'a' && c <= 'z')
  {
    return (c - 'a') + 26;
  }
  else if(c >= '0' && c <= '9')
  {
    return (c - '0') + 52;
  }
  else if(c == '+')
  {
    return 62;
  }
  else if(c == '/')
  {
    return 63;
  }
  return 0;
}
} // namespace

size_t decode(char const s[4], uint8_t t[3])
{
  uint8_t c0 = inverse(s[0]);
  uint8_t c1 = inverse(s[1]);
  uint8_t c2 = inverse(s[2]);
  uint8_t c3 = inverse(s[3]);

  t[0] = (c0 << 2) | (c1 >> 4);
  t[1] = ((c1 & 0x0f) << 4) | (c2 >> 2);
  t[2] = ((c2 & 0x03) << 6) | (c3 & 0x3f);

  if(s[2] == '=')
    return 1;
  else if(s[3] == '=')
    return 2;
  else
    return 3;
}

void encode(uint8_t const s[], size_t l, char t[4])
{
  assert(l > 0 && l < 4);

  uint8_t c0 = s[0];
  uint8_t c1 = l > 1 ? s[1] : 0;
  uint8_t c2 = l > 2 ? s[2] : 0;

  t[0] = g_base64[c0 >> 2];
  t[1] = g_base64[((c0 & 0x03) << 4) | (c1 >> 4)];
  t[2] = l > 1 ? g_base64[((c1 & 0x0f) << 2) | (c2 >> 6)] : '=';
  t[3] = l > 2 ? g_base64[c2 & 0x3f] : '=';
}

vector<uint8_t> decode(string const& e)
{
  assert((e.size() & 3) == 0); // should be a multiple of 4

  vector<uint8_t> v;
  v.reserve(e.size() * 3 / 4);

  for(string::size_type i = 0; i < e.size(); i += 4)
  {
    char const* s = e.data() + i;
    uint8_t     t[3];
    size_t      l = decode(s, t);
    v.insert(v.end(), t, t + l);
  }

  return v;
}

string encode(vector<uint8_t> const& v)
{
  string e;
  e.reserve(v.size() * 4 / 3);

  for(vector<uint8_t>::size_type i = 0; i < v.size(); i += 3)
  {
    size_t l = min<size_t>(v.size() - i, 3);
    char   t[4];
    encode(&v[i], l, t);
    e.append(t, t + 4);
  }

  return e;
}

void inplace_encode(string& str)
{
  string::size_type i = 0;
  while(i < str.size())
  {
    size_t         l = min<size_t>(str.size() - i, 3);
    uint8_t const* s = reinterpret_cast<uint8_t const*>(str.data()) + i;
    char           t[4];
    encode(s, l, t);
    str.replace(i, l, t, 4);

    i += 4;
  }
}

void inplace_decode(string& str)
{
  assert((str.size() & 3) == 0); // should be a multiple of 4

  string::size_type i = 0;
  while(i < str.size())
  {
    char const* s = str.data() + i;
    uint8_t     t[3];
    size_t      l = decode(s, t);
    str.replace(i, 4, reinterpret_cast<char*>(t), l);

    i += l;
  }
}

void sanitize(string& str)
{
  if(str.size() & 3)
  {
    size_t l = 4 - str.size() % 4;
    for(size_t i = 0; i < l; ++i)
      str += '=';
  }
}

} // namespace kb64
