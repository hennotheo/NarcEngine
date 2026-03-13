//
// Created by theoh on 3/23/2025.
//

#include "catch2/catch_all.hpp"

#include "matrices/Matrix4.h"
#include "vectors/Vec3.h"

using namespace Catch;
using namespace narc_math;

TEST_CASE("Matrix4 Identity", "[Matrix4]")
{
    const auto m = Matrix4::Identity();

    REQUIRE(m.Data[0] == 1.0f);
    REQUIRE(m.Data[5] == 1.0f);
    REQUIRE(m.Data[10] == 1.0f);
    REQUIRE(m.Data[15] == 1.0f);

    // Other must be 0
    REQUIRE(m.Data[1] == 0.0f);
    REQUIRE(m.Data[2] == 0.0f);
    REQUIRE(m.Data[3] == 0.0f);
}

TEST_CASE("Matrix4 multiplication", "[Matrix4]") {
    const auto m1 = Matrix4::Identity();
    const auto m2 = Matrix4::Identity();

    const auto result = m1 * m2;

    REQUIRE(result.Data[0] == 1.0f);
    REQUIRE(result.Data[5] == 1.0f);
    REQUIRE(result.Data[10] == 1.0f);
    REQUIRE(result.Data[15] == 1.0f);
}

TEST_CASE("Matrix4 translation", "[Matrix4]") {
    const Vec3 t(1.0f, 2.0f, 3.0f);
    auto m = Matrix4::Identity().translation(t);

    REQUIRE(m.Data[12] == Approx(1.0f));
    REQUIRE(m.Data[13] == Approx(2.0f));
    REQUIRE(m.Data[14] == Approx(3.0f));
}

TEST_CASE("Matrix4 scale", "[Matrix4]") {
    const Vec3 s(2.0f, 3.0f, 4.0f);
    auto m = Matrix4::Identity().scale(s);

    REQUIRE(m.Data[0] == Approx(2.0f));
    REQUIRE(m.Data[5] == Approx(3.0f));
    REQUIRE(m.Data[10] == Approx(4.0f));
}

TEST_CASE("Matrix4 rotation", "[Matrix4]") {
    const Vec3 axis(0, 1, 0); // Y rot
    const float angle = 3.14159265f / 2.0f; // 90 degres

    auto m = Matrix4::Identity().rotate(axis, angle);

    REQUIRE(m.Data[0] == Approx(0.0f).margin(0.01f));
    REQUIRE(m.Data[2] == Approx(-1.0f).margin(0.01f));
    REQUIRE(m.Data[8] == Approx(1.0f).margin(0.01f));
    REQUIRE(m.Data[10] == Approx(0.0f).margin(0.01f));
}

TEST_CASE("Matrix4 perspective", "[Matrix4]") {
    float fov = 3.14159265f / 4.0f; // 45 deg
    float aspect = 16.0f/9.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    auto m = Matrix4::Perspective(fov, aspect, nearPlane, farPlane);

    // Values are ok ?
    REQUIRE(m.Data[0] > 0.0f);
    REQUIRE(m.Data[5] > 0.0f);
    REQUIRE(m.Data[10] < 0.0f); // OpenGL-style
    REQUIRE(m.Data[11] == Approx(-1.0f));
}