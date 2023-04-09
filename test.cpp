#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <vec2.h>
#include <vec2.hpp>

#include <geometric.hpp>

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