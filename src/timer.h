#ifndef UGD_TIMER_H
#define UGD_TIMER_H

class  Timer {
  public:
    void update (size_t samples);
    size_t time const ();
    static Timer* instance();
    /// Timer *timer = Timer::instance();
    ~Timer();
  private:
    size_t time_;
    static Timer *intance_;
    Timer();
}
#endif
