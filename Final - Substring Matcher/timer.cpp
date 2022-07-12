#include "timer.h"

void Timer::tick()
{
    m_begin = std::chrono::high_resolution_clock::now();
}

void Timer::tock()
{
    m_end = std::chrono::high_resolution_clock::now();
}

double Timer::duration()
{
    // Elapased time
    return std::chrono::duration<double, std::milli>(m_end - m_begin).count();
}
