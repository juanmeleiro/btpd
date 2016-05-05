
#ifndef UGD_NODE_H
#define UGD_NODE_H

#include <functional>
#include <string>

class Node {
  public:
    virtual Buffer evaluate(size_t samples) = 0;
    std::string name() const;
  protected:
    Node(std::string name);
    virtual ~Node();
  private:
    std::string name_;
};


#endif
