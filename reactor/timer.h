#ifndef REACTOR_TIMER_H_
#define REACTOR_TIMER_H_

class Timer
{
public:
    Timer& operator+(const Timer& time) {
        this.time_.tv_sec = this.time_.tv_sec + time.time_.tv_sec;
        this.time_.tv_usec = this.time_.tv_usec + time.time_.tv_usec;

        if (this.time_.tv_usec >= 1000000) {
            this.time_.tv_usec -= 1000000;
            this.time_.tv_sec += 1;
        }
        return *this;
    }

    Timer& operator-(const Timer& time) {
        this.time_.tv_sec = this.time_.tv_sec - time.time_.tv_sec;
        this.time_.tv_usec = this.time_.tv_usec - time.time_.tv_usec;

        if (this.time_.tv_usec < 0) {
            this.time_.tv_usec += 1000000;
            this.time_.tv_sec -= 1;
        }
        return *this;
    }

    bool operator<(const Timer& time) {
        return true;
    }

    bool operator>(const Timer& time) {
        return true;
    }
    void now() {
        struct timeval current_time;
        timer_reset(&current_time);
        gettimeofday(&current_time, NULL);
        time_ = current_time;

    }

private:
    struct timeval time_;
    int day_;
    int month_;
    int year_;
};

#endif
