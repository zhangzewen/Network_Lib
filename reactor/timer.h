#ifndef REACTOR_TIMER_H_
#define REACTOR_TIMER_H_

#include <sys/time.h>
#include <string>

#ifndef NULL
#define NULL 0
#endif

class Timer
{
public:
    Timer (){
        gettimeofday(&time_, NULL);
    }

    Timer(int timeout) {
        time_.tv_sec = timeout % 1000000;
        time_.tv_usec = timeout / 1000000;
    }

    ~Timer(){}

    Timer(const Timer& time) {
        this->time_.tv_sec = time.time_.tv_sec;
        this->time_.tv_usec = time.time_.tv_usec;
    }

    const Timer operator +(const Timer& time) {
        Timer result;
        result.time_.tv_sec = this->time_.tv_sec + time.time_.tv_sec;
        result.time_.tv_usec = this->time_.tv_usec + time.time_.tv_usec;

        if (result.time_.tv_usec >= 1000000) {
            result.time_.tv_usec -= 1000000;
            result.time_.tv_sec += 1;
        }
        return result;
    }

    Timer operator -(const Timer& time) {
        Timer result;
        result.time_.tv_sec = this->time_.tv_sec - time.time_.tv_sec;
        result.time_.tv_usec = this->time_.tv_usec - time.time_.tv_usec;

        if (result.time_.tv_usec < 0) {
            result.time_.tv_usec += 1000000;
            result.time_.tv_sec -= 1;
        }
        return result;
    }

    Timer& operator =(const Timer& time) {
        this->time_.tv_sec = time.time_.tv_sec;
        this->time_.tv_usec = time.time_.tv_usec;
        return *this;
    }

    bool operator <(const Timer& time) const {
        return this->convertToMilliseconds() < time.convertToMilliseconds();
    }

    bool operator >(const Timer& time) {
        return this->convertToMilliseconds() > time.convertToMilliseconds();
    }

    bool operator ==(const Timer& time) {
        return (this->time_.tv_sec == time.time_.tv_sec
            && this->time_.tv_usec == time.time_.tv_usec);
    }

    bool operator <=(const Timer& time) {
        return (*this == time || *this < time);
    }

    bool operator >=(const Timer& time) {
        return (*this > time || *this == time);
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
    std::string toString() const {
        char buf[256] = {0};
        sprintf(buf, "{tv_sec: %ld, tv_usec: %ld}", time_.tv_sec, time_.tv_usec);
        return buf;
    }

private:
    struct timeval time_;
};

#endif
