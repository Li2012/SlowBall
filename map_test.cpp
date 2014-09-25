#include "map.h"

#include "gtest/gtest.h"

TEST(LocationTest, OK) {
  Location l1(1, 1);
  Location l1e(2, 1);
  Location l1w(0, 1);
  Location l1ne(1, 0);
  Location l1se(1, 2);
  Location l1nw(0, 0);
  Location l1sw(0, 2);

  EXPECT_EQ(l1.GetLocation(EAST), l1e);
  EXPECT_EQ(l1.GetLocation(WEST), l1w);
  EXPECT_EQ(l1.GetLocation(NORTH_EAST), l1ne);
  EXPECT_EQ(l1.GetLocation(SOUTH_EAST), l1se);
  EXPECT_EQ(l1.GetLocation(NORTH_WEST), l1nw);
  EXPECT_EQ(l1.GetLocation(SOUTH_WEST), l1sw);

  EXPECT_NE(l1, l1nw);
  EXPECT_NE(l1, l1sw);
  EXPECT_NE(l1, l1ne);
  EXPECT_NE(l1, l1se);
  EXPECT_NE(l1, l1e);
  EXPECT_NE(l1, l1w);

  Location l2(1, 0);
  Location l2e(2, 0);
  Location l2w(0, 0);
  Location l2ne(-1, -1);
  Location l2se(2, 1);
  Location l2nw(-1, -1);
  Location l2sw(1, 1);

  EXPECT_EQ(l2.GetLocation(EAST), l2e);
  EXPECT_EQ(l2.GetLocation(WEST), l2w);
  EXPECT_EQ(l2.GetLocation(NORTH_EAST), l2ne);
  EXPECT_EQ(l2.GetLocation(SOUTH_EAST), l2se);
  EXPECT_EQ(l2.GetLocation(NORTH_WEST), l2nw);
  EXPECT_EQ(l2.GetLocation(SOUTH_WEST), l2sw);

  EXPECT_NE(l2, l2nw);
  EXPECT_NE(l2, l2sw);
  EXPECT_NE(l2, l2ne);
  EXPECT_NE(l2, l2se);
  EXPECT_NE(l2, l2e);
  EXPECT_NE(l2, l2w);

}



TEST(LocationTest, InvalideLocation) {
  Location l1(-1, -1);
  Location l1e(-1, -1);
  Location l1w(-1, -1);
  Location l1ne(-1, -1);
  Location l1se(-1, -1);
  Location l1nw(-1, -1);
  Location l1sw(-1, -1);

  EXPECT_EQ(l1.GetLocation(EAST), l1e);
  EXPECT_EQ(l1.GetLocation(WEST), l1w);
  EXPECT_EQ(l1.GetLocation(NORTH_EAST), l1ne);
  EXPECT_EQ(l1.GetLocation(SOUTH_EAST), l1se);
  EXPECT_EQ(l1.GetLocation(NORTH_WEST), l1nw);
  EXPECT_EQ(l1.GetLocation(SOUTH_WEST), l1sw);
}
