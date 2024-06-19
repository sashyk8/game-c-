/**
 * @file test_main.cpp
 * @brief Unit tests for the game simulation functions.
 */

#include <gtest/gtest.h>
#include <cmath>

// Прототипы функций и переменные из основного кода
void SimulatePlayer(float *p, float *dp, float ddp, float dt);
bool AabbVsAabb(float p1x, float p1y, float hs1x, float hs1y,
                float p2x, float p2y, float hs2x, float hs2y);

/**
 * @brief Tests the position and velocity update in SimulatePlayer function.
 */
TEST(SimulatePlayerTest, PositionAndVelocityUpdate) {
    float p = 0.0f, dp = 0.0f, ddp = 100.0f, dt = 0.1f;
    SimulatePlayer(&p, &dp, ddp, dt);
    EXPECT_NEAR(p, 0.5f * ddp * dt * dt, 1e-5);
    EXPECT_NEAR(dp, ddp * dt, 1e-5);
}

/**
 * @brief Tests the collision with the top boundary in SimulatePlayer function.
 */
TEST(SimulatePlayerTest, BoundaryCollisionTop) {
    float p = 44.0f, dp = 0.0f, ddp = 100.0f, dt = 0.1f;
    SimulatePlayer(&p, &dp, ddp, dt);
    EXPECT_NEAR(p, 45.0f - 12.0f, 1e-5);
    EXPECT_NEAR(dp, 0.0f, 1e-5);
}

/**
 * @brief Tests the collision with the bottom boundary in SimulatePlayer function.
 */
TEST(SimulatePlayerTest, BoundaryCollisionBottom) {
    float p = -44.0f, dp = 0.0f, ddp = -100.0f, dt = 0.1f;
    SimulatePlayer(&p, &dp, ddp, dt);
    EXPECT_NEAR(p, -45.0f + 12.0f, 1e-5);
    EXPECT_NEAR(dp, 0.0f, 1e-5);
}

/**
 * @brief Tests the intersection of two boxes in AabbVsAabb function.
 */
TEST(AabbVsAabbTest, Intersection) {
    EXPECT_TRUE(AabbVsAabb(0, 0, 1, 1, 0.5, 0.5, 1, 1));
    EXPECT_TRUE(AabbVsAabb(0, 0, 1, 1, -0.5, -0.5, 1, 1));
}

/**
 * @brief Tests the non-intersection of two boxes in AabbVsAabb function.
 */
TEST(AabbVsAabbTest, NoIntersection) {
    EXPECT_FALSE(AabbVsAabb(0, 0, 1, 1, 3, 3, 1, 1));
    EXPECT_FALSE(AabbVsAabb(0, 0, 1, 1, -3, -3, 1, 1));
}

/**
 * @brief Tests the ball collision with the player in SimulateGame function.
 */
TEST(GameSimulationTest, BallCollisionWithPlayer) {
    float ball_p_x = 79.0f, ball_p_y = 0.0f, ball_half_size = 1.0f;
    float ball_dp_x = 130.0f, ball_dp_y = 0.0f;
    float player_1_p = 0.0f, player_half_size_x = 2.5f, player_half_size_y = 12.0f;
    float dt = 0.1f;

    ball_p_x += ball_dp_x * dt;
    if (AabbVsAabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
        ball_p_x = 80 - player_half_size_x - ball_half_size;
        ball_dp_x *= -1;
    }

    EXPECT_NEAR(ball_p_x, 80.0f - player_half_size_x - ball_half_size, 1e-5);
    EXPECT_NEAR(ball_dp_x, -130.0f, 1e-5);
}

/**
 * @brief Main function to run all tests.
 * 
 * @param argc Number of arguments.
 * @param argv Argument values.
 * @return int Result of the test execution.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
