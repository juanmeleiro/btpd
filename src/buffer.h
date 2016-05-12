
#ifndef UGD_BUFFER_H
#define UGD_BUFFER_H

class Buffer {
  public:
    double getValue(size_t address);
    double setValue(size_t address, double value);
  protected:
    Buffer(size_t size);
    virtual ~Buffer();
  private:
    double *buf_;
};

#endif
