/**
 * @file renderer.cpp
 * @brief Реализация функции для работы с экраном.
 */

/**
 * @brief Заполняет весь экран (или область памяти, представляющую экран) заданным цветом.
 * 
 * Функция перебирает все пиксели на экране и задает им указанный цвет.
 * 
 * @param color Цвет, которым будет заполнен экран. Тип u32 (обычно это 32-битное целое число, представляющее цвет в формате ARGB или RGBA).
 * 
 * @return void Функция не возвращает значения.
 */
internal void
clear_screen(u32 color) {
  unsigned int* pixel = (u32*)render_state.memory;
  for (int y = 0; y < render_state.height; y++) {
    for (int x = 0; x < render_state.width; x++) {
      *pixel++ = color;
    }
  }
}

/**
 * @file renderer.cpp
 * @brief Реализация функций для работы с экраном.
 */

/**
 * @brief Рисует прямоугольник на экране, используя координаты пикселей для указания его позиции и размеров.
 * 
 * Функция рисует прямоугольник, заполняя его указанным цветом. Координаты прямоугольника автоматически
 * ограничиваются границами экрана.
 * 
 * @param x0 Координата X верхнего левого угла прямоугольника.
 * @param y0 Координата Y верхнего левого угла прямоугольника.
 * @param x1 Координата X нижнего правого угла прямоугольника.
 * @param y1 Координата Y нижнего правого угла прямоугольника.
 * @param color Цвет, которым будет заполнен прямоугольник. Тип u32 (обычно это 32-битное целое число, представляющее цвет в формате ARGB или RGBA).
 * 
 * @return void Функция не возвращает значения.
 */
internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {
  
  x0 = clamp(0, x0, render_state.width);
  x1 = clamp(0, x1, render_state.width);
  y0 = clamp(0, y0, render_state.height);
  y1 = clamp(0, y1, render_state.height);

  for (int y = y0; y < y1; y++) {
    u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
    for (int x = x0; x < x1; x++) {
      *pixel++ = color;
    }
  }
}

/**
 * @brief Масштаб рендеринга, используется для преобразования координат.
 * 
 * Глобальная переменная, которая задает масштаб для преобразования координат логических единиц в пиксели.
 */
global_variable float render_scale = 0.01f;


/**
 * @file render.cpp
 * @brief Реализация функций для работы с экраном.
 */

/**
 * @brief Рисует границы арены на экране с заданным цветом.
 * 
 * Функция рисует границы арены, определенные параметрами `arena_x` и `arena_y`, заполняя области за пределами арены указанным цветом.
 * Координаты арены преобразуются с учетом масштаба и размеров экрана.
 * 
 * @param arena_x Ширина арены в логических единицах.
 * @param arena_y Высота арены в логических единицах.
 * @param color Цвет, которым будут заполнены области за пределами арены. Тип u32 (обычно это 32-битное целое число, представляющее цвет в формате ARGB или RGBA).
 * 
 * @return void Функция не возвращает значения.
 */
internal void draw_arena_borders(float arena_x, float arena_y, u32 color) {
  arena_x *= render_state.height * render_scale;
  arena_y *= render_state.height * render_scale;

  int x0 = (int)((float)render_state.width * .5f - arena_x);
  int x1 = (int)((float)render_state.width * .5f + arena_x);
  int y0 = (int)((float)render_state.height * .5f - arena_y);
  int y1 = (int)((float)render_state.height * .5f + arena_y);

  draw_rect_in_pixels(0, 0, render_state.width, y0, color);
  draw_rect_in_pixels(0, y1, x1, render_state.height, color);
  draw_rect_in_pixels(0, y0, x0, y1, color);
  draw_rect_in_pixels(x1, y0, render_state.width, render_state.height, color);
}

/**
 * @brief Рисует прямоугольник на экране, используя логические координаты и размеры.
 * 
 * Функция рисует прямоугольник с заданным цветом, используя логические координаты и размеры. Эти координаты преобразуются в пиксели
 * с учетом масштаба и размеров экрана.
 * 
 * @param x Координата X центра прямоугольника в логических единицах.
 * @param y Координата Y центра прямоугольника в логических единицах.
 * @param half_size_x Половина ширины прямоугольника в логических единицах.
 * @param half_size_y Половина высоты прямоугольника в логических единицах.
 * @param color Цвет, которым будет заполнен прямоугольник. Тип u32 (обычно это 32-битное целое число, представляющее цвет в формате ARGB или RGBA).
 * 
 * @return void Функция не возвращает значения.
 */
internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {

  x *= render_state.height * render_scale;
  y *= render_state.height * render_scale;
  half_size_x *= render_state.height * render_scale;
  half_size_y *= render_state.height * render_scale;

  x += render_state.width / 2.f;
  y += render_state.height / 2.f;

  // Преобразование в пиксели
  int x0 = x - half_size_x;
  int x1 = x + half_size_x;
  int y0 = y - half_size_y;
  int y1 = y + half_size_y;

  draw_rect_in_pixels(x0, y0, x1, y1, color);
}


