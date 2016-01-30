#ifndef REACTOR_TIMER_H_
#define REACTOR_TIMER_H_

#include <sys/time.h>

#ifndef NULL
#define NULL 0
#endif

class Timer
{
public:
    Timer (){}
    ~Timer(){}
    void init(int timeout) {
        now();
        this->time_.tv_sec += timeout;
    }

    Timer& operator+(const Timer& time) {
        this->time_.tv_sec = this->time_.tv_sec + time.time_.tv_sec;
        this->time_.tv_usec = this->time_.tv_usec + time.time_.tv_usec;

        if (this->time_.tv_usec >= 1000000) {
            this->time_.tv_usec -= 1000000;
            this->time_.tv_sec += 1;
        }
        return *this;
    }

    Timer& operator -(const Timer& time) {
        this->time_.tv_sec = this->time_.tv_sec - time.time_.tv_sec;
        this->time_.tv_usec = this->time_.tv_usec - time.time_.tv_usec;

        if (this->time_.tv_usec < 0) {
            this->time_.tv_usec += 1000000;
            this->time_.tv_sec -= 1;
        }
        return *this;
    }

    bool operator <(const Timer& time) const {
        if (this->time_.tv_sec > time.time_.tv_sec) {
            return true;
        }
        if (this->time_.tv_usec > time.time_.tv_usec) {
            return true;
        }
        return true;
    }

    bool operator >(const Timer& time) {
        if (this->time_.tv_sec < time.time_.tv_sec) {
            return true;
        }
        if (this->time_.tv_usec < time.time_.tv_usec) {
            return true;
        }
        return true;
    }

    bool operator ==(const Timer& time) {
        return (this->time_.tv_sec == time.time_.tv_sec
            && this->time_.tv_usec == time.time_.tv_usec);
    }

    void now() {
        struct timeval current_time;
        timer_reset(&current_time);
        gettimeofday(&current_time, NULL);
        time_ = current_time;
    }

    void timer_reset() {
        this->time_.tv_sec = 0;
        this->time_.tv_usec = 0;
    }

    void timer_reset(Timer& time) {
        time.time_.tv_sec = 0;
        time.time_.tv_usec = 0;
    }

    void timer_reset(struct timeval* time) {
        time->tv_sec = 0;
        time->tv_usec = 0;
    }
    int convertToMilliseconds() const {
        return time_.tv_sec * 1000 + (time_.tv_usec + 999) / 1000;
    }

private:
    struct timeval time_;
};

#endif
