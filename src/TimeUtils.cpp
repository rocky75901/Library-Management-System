#include "TimeUtils.h"
#include <chrono>
#include <ctime>

using namespace std::chrono;

void getCurrentTime(int &minutes, int &seconds) {
    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm *ltm = localtime(&tt);
    minutes = ltm->tm_min + (ltm->tm_hour * 60);
    seconds = ltm->tm_sec;
}

int convertToSimulatedDays(int borrowMinutes, int borrowSeconds, int currentMinutes, int currentSeconds) {
    int totalBorrowSeconds = borrowMinutes * 60 + borrowSeconds;
    int totalCurrentSeconds = currentMinutes * 60 + currentSeconds;
    int elapsedSeconds = totalCurrentSeconds - totalBorrowSeconds;
    if (elapsedSeconds < 0) elapsedSeconds += 24 * 60 * 60;
    return elapsedSeconds / 4;
}

void addSimulatedDays(int &minutes, int &seconds, int simulatedDays) {
    int totalSeconds = minutes * 60 + seconds + (simulatedDays * 4);
    int totalMinutes = totalSeconds / 60;
    minutes = totalMinutes % (24 * 60);
    seconds = totalSeconds % 60;
}