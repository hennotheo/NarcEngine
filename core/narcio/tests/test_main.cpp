//
// Created by theoh on 3/23/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcIO.h"

TEST(NarcIOLoading, ReadFile)
{
    EXPECT_NO_THROW(narc_io::FileReader::readFile("shaders/shader_vert.spv"););
}

TEST(NarcIOLoading, Load3DModel)
{
    EXPECT_NO_THROW(narc_io::FileReader::load3DModel("models/test.obj"););
}

TEST(NarcIOLoading, ReadImage)
{
    EXPECT_NO_THROW(narc_io::Image image = narc_io::FileReader::readImage("textures/test.png"););
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}