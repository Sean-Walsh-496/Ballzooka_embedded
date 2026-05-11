#ifndef HELPER_H
#define HELPER_H

// PROGRAM SETTINGS
#define CONSOLE_LOGGING true

// Macro that easily lets you disable or enable console logging
#define LOG(x) \ 
  if (CONSOLE_LOGGING) { \
    Monitor.print(x); \
  }

// FUNCTIONS ===================================================================
bool HasValidTarget();

#endif