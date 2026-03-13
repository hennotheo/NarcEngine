//
// Created by theo on 3/13/26.
//

#include "catch2/catch_all.hpp"

#include "vectors/Vec3.h"

using namespace narc_math;
using namespace Catch;

TEST_CASE("Vec3 constructors", "[Vec3]") {
    Vec3 v1; // default
    REQUIRE(v1.X() == Approx(0.0f));
    REQUIRE(v1.Y() == Approx(0.0f));
    REQUIRE(v1.Z() == Approx(0.0f));

    Vec3 v2(1.0f, 2.0f, 3.0f); // float constructor
    REQUIRE(v2.X() == Approx(1.0f));
    REQUIRE(v2.Y() == Approx(2.0f));
    REQUIRE(v2.Z() == Approx(3.0f));

    Vec3 v3(v2); // copy constructor
    REQUIRE(v3 == v2);
}

TEST_CASE("Vec3 equality operators", "[Vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(1.0f, 2.0f, 3.0f);
    Vec3 c(3.0f, 2.0f, 1.0f);

    REQUIRE(a == b);
    REQUIRE(a != c);
}

TEST_CASE("Vec3 arithmetic operators", "[Vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(3.0f, 2.0f, 1.0f);

    Vec3 sum = a + b;
    REQUIRE(sum == Vec3(4.0f, 4.0f, 4.0f));

    Vec3 diff = a - b;
    REQUIRE(diff == Vec3(-2.0f, 0.0f, 2.0f));

    Vec3 scaled = a * 2.0f;
    REQUIRE(scaled == Vec3(2.0f, 4.0f, 6.0f));

    a += b;
    REQUIRE(a == Vec3(4.0f, 4.0f, 4.0f));

    b -= Vec3(1.0f, 1.0f, 1.0f);
    REQUIRE(b == Vec3(2.0f, 1.0f, 0.0f));

    b *= 3.0f;
    REQUIRE(b == Vec3(6.0f, 3.0f, 0.0f));
}

TEST_CASE("Vec3 dot product", "[Vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, -5.0f, 6.0f);

    REQUIRE(a.dot(b) == Approx(12.0f)); // 1*4 + 2*(-5) + 3*6 = 4 -10 +18 = 12
}

TEST_CASE("Vec3 cross product", "[Vec3]") {
    Vec3 a(1.0f, 0.0f, 0.0f);
    Vec3 b(0.0f, 1.0f, 0.0f);

    Vec3 c = a.cross(b);
    REQUIRE(c == Vec3(0.0f, 0.0f, 1.0f));
}

TEST_CASE("Vec3 length and normalized", "[Vec3]") {
    Vec3 v(3.0f, 4.0f, 0.0f);
    REQUIRE(v.length() == Approx(5.0f));

    Vec3 n = v.normalized();
    REQUIRE(n.length() == Approx(1.0f));
    REQUIRE(n == Vec3(0.6f, 0.8f, 0.0f));
}

TEST_CASE("Vec3 distance", "[Vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 6.0f, 3.0f);

    REQUIRE(a.distance(b) == Approx(5.0f)); // distance sqrt((3^2 + 4^2 + 0^2)) = 5
}