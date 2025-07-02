#ifndef TIME_UTILS_H
#define TIME_UTILS_H

void getCurrentTime(int &minutes, int &seconds);
int convertToSimulatedDays(int borrowMinutes, int borrowSeconds, int currentMinutes, int currentSeconds);
void addSimulatedDays(int &minutes, int &seconds, int simulatedDays);

#endif // TIME_UTILS_H