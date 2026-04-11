#ifndef HELPER_H
#define HELPER_H

// PROGRAM SETTINGS
#define CONSOLE_LOGGING true

#define LOG(x) \ 
  if (CONSOLE_LOGGING) { \
    Monitor.print(x); \
  }

// FUNCTIONS ===================================================================
bool HasValidTarget();

#endif