#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;

void SimulatePlayer(float *p, float *dp, float ddp, float dt) {
  ddp -= *dp * 10.f;

  *p = *p + *dp * dt + ddp * dt * dt * .5f;
  *dp = *dp + ddp * dt;

  if (*p + player_half_size_y > arena_half_size_y) {
    *p = arena_half_size_y - player_half_size_y;
    *dp = 0;
  } else if (*p - player_half_size_y < -arena_half_size_y) {
    *p = -arena_half_size_y + player_half_size_y;
    *dp = 0;
  }
}

bool AabbVsAabb(float p1x, float p1y, float hs1x, float hs1y,
                float p2x, float p2y, float hs2x, float hs2y) {
  return (p1x + hs1x > p2x - hs2x &&
          p1x - hs1x < p2x + hs2x &&
          p1y + hs1y > p2y - hs2y &&
          p1y + hs1y < p2y + hs2y);
}

enum Gamemode {
  kMenu,
  kGameplay,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

void SimulateGame(Input* input, float dt) {
  DrawRect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);
  DrawArenaBorders(arena_half_size_x, arena_half_size_y, 0xff5500);

  if (current_gamemode == kGameplay) {
    float player_1_ddp = 0.f;
    if (!enemy_is_ai) {
      if (is_down(BUTTON_UP)) player_1_ddp += 2000;
      if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
    } else {
      player_1_ddp = (ball_p_y - player_1_p) * 100;
      if (player_1_ddp > 1300) player_1_ddp = 1300;
      if (player_1_ddp < -1300) player_1_ddp = -1300;
    }

    float player_2_ddp = 0.f;
    if (is_down(BUTTON_W)) player_2_ddp += 2000;
    if (is_down(BUTTON_S)) player_2_ddp -= 2000;

    SimulatePlayer(&player_1_p, &player_1_dp, player_1_ddp, dt);
    SimulatePlayer(&player_2_p, &player_2_dp, player_2_ddp, dt);

    // Simulate Ball
    {
      ball_p_x += ball_dp_x * dt;
      ball_p_y += ball_dp_y * dt;

      if (AabbVsAabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
        ball_p_x = 80 - player_half_size_x - ball_half_size;
        ball_dp_x *= -1;
        ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
      } else if (AabbVsAabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
        ball_p_x = -80 + player_half_size_x + ball_half_size;
        ball_dp_x *= -1;
        ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
      }

      if (ball_p_y + ball_half_size > arena_half_size_y) {
        ball_p_y = arena_half_size_y - ball_half_size;
        ball_dp_y *= -1;
      } else if (ball_p_y - ball_half_size < -arena_half_size_y) {
        ball_p_y = -arena_half_size_y + ball_half_size;
        ball_dp_y *= -1;
      }

      if (ball_p_x + ball_half_size > arena_half_size_x) {
        ball_dp_x *= -1;
        ball_dp_y = 0;
        ball_p_x = 0;
        ball_p_y = 0;
        player_1_score++;
      } else if (ball_p_x - ball_half_size < -arena_half_size_x) {
        ball_dp_x *= -1;
        ball_dp_y = 0;
        ball_p_x = 0;
        ball_p_y = 0;
        player_2_score++;
      }
    }

    DrawNumber(player_1_score, -10, 40, 1.f, 0xbbffbb);
    DrawNumber(player_2_score, 10, 40, 1.f, 0xbbffbb);

    // Rendering
    DrawRect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);
    DrawRect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);
    DrawRect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);

  } else {
    if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
      hot_button = !hot_button;
    }

    if (pressed(BUTTON_ENTER)) {
      current_gamemode = kGameplay;
      enemy_is_ai = hot_button ? 0 : 1;
    }

    if (hot_button == 0) {
      DrawText("SINGLE PLAYER", -80, -10, 1, 0xff0000);
      DrawText("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
    } else {
      DrawText("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
      DrawText("MULTIPLAYER", 20, -10, 1, 0xff0000);
    }
  }
}

