#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H

class MAudioPlay
{
public:
    static MAudioPlay *get();
     bool start();
     void stop();
     void play(bool isplay);
     bool write(const char *data, int datasize);
     int getFree();

     ~MAudioPlay();

private:

    XAudioPlay();
};

#endif