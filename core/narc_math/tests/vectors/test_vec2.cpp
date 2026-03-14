//
// Created by theo on 3/14/26.
//

#include <random>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "vectors/Vec2.h"

using namespace narc_math;
using Catch::Approx;

static constexpr float EPSILON = 0.0001f;

TEST_CASE("Vec2 - constructors", "[Vec2]")
{
    SECTION("default constructor initializes to zero")
    {
        const Vec2 v;

        REQUIRE(v.X() == Approx(0.0f));
        REQUIRE(v.Y() == Approx(0.0f));
    }

    SECTION("value constructor")
    {
        const Vec2 v(2.0f, 3.0f);

        REQUIRE(v.X() == Approx(2.0f));
        REQUIRE(v.Y() == Approx(3.0f));
    }

    SECTION("copy constructor")
    {
        const Vec2 a(5.0f, 7.0f);
        const Vec2 b(a);

        REQUIRE(b == a);
    }
}

TEST_CASE("Vec2 - getters and setters", "[Vec2]")
{
    Vec2 v;

    v.setX(5.0f);
    v.setY(7.0f);

    REQUIRE(v.X() == Approx(5.0f));
    REQUIRE(v.Y() == Approx(7.0f));
}

TEST_CASE("Vec2 - equality operators", "[Vec2]")
{
    const Vec2 a(1,2);
    const Vec2 b(1,2);
    const Vec2 c(2,1);

    REQUIRE(a == b);
    REQUIRE(a != c);
}

TEST_CASE("Vec2 - arithmetic operators", "[Vec2]")
{
    Vec2 a(1,2);
    Vec2 b(3,4);

    SECTION("addition")
    {
        Vec2 r = a + b;

        REQUIRE(r.X() == Approx(4));
        REQUIRE(r.Y() == Approx(6));
    }

    SECTION("subtraction")
    {
        Vec2 r = a - b;

        REQUIRE(r.X() == Approx(-2));
        REQUIRE(r.Y() == Approx(-2));
    }

    SECTION("scalar multiplication")
    {
        Vec2 r = a * 2.0f;

        REQUIRE(r.X() == Approx(2));
        REQUIRE(r.Y() == Approx(4));
    }

    SECTION("compound operators")
    {
        a += b;
        REQUIRE(a == Vec2(4,6));

        b -= Vec2(1,1);
        REQUIRE(b == Vec2(2,3));

        b *= 2.0f;
        REQUIRE(b == Vec2(4,6));
    }
}

TEST_CASE("Vec2 - dot and cross product", "[Vec2]")
{
    const Vec2 a(1,2);
    const Vec2 b(3,4);

    SECTION("dot product")
    {
        REQUIRE(a.dot(b) == Approx(11.0f));
    }

    SECTION("2D cross product")
    {
        // In 2D the cross product returns a scalar representing
        // the magnitude of the 3D cross product's Z component
        const Vec2 x(1,0);
        const Vec2 y(0,1);

        REQUIRE(x.cross(y) == Approx(1.0f));
    }
}

TEST_CASE("Vec2 - length operations", "[Vec2]")
{
    const Vec2 v(3,4);

    REQUIRE(v.length() == Approx(5.0f));
    REQUIRE(v.lengthSquared() == Approx(25.0f));
}

TEST_CASE("Vec2 - normalization", "[Vec2]")
{
    const Vec2 v(3,4);
    const Vec2 n = v.normalized();

    REQUIRE(n.length() == Approx(1.0f).margin(EPSILON));
    REQUIRE(n.X() == Approx(0.6f).margin(EPSILON));
    REQUIRE(n.Y() == Approx(0.8f).margin(EPSILON));
}

TEST_CASE("Vec2 - distance", "[Vec2]")
{
    const Vec2 a(0,0);
    const Vec2 b(3,4);

    REQUIRE(a.distance(b) == Approx(5.0f));
}

TEST_CASE("Vec2 - perpendicular vector", "[Vec2]")
{
    const Vec2 v(2,3);
    const Vec2 p = v.perpendicular();

    // Perpendicular of (x,y) is (-y,x)
    REQUIRE(p.X() == Approx(-3));
    REQUIRE(p.Y() == Approx(2));

    // Must be orthogonal
    REQUIRE(v.dot(p) == Approx(0.0f));
}

TEST_CASE("Vec2 - reflection", "[Vec2]")
{
    const Vec2 v(1,-1);
    const Vec2 normal(0,1);

    // Reflecting across a vertical surface normal
    const Vec2 r = v.reflect(normal);

    REQUIRE(r.X() == Approx(1));
    REQUIRE(r.Y() == Approx(1));
}

TEST_CASE("Vec2 - rotation", "[Vec2]")
{
    const Vec2 v(1,0);

    const Vec2 r = v.rotate(3.14159265f / 2.0f);

    REQUIRE(r.X() == Approx(0).margin(EPSILON));
    REQUIRE(r.Y() == Approx(1).margin(EPSILON));
}

TEST_CASE("Vec2 - rotation preserves length", "[Vec2]")
{
    const Vec2 v(3,4);

    const Vec2 r = v.rotate(1.2345f);

    // Rotations must not change vector magnitude
    REQUIRE(r.length() == Approx(v.length()).margin(EPSILON));
}

TEST_CASE("Vec2 - lerp", "[Vec2]")
{
    const Vec2 a(0,0);
    const Vec2 b(10,10);

    const Vec2 mid = Vec2::lerp(a,b,0.5f);

    REQUIRE(mid.X() == Approx(5));
    REQUIRE(mid.Y() == Approx(5));
}

TEST_CASE("Vec2 - mathematical invariants", "[Vec2]")
{
    const Vec2 a(3,5);
    const Vec2 b(7,2);

    SECTION("dot product symmetry")
    {
        REQUIRE(a.dot(b) == Approx(b.dot(a)));
    }

    SECTION("distance symmetry")
    {
        REQUIRE(a.distance(b) == Approx(b.distance(a)));
    }

    SECTION("cross self is zero")
    {
        REQUIRE(a.cross(a) == Approx(0));
    }

    SECTION("normalized vector has unit length")
    {
        const Vec2 n = a.normalized();

        REQUIRE(n.length() == Approx(1.0f).margin(EPSILON));
    }
}

TEST_CASE("Vec2 - randomized invariants", "[Vec2]")
{
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

    for(int i = 0; i < 1000; ++i)
    {
        Vec2 v(dist(rng), dist(rng));

        if(v.length() > EPSILON)
        {
            Vec2 n = v.normalized();

            // Normalized vector should have unit length
            REQUIRE(n.length() == Approx(1.0f).margin(0.001f));
        }

        // Fundamental vector identity
        REQUIRE(v.lengthSquared() == Approx(v.dot(v)).margin(0.001f));
    }
}

TEST_CASE("Vec2 - numerical stability", "[Vec2]")
{
    SECTION("small vectors")
    {
        Vec2 v(1e-8f, 1e-8f);
        Vec2 n = v.normalized();

        // Ensure we do not generate NaNs or infinities
        REQUIRE(std::isfinite(n.X()));
        REQUIRE(std::isfinite(n.Y()));
    }

    SECTION("large vectors")
    {
        Vec2 v(1e6f, 1e6f);

        REQUIRE(std::isfinite(v.length()));
    }

    SECTION("zero vector normalization")
    {
        Vec2 v(0,0);
        Vec2 n = v.normalized();

        REQUIRE(n.X() == Approx(0));
        REQUIRE(n.Y() == Approx(0));
    }
}