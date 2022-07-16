#include <benchmark_slice.h>
#include <radix.h>
#include <radix_memmanager.h>
#include <gtest/gtest.h>
#include <algorithm>

static const struct {
  int           d_lineNum;  // source line number
  u_int16_t     d_size;     // size of string in bytes
  u_int8_t      d_data[16]; // up to 16 bytes of data
} VALUES[] = {
  //line      size data
  //----      ---- ----------------------------
  { __LINE__,   1,  {0}},
  { __LINE__,   2,  {'A', 0}},
  { __LINE__,   8,  {'P', 'r', 'o', 'j', 'e', 'c', 't', 0 }},
  { __LINE__,   3,  {'P', 'r', 'o'}},
  { __LINE__,   8,  {'P', 'r', 'o', 'g', 'r', 'a', 'm', 0 }},
  { __LINE__,  10,  {'G', 'u', 't', 'e', 'n', 'b', 'e', 'r', 'g', 0}},
  { __LINE__,   6,  {'E', 'B', 'o', 'o', 'k', 0}},
  { __LINE__,   3,  {'o', 'f', 0}},
};

const std::size_t NUM_VALUES = sizeof VALUES / sizeof *VALUES;

TEST(radix, addKeySize1) {
  Radix::MemManager mem;
  Radix::Tree tree(&mem);

  Benchmark::Slice<unsigned char> key(VALUES[0].d_data, VALUES[0].d_size);
  int rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_NOT_FOUND);
  rc = tree.insert(key);
  EXPECT_TRUE(rc==Radix::e_OK);
  rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_EXISTS);
}

TEST(radix, addKeySize2) {
  Radix::MemManager mem;
  Radix::Tree tree(&mem);

  Benchmark::Slice<unsigned char> key(VALUES[1].d_data, VALUES[1].d_size);
  int rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_NOT_FOUND);
  rc = tree.insert(key);
  EXPECT_TRUE(rc==Radix::e_OK);
  rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_EXISTS);
}

TEST(radix, addOneLongerKey) {
  Radix::MemManager mem;
  Radix::Tree tree(&mem);

  Benchmark::Slice<unsigned char> key(VALUES[1].d_data, VALUES[1].d_size);
  int rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_NOT_FOUND);
  rc = tree.insert(key);
  EXPECT_TRUE(rc==Radix::e_OK);
  rc = tree.find(key);
  EXPECT_TRUE(rc==Radix::e_EXISTS);
}

TEST(radix, addMuliKey) {
  Radix::MemManager mem;
  Radix::Tree tree(&mem);

  for (unsigned i=0; i<NUM_VALUES; ++i) {
    Benchmark::Slice<unsigned char> key(VALUES[i].d_data, VALUES[i].d_size);
    int rc = tree.find(key);
    EXPECT_TRUE(rc==Radix::e_NOT_FOUND);
    rc = tree.insert(key);
    EXPECT_TRUE(rc==Radix::e_OK);
    // All previous keys should still be there
    for (unsigned j=0; j<=i; ++j) {
      Benchmark::Slice<unsigned char> skey(VALUES[j].d_data, VALUES[j].d_size);
      rc = tree.find(key);
      EXPECT_TRUE(rc==Radix::e_EXISTS);
    }
  }
}

TEST(radix, addMuliKeyAllPerms) {
  return;
}