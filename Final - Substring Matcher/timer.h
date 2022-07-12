#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {

public:
    void tick();

    void tock();

    double duration();

private:
    std::chrono::time_point<std::chrono::system_clock> m_begin;
    std::chrono::time_point<std::chrono::system_clock> m_end;
};
#endif // TIMER_H
