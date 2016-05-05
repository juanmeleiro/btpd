#include <timer.h>

Timer* Timer::instance_ = nullptr;

Timer::Timer() : time_(0) {}

size_t Timer::time const() {
    return time_;
}

static Timer* Timer::instance() {
    if (instance_ == nullptr) {
        instance_ = new Timer();
    }
    return instance_;
}

void Timer::update (size_t samples) {
    time_ += samples;
}
