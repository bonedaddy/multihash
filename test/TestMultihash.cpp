#include "gtest/gtest.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <multihash/multihash.h>

std::string toHexString(const multihash::Hash& hash)
{
    std::ostringstream os;
    os << hash;
    return os.str();
}

TEST(Multihash, conversions)
{
    /** Looking up a hash type and querying properties */
    {
        auto expected("sha1");
        auto result(multihash::HashType("sha1").name());
        EXPECT_EQ(expected, result);
    }

    {
        auto expected = static_cast<unsigned char>(multihash::HashCode::SHA1);
        auto result =
            static_cast<unsigned char>(multihash::HashType("sha1").code());
        EXPECT_EQ(expected, result);
    }

    {
        auto expected = static_cast<unsigned char>(multihash::HashCode::SHA1);
        auto hash_type = multihash::HashType(multihash::HashCode::SHA1);
        auto result = static_cast<unsigned char>(hash_type.code());
        EXPECT_EQ(expected, result);
    }

    {
        auto expected = static_cast<unsigned char>(multihash::HashCode::SHA1);
        auto hash_type = multihash::HashType("sha1");
        auto result = static_cast<unsigned char>(hash_type.code());
        EXPECT_EQ(expected, result);
    }

    {
        auto result = multihash::HashType("sha2-256").size();
        decltype(result) expected = 32;
        EXPECT_EQ(expected, result);
    }

    /** Failing to look up a hash type */
    EXPECT_THROW(multihash::HashType("unknown_hash"), multihash::Exception);
    EXPECT_THROW(multihash::HashType(static_cast<multihash::HashCode>(0x84)),
                 multihash::Exception);
}

TEST(Multihash, hashing)
{
    /** Decoding a multihash from raw bytes */
    std::istringstream input_stream("foo");
    {
        multihash::HashFunction hash_function(multihash::HashCode::SHA1);
        auto hash = hash_function(input_stream);

        {
            auto expected =
                static_cast<unsigned char>(multihash::HashCode::SHA1);
            auto result = static_cast<unsigned char>(hash.type().code());
            EXPECT_EQ(expected, result);
        }
        {
            auto expected = "11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33";
            auto result = toHexString(hash);
            EXPECT_EQ(expected, result);
        }
    }
    {
        input_stream.clear();
        input_stream.seekg(0);

        auto hash_function =
            multihash::HashFunction(multihash::HashCode::SHA2_256);
        auto hash = hash_function(input_stream);
        {
            auto expected =
                static_cast<unsigned char>(multihash::HashCode::SHA2_256);
            auto result = static_cast<unsigned char>(hash.type().code());
            EXPECT_EQ(expected, result);
        }
        {
            auto expected = "12202c26b46b68ffc68ff99b453"
                            "c1d30413413422d706483bfa0f98a5e886266e7ae";
            auto result = toHexString(hash);
            EXPECT_EQ(expected, result);
        }
    }
    {
        input_stream.clear();
        input_stream.seekg(0);

        auto hash_function =
            multihash::HashFunction(multihash::HashCode::SHA2_512);
        auto hash = hash_function(input_stream);
        {
            auto expected =
                static_cast<unsigned char>(multihash::HashCode::SHA2_512);
            auto result = static_cast<unsigned char>(hash.type().code());
            EXPECT_EQ(expected, result);
        }
        {
            auto expected =
                "1340f7fbba6e0636f890e56fbbf"
                "3283e524c6fa3204ae298382d624741d0dc6638326e282c41be5"
                "e4254d8820772c5518a2c5a8c0c7f7eda19594a7eb539453e1ed7";
            auto result = toHexString(hash);
            EXPECT_EQ(expected, result);
        }
    }
}

TEST(Multihash, encoding)
{
    std::istringstream input_stream("foo");
    multihash::HashFunction hash_function(multihash::HashCode::SHA1);
    auto hash = hash_function(input_stream);
    multihash::BufferEncoder encode;
    multihash::BufferDecoder decode;

    auto encoded = encode(hash);
    auto decoded = decode(encoded);
}