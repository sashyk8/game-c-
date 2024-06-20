/**
 * @file utils.cpp
 * @brief Определения типов и макросов.
 */

/**
 * @typedef s8
 * @brief Определяет тип знакового 8-битного целого числа.
 */
typedef char s8;

/**
 * @typedef u8
 * @brief Определяет тип беззнакового 8-битного целого числа.
 */
typedef unsigned char u8;

/**
 * @typedef s16
 * @brief Определяет тип знакового 16-битного целого числа.
 */
typedef short s16;

/**
 * @typedef u16
 * @brief Определяет тип беззнакового 16-битного целого числа.
 */
typedef unsigned short u16;

/**
 * @typedef s32
 * @brief Определяет тип знакового 32-битного целого числа.
 */
typedef int s32;

/**
 * @typedef u32
 * @brief Определяет тип беззнакового 32-битного целого числа.
 */
typedef unsigned int u32;

/**
 * @typedef s64
 * @brief Определяет тип знакового 64-битного целого числа.
 */
typedef long long s64;

/**
 * @typedef u64
 * @brief Определяет тип беззнакового 64-битного целого числа.
 */
typedef unsigned long long u64;

/**
 * @def global_variable
 * @brief Определяет глобальную переменную как статическую.
 */
#define global_variable static

/**
 * @def internal
 * @brief Определяет внутреннюю (статическую) функцию.
 */
#define internal static

/**
 * @brief Ограничивает значение заданным диапазоном.
 *
 * Функция принимает минимальное и максимальное значение и возвращает значение, ограниченное этим диапазоном.
 *
 * @param min Минимальное значение.
 * @param val Текущее значение.
 * @param max Максимальное значение.
 *
 * @return Ограниченное значение, если оно выходит за пределы диапазона.
 */
inline int clamp(int min, int val, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
