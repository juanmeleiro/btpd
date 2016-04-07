/* Motor de audio e interpretador de arquivo btpd - v0 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <AL/al.h>
#include <AL/alc.h>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

using std::transform;
using std::vector;

#define PI 3.1415926535

class Sine {
    public:
        double time = 0;
        unsigned sampleRate = 44100;

        void generate(double frequency, vector<double> *buffer)
        {
            for (int i = 0; i < buffer->size(); i++)
            {
               (*buffer)[i] = sin(2*PI*frequency*(this->time + i/(double)this->sampleRate));
            }
            time += buffer->size()/this->sampleRate;    
        }
};


/* Bureuacracy */

void initialize() {
    ALCdevice *device = NULL;
    ALCcontext *context = NULL;
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
}

void exit() {
    ALCdevice *device = NULL;
    ALCcontext *context = NULL;
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

const char* errorString(ALenum error) {
    switch(error) {
        case (AL_NO_ERROR):
            return "AL_NO_ERROR";
        case (AL_INVALID_NAME):
            return "AL_INVALID_NAME";
        case (AL_INVALID_ENUM):
            return "AL_INVALID_NAME";
        case (AL_INVALID_VALUE):
            return "AL_INVALID_VALUE";
        case (AL_INVALID_OPERATION):
            return "AL_INVALID_OPERATION";
        case (AL_OUT_OF_MEMORY):
            return "AL_OUT_OF_MEMORY";
    }
        return "UNKNOWN";
}
void checkErrorFunction(const char* file, int line) {
    ALenum error = alGetError();
    while (error != AL_NO_ERROR) {
        printf("AL Error %s at %s: %d\n", errorString(error), file, line);
        error = alGetError();
    }
}

#define checkError() checkErrorFunction(__FILE__, __LINE__)
/* EOB (End of bureuacracy) */

void sine(double frequency, double duration, int sampleRate, double* buffer)
{}
int main()
{
    Sine s;
    vector<double> buf(44100*5);
    s.generate(440.0, &buf);
   
    vector<short> bufShort(44100*5);

    transform(buf.begin(), buf.end(), bufShort.begin(), [](double x){
        return static_cast<short>(x * (1<<16 - 1));
    });

//    for (int i = 0; i <= 100; i++) printf("%d\n", bufShort[3000+i]);	

    initialize();
    checkError();
    
    ALuint buffer;
    alGenBuffers(1, &buffer);

    alBufferData(buffer, AL_FORMAT_MONO16, bufShort.data(), bufShort.size(), 44100);
    checkError();

    ALuint source;
    alGenSources(1, &source);
    checkError();

    alSourcei(source, AL_BUFFER, buffer);
    checkError();

    alSourcePlay(source);
    checkError();

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    checkError();
    exit();
    return(EXIT_SUCCESS);
}
