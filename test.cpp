#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <vec2.h>
#include <vec2.hpp>

#include <vec3.h>
#include <vec3.hpp>

#include <vec4.h>
#include <vec4.hpp>

#include <mat2x2.h>
#include <mat2x2.hpp>

#include <mat3x3.h>
#include <mat3x3.hpp>

#include <geometric.hpp>
#include <gtc/matrix_access.hpp>

/* \\\\\\\\\\\\\\\ TEST FOR VEC 2 ////////////// */

TEST_CASE("Constructor", "[Vector]") {
    Vec2 res1(5, 7);
    glm::vec2 res2(5, 7);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("Length", "[Vector]") {
    Vec2 res1(5, 7);
    glm::vec2 res2(5, 7);

    REQUIRE(res1.len() == glm::length(res2));
}

TEST_CASE("+", "[Vector]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    Vec2 res1 = resA1 + resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    glm::vec2 res2 = resA2 + resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("+=", "[Vector]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    resA1 += resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    resA2 += resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
}

TEST_CASE("-", "[Vector]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    Vec2 res1 = resA1 - resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    glm::vec2 res2 = resA2 - resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("-=", "[Vector]") {
    Vec2 resA1(5, 7);
    Vec2 resB1(7, 5);
    resA1 -= resB1;

    glm::vec2 resA2(5, 7);
    glm::vec2 resB2(7, 5);
    resA2 -= resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
}


TEST_CASE("dot product", "[Vector]") {
    Vec2 vecA1(12, 1);
    Vec2 vecB1(8, 5);
    float res1 = vecA1.dotProduct(vecB1);

    glm::vec2 vecA2 = glm::vec2(12, 1);
    glm::vec2 vecB2 = glm::vec2(8, 5);
    float res2 = glm::dot(vecA2, vecB2);

    REQUIRE(res1 == res2);
}

TEST_CASE("*", "[Vector]") {
    Vec2 vec1(12, 7);
    Vec2 res1(vec1 * 2.0f);

    glm::vec2 vec2 = glm::vec2(12, 7);
    glm::vec2 res2 = vec2 * 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("*=", "[Vector]") {
    Vec2 vec1(12, 7);
    vec1 *= 6.0f;

    glm::vec2 vec2 = glm::vec2(12, 7);
    vec2 *= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
}

TEST_CASE("/", "[Vector]") {
    Vec2 vec1(12, 7);
    Vec2 res1(vec1 / 2.0f);

    glm::vec2 vec2 = glm::vec2(12, 7);
    glm::vec2 res2 = vec2 / 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
}

TEST_CASE("/=", "[Vector]") {
    Vec2 vec1(12, 7);
    vec1 /= 6.0f;

    glm::vec2 vec2 = glm::vec2(12, 7);
    vec2 /= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
}

//// WAIT WHAT?
//TEST_CASE("Normalize", "[Vector]") {
//    Vec2 vec1(12, 7);
//    Vec2 res1 = vec1.normalize();
//
//    glm::vec2 vec2 = glm::vec2(12, 7);
//    glm::vec2 res2 = glm::normalize(vec2);
//
//    REQUIRE(res1.x == res2.x);
//    REQUIRE(res1.y == res2.y);
//}

TEST_CASE("==", "[Vector]") {
    Vec2 resA1(7, 7);
    Vec2 resB1(7, 7);
    bool res1 = resA1 == resB1;

    glm::vec2 resA2(7, 7);
    glm::vec2 resB2(7, 7);
    bool res2 = resA2 == resB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!=", "[Vector]") {
    Vec2 resA1(7, 7);
    Vec2 resB1(7, 2);
    bool res1 = resA1 != resB1;

    glm::vec2 resA2(7, 7);
    glm::vec2 resB2(7, 2);
    bool res2 = resA2 != resB2;

    REQUIRE(res1 == res2);
}

/* \\\\\\\\\\\\\\\ TEST FOR VEC 3 ////////////// */

TEST_CASE("Constructor Vec3", "[Vector]") {
    Vec3 res1(5, 7, 2);
    glm::vec3 res2(5, 7, 2);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);

}

TEST_CASE("Length Vec3", "[Vector]") {
    Vec3 res1(5, 7, 2);
    glm::vec3 res2(5, 7, 2);

    REQUIRE(res1.len() == glm::length(res2));
}

TEST_CASE("+ Vec3", "[Vector]") {
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

TEST_CASE("+= Vec3", "[Vector]") {
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

TEST_CASE("- Vec3", "[Vector]") {
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

TEST_CASE("-= Vec3", "[Vector]") {
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


TEST_CASE("dot product Vec3", "[Vector]") {
    Vec3 vecA1(12, 1, 2);
    Vec3 vecB1(8, 5, 2);
    float res1 = vecA1.dotProduct(vecB1);

    glm::vec3 vecA2 = glm::vec3(12, 1, 2);
    glm::vec3 vecB2 = glm::vec3(8, 5, 2);
    float res2 = glm::dot(vecA2, vecB2);

    REQUIRE(res1 == res2);
}

TEST_CASE("cross product Vec3", "[Vector]") {
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

TEST_CASE("* Vec3", "[Vector]") {
    Vec3 vec1(12, 7, 2);
    Vec3 res1(vec1 * 2.0f);

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    glm::vec3 res2 = vec2 * 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("*= Vec3", "[Vector]") {
    Vec3 vec1(12, 7, 2);
    vec1 *= 6.0f;

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    vec2 *= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
    REQUIRE(vec1.z == vec2.z);
}

TEST_CASE("/ Vec3", "[Vector]") {
    Vec3 vec1(12, 7, 2);
    Vec3 res1(vec1 / 2.0f);

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    glm::vec3 res2 = vec2 / 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("/= Vec3", "[Vector]") {
    Vec3 vec1(12, 7, 2);
    vec1 /= 6.0f;

    glm::vec3 vec2 = glm::vec3(12, 7, 2);
    vec2 /= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
    REQUIRE(vec1.z == vec2.z);
}

//// WAIT WHAT?
//TEST_CASE("Normalize Vec3", "[Vector]") {
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

TEST_CASE("== Vec3", "[Vector]") {
    Vec3 resA1(7, 7, 2);
    Vec3 resB1(7, 7, 2);
    bool res1 = resA1 == resB1;

    glm::vec3 resA2(7, 7, 2);
    glm::vec3 resB2(7, 7, 2);
    bool res2 = resA2 == resB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!= Vec3", "[Vector]") {
    Vec3 resA1(7, 7, 2);
    Vec3 resB1(7, 2, 2);
    bool res1 = resA1 != resB1;

    glm::vec3 resA2(7, 7, 2);
    glm::vec3 resB2(7, 2, 2);
    bool res2 = resA2 != resB2;

    REQUIRE(res1 == res2);
}

/* \\\\\\\\\\\\\\\ TEST FOR VEC 4 ////////////// */

TEST_CASE("Constructor Vec4", "[Vector]") {
    Vec4 res1(5, 7, 2, 3);
    glm::vec4 res2(5, 7, 2, 3);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
    REQUIRE(res1.w == res2.w);
}

TEST_CASE("Length Vec4", "[Vector]") {
    Vec4 res1(5, 7, 2, 3);
    glm::vec4 res2(5, 7, 2, 3);

    REQUIRE(res1.len() == glm::length(res2));
}

TEST_CASE("+ Vec4", "[Vector]") {
    Vec4 resA1(5, 7, 2, 3);
    Vec4 resB1(7, 5, 2, 3);
    Vec4 res1 = resA1 + resB1;

    glm::vec4 resA2(5, 7, 2, 3);
    glm::vec4 resB2(7, 5, 2, 3);
    glm::vec4 res2 = resA2 + resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
    REQUIRE(res1.w == res2.w);
}

TEST_CASE("+= Vec4", "[Vector]") {
    Vec4 resA1(5, 7, 2, 3);
    Vec4 resB1(7, 5, 2, 3);
    resA1 += resB1;

    glm::vec4 resA2(5, 7, 2, 3);
    glm::vec4 resB2(7, 5, 2, 3);
    resA2 += resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
    REQUIRE(resA1.z == resA2.z);
    REQUIRE(resA1.w == resA2.w);
}

TEST_CASE("- Vec4", "[Vector]") {
    Vec4 resA1(5, 7, 2, 3);
    Vec4 resB1(7, 5, 2, 3);
    Vec4 res1 = resA1 - resB1;

    glm::vec4 resA2(5, 7, 2, 3);
    glm::vec4 resB2(7, 5, 2, 3);
    glm::vec4 res2 = resA2 - resB2;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
    REQUIRE(res1.w == res2.w);
}

TEST_CASE("-= Vec4", "[Vector]") {
    Vec4 resA1(5, 7, 2, 3);
    Vec4 resB1(7, 5, 2, 3);
    resA1 -= resB1;

    glm::vec4 resA2(5, 7, 2, 3);
    glm::vec4 resB2(7, 5, 2, 3);
    resA2 -= resB2;

    REQUIRE(resA1.x == resA2.x);
    REQUIRE(resA1.y == resA2.y);
    REQUIRE(resA1.z == resA2.z);
    REQUIRE(resA1.w == resA2.w);
}


TEST_CASE("dot product Vec4", "[Vector]") {
    Vec4 vecA1(12, 1, 2, 3);
    Vec4 vecB1(8, 5, 2, 3);
    float res1 = vecA1.dotProduct(vecB1);

    glm::vec4 vecA2 = glm::vec4(12, 1, 2, 3);
    glm::vec4 vecB2 = glm::vec4(8, 5, 2, 3);
    float res2 = glm::dot(vecA2, vecB2);

    REQUIRE(res1 == res2);
}

TEST_CASE("* Vec4", "[Vector]") {
    Vec4 vec1(12, 7, 2, 3);
    Vec4 res1(vec1 * 2.0f);

    glm::vec4 vec2 = glm::vec4(12, 7, 2, 3);
    glm::vec4 res2 = vec2 * 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
    REQUIRE(res1.w == res2.w);
}

TEST_CASE("*= Vec4", "[Vector]") {
    Vec4 vec1(12, 7, 2, 3);
    vec1 *= 6.0f;

    glm::vec4 vec2 = glm::vec4(12, 7, 2, 3);
    vec2 *= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
}

TEST_CASE("/ Vec4", "[Vector]") {
    Vec4 vec1(12, 7, 2, 3);
    Vec4 res1(vec1 / 2.0f);

    glm::vec4 vec2 = glm::vec4(12, 7, 2, 3);
    glm::vec4 res2 = vec2 / 2.0f;

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
    REQUIRE(res1.w == res2.w);
}

TEST_CASE("/= Vec4", "[Vector]") {
    Vec4 vec1(12, 7, 2, 3);
    vec1 /= 6.0f;

    glm::vec4 vec2 = glm::vec4(12, 7, 2, 3);
    vec2 /= 6.0f;

    REQUIRE(vec1.x == vec2.x);
    REQUIRE(vec1.y == vec2.y);
    REQUIRE(vec1.z == vec2.z);
    REQUIRE(vec1.w == vec2.w);
}

//// WAIT WHAT?
//TEST_CASE("Normalize Vec4", "[Vector]") {
//    Vec4 vec1(12, 7, 2, 3);
//    Vec4 res1 = vec1.normalize();
//
//    glm::vec4 vec2 = glm::vec4(12, 7, 2, 3);
//    glm::vec4 res2 = glm::normalize(vec2);
//
//    REQUIRE(res1.x == res2.x);
//    REQUIRE(res1.y == res2.y);
//    REQUIRE(res1.z == res2.z);
//    REQUIRE(res1.w == res2.w);
//}

TEST_CASE("== Vec4", "[Vector]") {
    Vec4 resA1(7, 7, 2, 3);
    Vec4 resB1(7, 7, 2, 3);
    bool res1 = resA1 == resB1;

    glm::vec4 resA2(7, 7, 2, 3);
    glm::vec4 resB2(7, 7, 2, 3);
    bool res2 = resA2 == resB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!= Vec4", "[Vector]") {
    Vec4 resA1(7, 7, 2, 3);
    Vec4 resB1(7, 2, 2, 3);
    bool res1 = resA1 != resB1;

    glm::vec4 resA2(7, 7, 2, 3);
    glm::vec4 resB2(7, 2, 2, 3);
    bool res2 = resA2 != resB2;

    REQUIRE(res1 == res2);
}

/* \\\\\\\\\\\\\\\ TEST FOR MATRIX 2 ////////////// */

TEST_CASE("Constructor Mat2x2", "[Mat]") {
    Mat2x2 res1(2);
    glm::mat2 res2(2);

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("+ Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    Mat2x2 res1;
    res1 = matOne + matTwo;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    glm::mat2 res2 = mat1 + mat2;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("+= Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    matOne += matTwo;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    mat1 += mat2;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("- Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    Mat2x2 fuc = matOne - matTwo;
    Mat2x2 res1 = fuc;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    glm::mat2 res2 = mat1 - mat2;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("-= Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    matOne -= matTwo;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    mat1 -= mat2;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("* Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    Mat2x2 res1(matOne * matTwo);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    glm::mat2 res2 = mat1 * mat2;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("*= Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 matTwo(5, 6, 7, 8);
    matOne *= matTwo;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 mat2 = glm::mat2(5, 6, 7, 8);
    mat1 *= mat2;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("*x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 res1(matOne * 15.0f);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = mat1 * 15.0f;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("*=x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    matOne *= 15.0f;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    mat1 *= 15.0f;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("|x|=1 Mat2x2", "[Mat]") {
    Mat2x2 res1 = Mat2x2::identity();

    auto res2 = glm::mat2(1);

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("/ Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 res1(matOne / 15.0f);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = mat1 / 15.0f;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("/= Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    matOne /= 15.0f;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    mat1 /= 15.0f;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("+x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 res1(matOne + 15.0f);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = mat1 + 15.0f;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("+=x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    matOne += 15.0f;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    mat1 += 15.0f;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("-x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Mat2x2 res1(matOne - 15.0f);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = mat1 - 15.0f;

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("-=x Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    matOne -= 15.0f;

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    mat1 -= 15.0f;

    REQUIRE(mat1[0][0] == matOne[0][0]);
    REQUIRE(mat1[0][1] == matOne[0][1]);
    REQUIRE(mat1[1][0] == matOne[1][0]);
    REQUIRE(mat1[1][1] == matOne[1][1]);
}

TEST_CASE("M*M Mat2x2", "[Mat]") {
    Mat2x2 matOne(1, 2, 3, 4);
    Vec2 vecOne(5, 8);
    Vec2 res1(matOne * vecOne);

    glm::mat2 mat1 = glm::mat2(1, 2, 3, 4);
    glm::vec2 mat2 = glm::vec2(5, 8);
    glm::vec2 res2 = mat1 * mat2;

    REQUIRE(res2.x == res1.x);
    REQUIRE(res2.y == res1.y);
}

TEST_CASE("Trans Mat2x2", "[Mat]") {
    Mat2x2 MatOne(1, 2, 3, 4);
    Mat2x2 res1 = MatOne.transpose();

    glm::mat2 MathOne = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = glm::transpose(MathOne);

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("Inv Mat2x2", "[Mat]") {
    Mat2x2 MathA1(1, 2, 3, 4);
    Mat2x2 res1 = MathA1.reverse();

    glm::mat2 MathA2 = glm::mat2(1, 2, 3, 4);
    glm::mat2 res2 = glm::inverse(MathA2);

    REQUIRE(res1[0][0] == res2[0][0]);
    REQUIRE(res1[0][1] == res2[0][1]);
    REQUIRE(res1[1][0] == res2[1][0]);
    REQUIRE(res1[1][1] == res2[1][1]);
}

TEST_CASE("Row Mat2x2", "[Mat]") {
    Mat2x2 MathA1(144, 12, 78, -10);
    Vec2 Row1(MathA1.Row(0));

    glm::mat2 MathA2 = glm::mat2(144, 12, 78, -10);
    auto Row2 = glm::vec2(glm::row(MathA2, 0));

    REQUIRE(Row1.x == Row2.x);
    REQUIRE(Row1.y == Row2.y);
}

TEST_CASE("Column Mat2x2", "[Mat]") {
    Mat2x2 MathA1(36, 26, 100, -2);
    Vec2 Col1(MathA1.Column(0));

    glm::mat2 MathA2 = glm::mat2(36, 26, 100, -2);
    auto Col2 = glm::vec2(glm::column(MathA2, 0));

    REQUIRE(Col1.x == Col2.x);
    REQUIRE(Col1.y == Col2.y);
}

TEST_CASE("== Mat2x2", "[Mat]") {
    Mat2x2 MathA1(144, 12, 78, -10);
    Mat2x2 MathB1(144, 12, 78, -10);
    bool res1 = MathA1 == MathB1;

    glm::mat2 MathA2(144, 12, 78, -10);
    glm::mat2 MathB2(144, 12, 78, -10);
    bool res2 = MathA2 == MathB2;

    REQUIRE(res1 == res2);
}

TEST_CASE("!= Mat2x2", "[Mat]") {
    Mat2x2 MatA1(19, 70, 25, -6);
    Mat2x2 MatB1(25, 70, 19, -100);
    bool res1 = MatA1 != MatB1;

    glm::mat2 MathA2 = glm::mat2(19, 70, 25, -6);
    glm::mat2 MathB2 = glm::mat2(25, 70, 19, -100);
    bool res2 = MathA2 != MathB2;

    REQUIRE(res1 == res2);
}

/* \\\\\\\\\\\\\\\ TEST FOR MATRIX 3 ////////////// */


TEST_CASE("Constructor Ma3x3", "[Mat]") {
    auto res2 = glm::mat3(9);
    Mat3x3 res1(9);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("+ Mat3x3", "[Mat]") {
    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne + matTwo);

    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne + MAthTwo;

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("+= Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    MathOne += MAthTwo;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(1, 2, 3, 4, 5, 6, 7, 8, 9);
    matOne += matTwo;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("|x| = 1 Mat3x3", "[Mat]") {
    auto res2 = glm::mat3(1);
    Mat3x3 res1 = Mat3x3::identity();

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("- Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(9, 8, 7, 6, 5, 4, 3, 2, 1);
    glm::mat3 res2 = MathOne - MAthTwo;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(9, 8, 7, 6, 5, 4, 3, 2, 1);
    Mat3x3 res1(matOne - matTwo);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("-= Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(9, 8, 7, 6, 5, 4, 3, 2, 1);
    MathOne -= MAthTwo;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(9, 8, 7, 6, 5, 4, 3, 2, 1);
    matOne -= matTwo;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("* Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(9, 8, 7, 6, 5, 4, 3, 2, 1);
    glm::mat3 res2 = MathOne * MAthTwo;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(9, 8, 7, 6, 5, 4, 3, 2, 1);
    Mat3x3 res1(matOne * matTwo);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("*= Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(9, 8, 7, 6, 5, 4, 3, 2, 1);
    MathOne *= MAthTwo;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(9, 8, 7, 6, 5, 4, 3, 2, 1);
    matOne *= matTwo;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("*x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne * 17.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne * 17.0f);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("*=x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    MathOne *= 17.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    matOne *= 17.0f;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("/x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne / 89.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne / 89.0f);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("/=x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    MathOne /= 89.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    matOne /= 89.0f;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("-x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne - 100.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne - 100.0f);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("-=x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    MathOne -= 100.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    matOne -= 100.0f;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("+x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne + 27.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne + 27.0f);

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("+=x Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    MathOne += 27.0f;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    matOne += 27.0f;

    REQUIRE(MathOne[0][0] == matOne[0][0]);
    REQUIRE(MathOne[0][1] == matOne[0][1]);
    REQUIRE(MathOne[0][2] == matOne[0][2]);

    REQUIRE(MathOne[1][0] == matOne[1][0]);
    REQUIRE(MathOne[1][1] == matOne[1][1]);
    REQUIRE(MathOne[1][2] == matOne[1][2]);

    REQUIRE(MathOne[2][0] == matOne[2][0]);
    REQUIRE(MathOne[2][1] == matOne[2][1]);
    REQUIRE(MathOne[2][2] == matOne[2][2]);
}

TEST_CASE("*vec Mat3x3", "[Mat]") {
    glm::mat3 MatOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::vec3 mat2 = glm::vec3(10, 11, 12);
    glm::vec3 res2 = MatOne * mat2;

    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Vec3 vecOne(10, 11, 12);
    Vec3 res1(matOne * vecOne);

    REQUIRE(res1.x == res2.x);
    REQUIRE(res1.y == res2.y);
    REQUIRE(res1.z == res2.z);
}

TEST_CASE("Reverse- Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = glm::inverse(MathOne);

    Mat3x3 MatOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1 = MatOne.reverse();

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("Trans Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = glm::transpose(MathOne);

    Mat3x3 MatOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1 = MatOne.transpose();

    REQUIRE(res2[0][0] == res1[0][0]);
    REQUIRE(res2[0][1] == res1[0][1]);
    REQUIRE(res2[0][2] == res1[0][2]);

    REQUIRE(res2[1][0] == res1[1][0]);
    REQUIRE(res2[1][1] == res1[1][1]);
    REQUIRE(res2[1][2] == res1[1][2]);

    REQUIRE(res2[2][0] == res1[2][0]);
    REQUIRE(res2[2][1] == res1[2][1]);
    REQUIRE(res2[2][2] == res1[2][2]);
}

TEST_CASE("Row Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(144, 12, 78, -10, 23, 0, 8, 100, 89);
    glm::vec3 Row1(glm::row(MathOne, 2));

    Mat3x3 Mat2x2One(144, 12, 78, -10, 23, 0, 8, 100, 89);
    Vec3 Row2(Mat2x2One.Row(2));

    REQUIRE(Row1.x == Row2.x);
    REQUIRE(Row1.y == Row2.y);
    REQUIRE(Row1.z == Row2.z);
}

TEST_CASE("Col Mat3x3", "[Mat]") {
    glm::mat3 MathOne = glm::mat3(36, 26, 100, -2, 1, 1, 30, 20, 18);
    glm::vec3 Col1(glm::column(MathOne, 1));

    Mat3x3 Mat2x2One(36, 26, 100, -2, 1, 1, 30, 20, 18);
    Vec3 Col2(Mat2x2One.Column(1));

    REQUIRE(Col1.x == Col2.x);
    REQUIRE(Col1.y == Col2.y);
    REQUIRE(Col1.z == Col2.z);
}

TEST_CASE("== Mat3x3", "[Mat]") {
    Mat3x3 Mat2x2One(144, 12, 78, -10, 23, 0, 8, 100, 89);
    Mat3x3 Mat2x2Two(144, 12, 78, -10, 23, 0, 8, 100, 89);
    bool res1 = Mat2x2One == Mat2x2Two;

    glm::mat3 MathOne = glm::mat3(144, 12, 78, -10, 23, 0, 8, 100, 89);
    glm::mat3 MathTwo = glm::mat3(144, 12, 78, -10, 23, 0, 8, 100, 89);
    bool res2 = MathOne == MathTwo;

    REQUIRE(res1 == res2);
}

TEST_CASE("!= Mat3x3", "[Mat]") {
    Mat3x3 Mat2x2One(19, 70, 25, -6, 0, 0, 0, 0, 0);
    Mat3x3 Mat2x2Two(25, 70, 19, -100, 0, 0, 0, 0, 0);
    bool res1 = Mat2x2One != Mat2x2Two;

    glm::mat3 MathOne = glm::mat3(19, 70, 25, -6, 0, 0, 0, 0, 0);
    glm::mat3 MathTwo = glm::mat3(25, 70, 19, -100, 0, 0, 0, 0, 0);
    bool res2 = MathOne != MathTwo;

    REQUIRE(res1 == res2);
}