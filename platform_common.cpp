/**
 * @file input.h
 * @brief Содержит определения для структуры ввода и кнопок.
 */

/**
 * @struct Button_State
 * @brief Представляет состояние кнопки.
 */
struct Button_State {
    bool is_down; /**< Кнопка нажата. */
    bool changed; /**< Состояние кнопки изменилось. */
};

/**
 * @enum
 * @brief Перечисление всех кнопок.
 */
enum {
    BUTTON_UP, /**< Кнопка "Вверх". */
    BUTTON_DOWN, /**< Кнопка "Вниз". */
    BUTTON_W, /**< Кнопка "W". */
    BUTTON_S, /**< Кнопка "S". */
    BUTTON_LEFT, /**< Кнопка "Влево". */
    BUTTON_RIGHT, /**< Кнопка "Вправо". */
    BUTTON_ENTER, /**< Кнопка "Enter". */

    BUTTON_COUNT, /**< Количество кнопок. */
};

/**
 * @struct Input
 * @brief Представляет состояние ввода.
 */
struct Input {
    Button_State buttons[BUTTON_COUNT]; /**< Массив состояний кнопок. */
};
