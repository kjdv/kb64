#include <algorithm>
#include <base64.hh>
#include <gtest/gtest.h>

namespace kb64 {
namespace {

using namespace std;

TEST(Base64, BasicDecode)
{
  char    b[4] = {'Y', 'W', '5', '5'};
  uint8_t t[3];

  EXPECT_EQ(3u, decode(b, t));
  EXPECT_EQ('a', t[0]);
  EXPECT_EQ('n', t[1]);
  EXPECT_EQ('y', t[2]);

  // scramble
  std::fill_n(t, 3, 0);

  b[3] = '=';
  EXPECT_EQ(2u, decode(b, t));
  EXPECT_EQ('a', t[0]);
  EXPECT_EQ('n', t[1]);

  // scramble
  std::fill_n(t, 3, 0);

  b[2] = '=';
  EXPECT_EQ(1u, decode(b, t));
  EXPECT_EQ('a', t[0]);
}

TEST(Base64, BasicEncode)
{
  uint8_t s[3] = {'a', 'n', 'y'};
  char    t[5];
  t[4] = '\0';

  encode(s, 3, t);
  EXPECT_STREQ("YW55", t);

  // scramble
  std::fill_n(t, 5, '\0');

  encode(s, 2, t);
  EXPECT_STREQ("YW4=", t);

  // scramble
  std::fill_n(t, 5, '\0');

  encode(s, 1, t);
  EXPECT_STREQ("YQ==", t);
}

TEST(Base64, Decode)
{
  string decoded("This is a decoded message.");
  string encoded("VGhpcyBpcyBhIGRlY29kZWQgbWVzc2FnZS4=");

  vector<uint8_t> decoded_v(decoded.begin(), decoded.end());
  EXPECT_EQ(decoded_v, decode(encoded));

  inplace_decode(encoded);
  EXPECT_EQ(decoded, encoded);
}

TEST(Base64, Encode)
{
  string decoded("This is a decoded message.");
  string encoded("VGhpcyBpcyBhIGRlY29kZWQgbWVzc2FnZS4=");

  vector<uint8_t> decoded_v(decoded.begin(), decoded.end());

  EXPECT_EQ(encoded, encode(decoded_v));

  inplace_encode(decoded);

  EXPECT_EQ(encoded, decoded);
}

TEST(Base64, AllClasses)
{
  string decoded("\x01\x96\xb3\xd3\xdf\xbf");
  string encoded("AZaz09+/");

  auto   actual_v = decode(encoded);
  string actual(actual_v.begin(), actual_v.end());

  EXPECT_EQ(decoded, actual);
}

TEST(Base64, Sanitize)
{
  string orig("a2xhYXNk");
  string sanitized("a2xhYXNk");

  sanitize(orig);
  EXPECT_EQ(sanitized, orig);

  orig      = "a2xhYXN";
  sanitized = "a2xhYXN=";

  sanitize(orig);
  EXPECT_EQ(sanitized, orig);

  orig      = "a2xhYX";
  sanitized = "a2xhYX==";

  sanitize(orig);
  EXPECT_EQ(sanitized, orig);

  // inorrect padding, but the santize still works
  orig      = "a2xhY";
  sanitized = "a2xhY===";

  sanitize(orig);
  EXPECT_EQ(sanitized, orig);
}
} // namespace
} // namespace kb64
