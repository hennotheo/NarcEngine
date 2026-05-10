//
// Created by theo on 3/19/26.
//
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <matrices/Matrix4.h>

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

TEST_CASE("Quaternion copy constructor", "[Quaternion]")
{
    const Quaternion q1(1,2,3,4);
    const Quaternion q2(q1);

    REQUIRE(q2.X == Catch::Approx(q1.X));
    REQUIRE(q2.Y == Catch::Approx(q1.Y));
    REQUIRE(q2.Z == Catch::Approx(q1.Z));
    REQUIRE(q2.W == Catch::Approx(q1.W));
}

TEST_CASE("Quaternion equality operator", "[Quaternion]")
{
    Quaternion a(1,2,3,4);
    Quaternion b(1,2,3,4);
    Quaternion c(4,3,2,1);

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("Quaternion toEulerAngles", "[Quaternion]")
{
    const Quaternion q = Quaternion::identity();

    const Vec3 euler = q.toEulerAngles();

    REQUIRE(euler.X == Catch::Approx(0.0f).margin(0.0001f));
    REQUIRE(euler.Y == Catch::Approx(0.0f).margin(0.0001f));
    REQUIRE(euler.Z == Catch::Approx(0.0f).margin(0.0001f));
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

TEST_CASE("Quaternion rotate multiple cases", "[Quaternion]")
{
    Vec3 v(1,0,0);

    SECTION("identity rotation")
    {
        Quaternion q = Quaternion::identity();
        Vec3 r = q.rotate(v);

        REQUIRE(r.X == Catch::Approx(1));
        REQUIRE(r.Y == Catch::Approx(0));
        REQUIRE(r.Z == Catch::Approx(0));
    }

    SECTION("180 degrees Y")
    {
        Quaternion q = Quaternion::fromAxisAngle(Vec3(0,1,0), 3.14159265f);

        Vec3 r = q.rotate(v);

        REQUIRE(r.X == Catch::Approx(-1).margin(0.0001));
    }
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

TEST_CASE("Quaternion lookRotation", "[Quaternion]")
{
    const Vec3 forward(0,0,-1);
    const Vec3 up(0,1,0);

    const Quaternion q = Quaternion::lookRotation(forward, up);

    const Vec3 result = q.rotate(Vec3(0,0,-1));

    // Should look in the same direction
    REQUIRE(result.Z == Catch::Approx(-1).margin(0.0001));
}

TEST_CASE("Quaternion model matrix", "[Quaternion]")
{
    const Vec3 pos(10, 0, 0);
    const Quaternion rot = Quaternion::identity();
    const Vec3 scale(1,1,1);

    Matrix4 model = Matrix4::model(pos, rot, scale);

    REQUIRE(pos.X == Catch::Approx(10).margin(0.0001));
    REQUIRE(pos.Y == Catch::Approx(0).margin(0.0001));
    REQUIRE(pos.Z == Catch::Approx(0).margin(0.0001));
}