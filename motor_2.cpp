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
#include <string>

using std::transform;
using std::vector;

#define PI 3.1415926535
#define SAMPLERATE 44100

class Sine {
    public:
        double time = 0;

        void generate(double frequency, vector<double> *buffer, double phase)
        {
            for (int i = 0; i < buffer->size(); i++)
            {
               (*buffer)[i] = sin(2*PI*frequency*(this->time + i/(double)SAMPLERATE) - phase);
            }
            time += buffer->size()/SAMPLERATE;  
        }

        void generate(double freq, vector<short> *buffer, double phase)
        {
            vector<double> d_buffer((*buffer).size());
            generate(freq, &d_buffer, phase);
            transform(d_buffer.begin(), d_buffer.end(), (*buffer).begin(), [](double x){
                return static_cast<short>(x * (1<<16 - 1)/2);
            });
        }
};

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

int main(int args, char* argv[])
{
    Sine s;

    /* if user dosen't enter an argument, default */
    float freq = 440.0;
    float dur = 3.0;
    float ratio = 1;

    /* else assign these values */
    if (args >= 2) {
        freq = static_cast<float> (atof(argv[1]));
        if(args >= 3)  {
            ratio = static_cast<float> (atof(argv[2]));
            if(args >= 4) dur = static_cast<float> (atof(argv[3]));
       }
    }

    printf("frequency: %f\n", freq);
    printf("duration: %f\n", dur);

    vector<short>::size_type size = SAMPLERATE*dur;
   
    printf("%d\n", static_cast<int>(size) );

    vector<short> buf1(size);
    vector<short> buf2(size);
    vector<short> buf(size);
   
    s.generate(freq, &buf1, 0);
    s.generate(freq*ratio, &buf2, 1/(2*freq));

    for(size_t i=0; i < size; i++) {
        buf[i] =  buf1[i] +  buf2[i];
     } 
    
    initialize();
    checkError();
    
    ALuint buffer;
    alGenBuffers(1, &buffer);

    alBufferData(buffer, AL_FORMAT_MONO16, buf.data(), buf.size()*sizeof(short), SAMPLERATE);
    checkError();

    ALuint source;
    alGenSources(1, &source);
    checkError();

    alSourcei(source, AL_BUFFER, buffer);
    checkError();

    alSourcePlay(source);
    checkError();

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int> (dur*1000)));

    checkError();
    exit();
    return(EXIT_SUCCESS);
}
