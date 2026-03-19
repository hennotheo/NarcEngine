//
// Created by theo on 3/19/26.
//
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "Quaternion.h"
#include "vectors/Vec3.h"

using namespace Catch;
using namespace narc_math;

TEST_CASE("Quaternion identity", "[Quaternion]")
{
    const auto q = Quaternion::identity();

    REQUIRE(q.X == Approx(0.0f));
    REQUIRE(q.Y == Approx(0.0f));
    REQUIRE(q.Z == Approx(0.0f));
    REQUIRE(q.W == Approx(1.0f));
}

TEST_CASE("Quaternion length and normalization", "[Quaternion]")
{
    Quaternion q(1, 2, 3, 4);

    const float len = q.length();
    REQUIRE(len > 0.0f);

    const Quaternion n = q.normalized();

    REQUIRE(n.length() == Approx(1.0f).margin(0.0001f));
}

TEST_CASE("Quaternion multiplication identity", "[Quaternion]")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 1.0f);
    Quaternion identity = Quaternion::identity();

    REQUIRE((q * identity).X == Approx(q.X));
    REQUIRE((q * identity).Y == Approx(q.Y));
    REQUIRE((q * identity).Z == Approx(q.Z));
    REQUIRE((q * identity).W == Approx(q.W));
}

TEST_CASE("Quaternion multiplication associativity (approx)", "[Quaternion]")
{
    Quaternion a = Quaternion::fromAxisAngle(Vec3(1, 0, 0), 0.5f);
    Quaternion b = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 1.0f);
    Quaternion c = Quaternion::fromAxisAngle(Vec3(0, 0, 1), 0.3f);

    Quaternion r1 = (a * b) * c;
    Quaternion r2 = a * (b * c);

    REQUIRE(r1.X == Approx(r2.X).margin(0.0001f));
    REQUIRE(r1.Y == Approx(r2.Y).margin(0.0001f));
    REQUIRE(r1.Z == Approx(r2.Z).margin(0.0001f));
    REQUIRE(r1.W == Approx(r2.W).margin(0.0001f));
}

TEST_CASE("Quaternion fromAxisAngle normalization", "[Quaternion]")
{
    const Quaternion q = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 1.0f);

    REQUIRE(q.length() == Approx(1.0f).margin(0.0001f));
}

TEST_CASE("Quaternion rotates vector correctly (90 deg Y)", "[Quaternion]")
{
    Vec3 v(1, 0, 0);

    Quaternion q = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 3.14159265f / 2.0f);

    const Vec3 r = q.rotate(v);

    REQUIRE(r.X == Approx(0.0f).margin(0.0001f));
    REQUIRE(r.Z == Approx(-1.0f).margin(0.0001f));
}

TEST_CASE("Quaternion dot product symmetry", "[Quaternion]")
{
    const Quaternion a(1, 2, 3, 4);
    const Quaternion b(5, 6, 7, 8);

    REQUIRE(a.dot(b) == Approx(b.dot(a)));
}

TEST_CASE("Quaternion lerp endpoints", "[Quaternion]")
{
    const auto a = Quaternion::identity();
    const auto b = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 1.0f);

    REQUIRE(Quaternion::lerp(a,b,0.0f).W == Approx(a.W));
    REQUIRE(Quaternion::lerp(a,b,1.0f).W == Approx(b.W));
}

TEST_CASE("Quaternion slerp endpoints", "[Quaternion]")
{
    const auto a = Quaternion::identity();
    const auto b = Quaternion::fromAxisAngle(Vec3(0, 1, 0), 1.0f);

    const auto r0 = Quaternion::slerp(a, b, 0.0f);
    const auto r1 = Quaternion::slerp(a, b, 1.0f);

    REQUIRE(r0.W == Approx(a.W).margin(0.0001f));
    REQUIRE(r1.W == Approx(b.W).margin(0.0001f));
}

TEST_CASE("Quaternion slerp keeps unit length", "[Quaternion]")
{
    const auto a = Quaternion::identity();
    const auto b = Quaternion::fromAxisAngle(Vec3(1, 0, 0), 2.0f);

    for (float t = 0.0f; t <= 1.0f; t += 0.1f)
    {
        Quaternion r = Quaternion::slerp(a, b, t);
        REQUIRE(r.length() == Approx(1.0f).margin(0.0001f));
    }
}
