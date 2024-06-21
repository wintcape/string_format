/**
 * @file common/ansicc.h
 * @brief Defines an alias for each ANSI color code.
 */
#ifndef ANSI_CC_H
#define ANSI_CC_H

/**
 * @brief Defines an alias for an ANSI color code with one attribute.
 */
#define ANSI_CC(color) \
    "\033["color"m"

/**
 * @brief Defines an alias for an ANSI color code with two attributes.
 */
#define ANSI_CC2(color,attr) \
    "\033["attr";"color"m"

/**
 * @brief Defines an alias for the 'reset color' ANSI color code.
 */
#define ANSI_CC_RESET \
    ANSI_CC ( ANSI_CC_NONE )

#define ANSI_CC_NONE            "0" /** @brief Attribute code. */
#define ANSI_CC_BOLD            "1" /** @brief Attribute code. */
#define ANSI_CC_UNDERLINE       "2" /** @brief Attribute code. */

#define ANSI_CC_FG_BLACK        "30" /** @brief Attribute code. */
#define ANSI_CC_FG_WHITE        "97" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_GRAY    "90" /** @brief Attribute code. */
#define ANSI_CC_FG_GRAY         "37" /** @brief Attribute code. */
#define ANSI_CC_FG_RED          "91" /** @brief Attribute code. */
#define ANSI_CC_FG_GREEN        "92" /** @brief Attribute code. */
#define ANSI_CC_FG_YELLOW       "93" /** @brief Attribute code. */
#define ANSI_CC_FG_BLUE         "94" /** @brief Attribute code. */
#define ANSI_CC_FG_MAGENTA      "95" /** @brief Attribute code. */
#define ANSI_CC_FG_CYAN         "96" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_RED     "31" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_GREEN   "32" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_YELLOW  "33" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_BLUE    "34" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_MAGENTA "35" /** @brief Attribute code. */
#define ANSI_CC_FG_DARK_CYAN    "36" /** @brief Attribute code. */

#define ANSI_CC_BG_BLACK        "40"  /** @brief Attribute code. */
#define ANSI_CC_BG_WHITE        "107" /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_GRAY    "100" /** @brief Attribute code. */
#define ANSI_CC_BG_GRAY         "47"  /** @brief Attribute code. */
#define ANSI_CC_BG_RED          "101" /** @brief Attribute code. */
#define ANSI_CC_BG_GREEN        "102" /** @brief Attribute code. */
#define ANSI_CC_BG_YELLOW       "103" /** @brief Attribute code. */
#define ANSI_CC_BG_BLUE         "104" /** @brief Attribute code. */
#define ANSI_CC_BG_MAGENTA      "105" /** @brief Attribute code. */
#define ANSI_CC_BG_CYAN         "106" /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_RED     "41"  /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_GREEN   "42"  /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_YELLOW  "43"  /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_BLUE    "44"  /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_MAGENTA "45"  /** @brief Attribute code. */
#define ANSI_CC_BG_DARK_CYAN    "46"  /** @brief Attribute code. */

#endif  // ANSI_CC_H