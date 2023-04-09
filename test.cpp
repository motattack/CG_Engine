#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <vec2.h>
#include <vec2.hpp>

#include <vec3.h>
#include <vec3.hpp>

#include <geometric.hpp>

/* \\\\\\\\\\\\\\\ TEST FOR VEC 2 ////////////// */

TEST_CASE("Constructor", "[Vec2]") {
    Vec2 res1(5, 7);
    glm::vec2 res2(5, 7);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("Length", "[Vec2]") {
    Vec2 res1(5, 7);
    glm::vec2 res2(5, 7);

    REQUIRE(res1.len() == glm::length(res2));
}

TEST_CASE("+", "[Vec2]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    Vec2 res1 = resA1 + resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    glm::vec2 res2 = resA2 + resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("+=", "[Vec2]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    resA1 += resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    resA2 += resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
}

TEST_CASE("-", "[Vec2]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    Vec2 res1 = resA1 - resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    glm::vec2 res2 = resA2 - resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("-=", "[Vec2]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    resA1 -= resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    resA2 -= resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
}


TEST_CASE("dot product", "[Vec2]") {
    Vec2 vecA1(12, 1);
    Vec2 vecB1(8, 5);
    float res1 = vecA1.dotProduct(vecB1);

    glm::vec2 vecA2 = glm::vec2(12, 1);
    glm::vec2 vecB2 = glm::vec2(8, 5);
    float res2 = glm::dot(vecA2, vecB2);

    REQUIRE(res1 == res2);
}

TEST_CASE("*", "[Vec2]") {
    Vec2 vec1(12, 7);
    Vec2 res1(vec1 * 2.0f);

    glm::vec2 vec2 = glm::vec2(12, 7);
    glm::vec2 res2 = vec2 * 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("*=", "[Vec2]") {
    Vec2 vec1(12, 7);
    vec1 *= 6.0f;

    glm::vec2 vec2 = glm::vec2(12, 7);
    vec2 *= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
}

TEST_CASE("/", "[Vec2]") {
    Vec2 vec1(12, 7);
    Vec2 res1(vec1 / 2.0f);

    glm::vec2 vec2 = glm::vec2(12, 7);
    glm::vec2 res2 = vec2 / 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("/=", "[Vec2]") {
    Vec2 vec1(12, 7);
    vec1 /= 6.0f;

    glm::vec2 vec2 = glm::vec2(12, 7);
    vec2 /= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
}

//// WAIT WHAT?
//TEST_CASE("Normalize", "[Vec2]") {
//    Vec2 vec1(12, 7);
//    Vec2 res1 = vec1.normalize();
//
//    glm::vec2 vec2 = glm::vec2(12, 7);
//    glm::vec2 res2 = glm::normalize(vec2);
//
//    REQUIRE(res1.x == res2.x);
//    REQUIRE(res1.y == res2.y);
//}

TEST_CASE("==", "[Vec2]") {
    Vec2 resA1(7, 7);
    Vec2 resB1(7, 7);
    bool res1 = resA1 == resB1;

    glm::vec2 resA2(7, 7);
    glm::vec2 resB2(7, 7);
    bool res2 = resA2 == resB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!=", "[Vec2]") {
    Vec2 resA1(7, 7);
    Vec2 resB1(7, 2);
    bool res1 = resA1 != resB1;

    glm::vec2 resA2(7, 7);
    glm::vec2 resB2(7, 2);
    bool res2 = resA2 != resB2;

    REQUIRE(res1 == res2);
}

/* \\\\\\\\\\\\\\\ TEST FOR VEC 3 ////////////// */

TEST_CASE("Constructor Vec3", "[Vec3]") {
    Vec3 res1(5, 7, 2);
    glm::vec3 res2(5, 7, 2);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);

}

TEST_CASE("Length Vec3", "[Vec3]") {
    Vec3 res1(5, 7, 2);
    glm::vec3 res2(5, 7, 2);

    REQUIRE(res1.len() == glm::length(res2));
}

TEST_CASE("+ Vec3", "[Vec3]") {
    Vec3 resA1(5, 7, 2);
    Vec3 resB1(7, 5, 1);
    Vec3 res1 = resA1 + resB1;

    glm::vec3 resA2(5, 7, 2);
    glm::vec3 resB2(7, 5, 1);
    glm::vec3 res2 = resA2 + resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("+= Vec3", "[Vec3]") {
    Vec3 resA1(5, 7, 2);
    Vec3 resB1(7, 5, 2);
    resA1 += resB1;

    glm::vec3 resA2(5, 7, 2);
    glm::vec3 resB2(7, 5, 2);
    resA2 += resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
    REQUIRE(resA1.z == resA2.z);
}

TEST_CASE("- Vec3", "[Vec3]") {
    Vec3 resA1(5, 7, 2);
    Vec3 resB1(7, 5, 2);
    Vec3 res1 = resA1 - resB1;

    glm::vec3 resA2(5, 7, 2);
    glm::vec3 resB2(7, 5, 2);
    glm::vec3 res2 = resA2 - resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("-= Vec3", "[Vec3]") {
    Vec3 resA1(5, 7, 2);
    Vec3 resB1(7, 5, 2);
    resA1 -= resB1;

    glm::vec3 resA2(5, 7, 2);
    glm::vec3 resB2(7, 5, 2);
    resA2 -= resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
    REQUIRE(resA1.z == resA2.z);
}


TEST_CASE("dot product Vec3", "[Vec3]") {
    Vec3 vecA1(12, 1, 2);
    Vec3 vecB1(8, 5, 2);
    float res1 = vecA1.dotProduct(vecB1);

    glm::vec3 vecA2 = glm::vec3(12, 1, 2);
    glm::vec3 vecB2 = glm::vec3(8, 5, 2);
    float res2 = glm::dot(vecA2, vecB2);

    REQUIRE(res1 == res2);
}

TEST_CASE("cross product Vec3", "[Vec3]") {
    Vec3 vecA1(15, 12, 1337);
    Vec3 vecB1(134, 0, 777);
    Vec3 res1(vecA1.crossProduct(vecB1));

    glm::vec3 vecA2 = glm::vec3(15, 12, 1337);
    glm::vec3 vecB2 = glm::vec3(134, 0, 777);
    glm::vec3 res2 = glm::cross(vecA2, vecB2);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("* Vec3", "[Vec3]") {
    Vec3 vec1(12, 7, 2);
    Vec3 res1(vec1 * 2.0f);

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    glm::vec3 res2 = vec2 * 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("*= Vec3", "[Vec3]") {
    Vec3 vec1(12, 7, 2);
    vec1 *= 6.0f;

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    vec2 *= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
    REQUIRE(vec1.z == vec2.z);
}

TEST_CASE("/ Vec3", "[Vec3]") {
    Vec3 vec1(12, 7, 2);
    Vec3 res1(vec1 / 2.0f);

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    glm::vec3 res2 = vec2 / 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("/= Vec3", "[Vec3]") {
    Vec3 vec1(12, 7, 2);
    vec1 /= 6.0f;

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    vec2 /= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
    REQUIRE(vec1.z == vec2.z);
}

//// WAIT WHAT?
//TEST_CASE("Normalize Vec3", "[Vec3]") {
//    Vec3 vec1(12, 7, 2);
//    Vec3 res1 = vec1.normalize();
//
//    glm::vec3 vec2 = glm::vec3(12, 7, 2);
//    glm::vec3 res2 = glm::normalize(vec2);
//
//    REQUIRE(res1.x == res2.x);
//    REQUIRE(res1.y == res2.y);
//    REQUIRE(res1.z == res2.z);
//}

TEST_CASE("== Vec3", "[Vec3]") {
    Vec3 resA1(7, 7, 2);
    Vec3 resB1(7, 7, 2);
    bool res1 = resA1 == resB1;

    glm::vec3 resA2(7, 7, 2);
    glm::vec3 resB2(7, 7, 2);
    bool res2 = resA2 == resB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!= Vec3", "[Vec3]") {
    Vec3 resA1(7, 7, 2);
    Vec3 resB1(7, 2, 2);
    bool res1 = resA1 != resB1;

    glm::vec3 resA2(7, 7, 2);
    glm::vec3 resB2(7, 2, 2);
    bool res2 = resA2 != resB2;

    REQUIRE(res1 == res2);
}