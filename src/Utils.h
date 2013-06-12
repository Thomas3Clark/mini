#pragma once

void IntToString(char *buffer, size_t bufferSize, int value);
uint16_t Random(uint16_t max);
void SetRandomSeed(long newSeed);

unsigned int GetUnixTime(PblTm *current_time);
