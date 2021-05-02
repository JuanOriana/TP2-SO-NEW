// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <timerTick.h>
#include <stringLib.h>

static unsigned long ticks = 0;

void timerHandler() {
      ticks++;
}

int ticksElapsed() {
      return ticks;
}

int secondsElapsed() {
	return ticks / 18;
}
