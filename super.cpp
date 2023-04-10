#include <mat3x3.h>
#include <mat3x3.hpp>

#include <iostream>

int REQUIRE(float x, float y) {
    std::cout << x << " " << y << std::endl;
}

int main() {
    Mat3x3 matOne(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 matTwo(1, 2, 3, 4, 5, 6, 7, 8, 9);
    Mat3x3 res1(matOne + matTwo);

    glm::mat3 MathOne = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 MAthTwo = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
    glm::mat3 res2 = MathOne + MAthTwo;

    REQUIRE(res2[0][0], res1[0][0]);
    REQUIRE(res2[0][1], res1[0][1]);
    REQUIRE(res2[0][1], res1[0][2]);

    REQUIRE(res2[1][0], res1[1][0]);
    REQUIRE(res2[1][1], res1[1][1]);
    REQUIRE(res2[1][2], res1[1][2]);

    REQUIRE(res2[2][0], res1[2][0]);
    REQUIRE(res2[2][1], res1[2][1]);
    REQUIRE(res2[2][2], res1[2][2]);
    return 0;
}