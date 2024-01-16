/*
 * _DEBUG_.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: Farhan Ahmed
 */


#include "_DEBUG_.h"
#include <stdarg.h>
#include "stdio.h"



int DEBUG_(const char *sFormat, ...) {
  int r;
  va_list paramList;

  char formattedString[512];  // Adjust the size as needed
  va_start(paramList, sFormat);
  vsnprintf(formattedString, sizeof(formattedString), sFormat, paramList);
  va_end(paramList);

  // Concatenate ANSI color code with the formatted string
  char coloredString[700];  // Adjust the size as needed
  snprintf(coloredString, sizeof(coloredString), "%s\n", formattedString);

  // Call SEGGER_RTT_printf with the colored string
  r = SEGGER_RTT_printf(0, coloredString);

  return r;
}

int DEBUG_E(const char *sFormat, ...) {
  int r;
  va_list paramList;

  char formattedString[512];  // Adjust the size as needed
  va_start(paramList, sFormat);
  vsnprintf(formattedString, sizeof(formattedString), sFormat, paramList);
  va_end(paramList);

  // Concatenate ANSI color code with the formatted string
  char coloredString[700];  // Adjust the size as needed
  snprintf(coloredString, sizeof(coloredString), "%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_RED, formattedString, RTT_CTRL_RESET);

  // Call SEGGER_RTT_printf with the colored string
  r = SEGGER_RTT_printf(0, coloredString);

  return r;
}

int DEBUG_W(const char *sFormat, ...) {
  int r;
  va_list paramList;

  char formattedString[512];  // Adjust the size as needed
  va_start(paramList, sFormat);
  vsnprintf(formattedString, sizeof(formattedString), sFormat, paramList);
  va_end(paramList);

  // Concatenate ANSI color code with the formatted string
  char coloredString[700];  // Adjust the size as needed
  snprintf(coloredString, sizeof(coloredString), "%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_YELLOW, formattedString, RTT_CTRL_RESET);

  // Call SEGGER_RTT_printf with the colored string
  r = SEGGER_RTT_printf(0, coloredString);

  return r;
}

int DEBUG_I(const char *sFormat, ...) {
  int r;
  va_list paramList;

  char formattedString[512];  // Adjust the size as needed
  va_start(paramList, sFormat);
  vsnprintf(formattedString, sizeof(formattedString), sFormat, paramList);
  va_end(paramList);

  // Concatenate ANSI color code with the formatted string
  char coloredString[700];  // Adjust the size as needed
  snprintf(coloredString, sizeof(coloredString), "%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_GREEN, formattedString, RTT_CTRL_RESET);

  // Call SEGGER_RTT_printf with the colored string
  r = SEGGER_RTT_printf(0, coloredString);

  return r;
}

int DEBUG_A(const char *sFormat, ...) {
  int r;
  va_list paramList;

  char formattedString[512];  // Adjust the size as needed
  va_start(paramList, sFormat);
  vsnprintf(formattedString, sizeof(formattedString), sFormat, paramList);
  va_end(paramList);

  // Concatenate ANSI color code with the formatted string
  char coloredString[700];  // Adjust the size as needed
  snprintf(coloredString, sizeof(coloredString), "%s%s%s%s\n", RTT_CTRL_BG_WHITE, RTT_CTRL_TEXT_BRIGHT_RED, formattedString, RTT_CTRL_RESET);

  // Call SEGGER_RTT_printf with the colored string
  r = SEGGER_RTT_printf(0, coloredString);

  return r;
}
