#include <math.h>
using std::vector;
#define PI 3.14159265

class Sine
{
        public:
                unsigned sampleRate = 44100;

                void generate(double time, double frequency, vector<double> *buffer)
                {
                        for (int i = 0; i < buffer->size(); i++) {
                                (*buffer)[i] = sin(2*PI*frequency*(time + i/(double)this->sampleRate));
                        }

                }
};

class Triangle
{
        public:
                unsigned sampleRate = 44100;

                void generate(double time, double frequency, vector<double> *buffer)
                {
                        for (int i = 0; i < buffer->size(); i++)
                                (*buffer)[i] = fmod((float)i*frequency/this->sampleRate, 1);
                }

};

class Square
{
        public:
                unsigned sampleRate = 44100;
                void generate(double time, double frequency, vector<double> *buffer)
                {
                        for (int i = 0; i < buffer->size(); i++)
                                (*buffer)[i] = (fmod(i, sampleRate/frequency) > sampleRate/(2*frequency) ? 1 : 0);
                }
};
