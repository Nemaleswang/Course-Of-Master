#include <gtest/gtest.h>
#include "../MyProject/CAdder.h"

TEST(CAdder, AddTwo)
{
    CAdder adder;
    EXPECT_EQ(8, adder.Add(3, 4));
}

TEST(CAdder, Add)
{
    CAdder adder(3, 4);
    EXPECT_EQ(7, adder.Add());
}