const char* letters[][7] = {
    " 00",
    "0  0",
    "0  0",
    "0000",
    "0  0",
    "0  0",
    "0  0",

  "000",
  "0  0",
  "0  0",
  "000",
  "0  0",
  "0  0",
  "000",

  " 000",
  "0",
  "0",
  "0",
  "0",
  "0",
  " 000",

  "000",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "000",

  "0000",
  "0",
  "0",
  "000",
  "0",
  "0",
  "0000",

  "0000",
  "0",
  "0",
  "000",
  "0",
  "0",
  "0",

  " 000",
  "0",
  "0",
  "0 00",
  "0  0",
  "0  0",
  " 000",

  "0  0",
  "0  0",
  "0  0",
  "0000",
  "0  0",
  "0  0",
  "0  0",

  "000",
  " 0",
  " 0",
  " 0",
  " 0",
  " 0",
  "000",

  " 000",
  "   0",
  "   0",
  "   0",
  "0  0",
  "0  0",
  " 000",

  "0  0",
  "0  0",
  "0 0",
  "00",
  "0 0",
  "0  0",
  "0  0",

  "0",
  "0",
  "0",
  "0",
  "0",
  "0",
  "0000",

  "00 00",
  "0 0 0",
  "0 0 0",
  "0   0",
  "0   0",
  "0   0",
  "0   0",

  "00  0",
  "0 0 0",
  "0 0 0",
  "0 0 0",
  "0 0 0",
  "0 0 0",
  "0  00",

  "0000",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "0000",

  " 000",
  "0  0",
  "0  0",
  "000",
  "0",
  "0",
  "0",

  " 000 ",
  "0   0",
  "0   0",
  "0   0",
  "0 0 0",
  "0  0 ",
  " 00 0",

  "000",
  "0  0",
  "0  0",
  "000",
  "0  0",
  "0  0",
  "0  0",

  " 000",
  "0",
  "0 ",
  " 00",
  "   0",
  "   0",
  "000 ",

  "000",
  " 0",
  " 0",
  " 0",
  " 0",
  " 0",
  " 0",

  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  "0  0",
  " 00",

  "0   0",
  "0   0",
  "0   0",
  "0   0",
  "0   0",
  " 0 0",
  "  0",

  "0   0 ",
  "0   0",
  "0   0",
  "0 0 0",
  "0 0 0",
  "0 0 0",
  " 0 0 ",

  "0   0",
  "0   0",
  " 0 0",
  "  0",
  " 0 0",
  "0   0",
  "0   0",

  "0   0",
  "0   0",
  " 0 0",
  "  0",
  "  0",
  "  0",
  "  0",

  "0000",
  "   0",
  "  0",
  " 0",
  "0",
  "0",
  "0000",

  "",
  "",
  "",
  "",
  "",
  "",
  "0",

  "   0",
  "  0",
  "  0",
  " 0",
  " 0",
  "0",
  "0",
};

internal void
draw_text(const char *text, float x, float y, float size, u32 color) {
  float half_size = size * .5f;
  float original_y = y;

  while (*text) {
    if (*text != 32) {
      const char** letter;
      if (*text == 47) letter = letters[27];
      else if (*text == 46) letter = letters[26];
      else letter = letters[*text - 'A'];
      float original_x = x;

      for (int i = 0; i < 7; i++) {
        const char* row = letter[i];
        while (*row) {
          if (*row == '0') {
            draw_rect(x, y, half_size, half_size, color);
          }
          x += size;
          row++;
        }
        y -= size;
        x = original_x;
      }
    }
    text++;
    x += size * 6.f;
    y = original_y;
  }
}

internal void
draw_number(int number, float x, float y, float size, u32 color) {
  float half_size = size * .5f;

  bool drew_number = false;
  while (number || !drew_number) {
    drew_number = true;

    int digit = number % 10;
    number = number / 10;

    switch (digit) {
    case 0: {
      draw_rect(x - size, y, half_size, 2.5f * size, color);
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      draw_rect(x, y + size * 2.f, half_size, half_size, color);
      draw_rect(x, y - size * 2.f, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 1: {
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      x -= size * 2.f;
    } break;

    case 2: {
      draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
      draw_rect(x, y, 1.5f * size, half_size, color);
      draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
      draw_rect(x + size, y + size, half_size, half_size, color);
      draw_rect(x - size, y - size, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 3: {
      draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
      draw_rect(x - half_size, y, size, half_size, color);
      draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      x -= size * 4.f;
    } break;

    case 4: {
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      draw_rect(x - size, y + size, half_size, 1.5f * size, color);
      draw_rect(x, y, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 5: {
      draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
      draw_rect(x, y, 1.5f * size, half_size, color);
      draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
      draw_rect(x - size, y + size, half_size, half_size, color);
      draw_rect(x + size, y - size, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 6: {
      draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
      draw_rect(x + half_size, y, size, half_size, color);
      draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
      draw_rect(x - size, y, half_size, 2.5f * size, color);
      draw_rect(x + size, y - size, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 7: {
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
      x -= size * 4.f;
    } break;

    case 8: {
      draw_rect(x - size, y, half_size, 2.5f * size, color);
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      draw_rect(x, y + size * 2.f, half_size, half_size, color);
      draw_rect(x, y - size * 2.f, half_size, half_size, color);
      draw_rect(x, y, half_size, half_size, color);
      x -= size * 4.f;
    } break;

    case 9: {
      draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
      draw_rect(x - half_size, y, size, half_size, color);
      draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
      draw_rect(x + size, y, half_size, 2.5f * size, color);
      draw_rect(x - size, y + size, half_size, half_size, color);
      x -= size * 4.f;
    } break;
    }

  }
}
    
