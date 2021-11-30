#include "../src/version.h"

#include <gtest/gtest.h>
 
TEST(VersionTest, ValidVersion) { 
    ASSERT_NE(0, version());
}