#include <mat4x4.h>
#include <mat4x4.hpp>

#include <iostream>

int REQUIRE(float x, float y) {
    std::cout << x << " " << y << std::endl;
}

int main() {
    glm::mat4 MathOne = glm::mat4(3, 3, 0, 1, 3, 2, 7, 7, 0, 7, 0, 2, 1, 7, 2, 0);
    glm::mat4 res2 = glm::inverse(MathOne);

    Mat4x4 MatOne(3, 3, 0, 1, 3, 2, 7, 7, 0, 7, 0, 2, 1, 7, 2, 0);
    Mat4x4 res1 = MatOne.reverse();

    REQUIRE(res1[0][0], res2[0][0]);
    REQUIRE(res1[0][1], res2[0][1]);
    REQUIRE(res1[0][2], res2[0][2]);
    REQUIRE(res1[0][3], res2[0][3]);

    REQUIRE(res1[1][0], res2[1][0]);
    REQUIRE(res1[1][1], res2[1][1]);
    REQUIRE(res1[1][2], res2[1][2]);
    REQUIRE(res1[1][3], res2[1][3]);

    REQUIRE(res1[2][0], res2[2][0]);
    REQUIRE(res1[2][1], res2[2][1]);
    REQUIRE(res1[2][2], res2[2][2]);
    REQUIRE(res1[2][3], res2[2][3]);

    REQUIRE(res1[3][0], res2[3][0]);
    REQUIRE(res1[3][1], res2[3][1]);
    REQUIRE(res1[3][2], res2[3][2]);
    REQUIRE(res1[3][3], res2[3][3]);
    return 0;
}