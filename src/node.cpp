#include <node.h>

using std::string;

Node::Node(string name) :name_(name) {}

Buffer Node::evaluate (size_t samples) {
    Buffer buf = Buffer(samples)
    for (size_t s = 0; s < samples; s++) {
        Timer* t = Timer::instance();
        buf.setSample(s, process_function_(t.time() + s))
    }
    return buf;
}

string Node::name () const {
    return name_;
}
