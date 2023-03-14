#include <stdio.h>
#include <stdlib.h>
//webrtc
#include "common_audio/vad/include/webrtc_vad.h"

#define MAX_FRAME_SIZE 160
#define NEW_LINE 30
#define SILENCE_STEP 50

int help(char *argv[])
{
    printf("please input a pcm file like:%s a.pcm samplerate framesize silencestep\n", argv[1]);
    return -1;
}

int main(int argc, char* argv[])
{
    if(argc < 2){
        return help(argv);
    }
    int frameSize = MAX_FRAME_SIZE;
    int silenceStep = SILENCE_STEP;
    short sampleRate = 16000;

    if(argc == 3) sampleRate = atoi(argv[2]);
    if(argc == 4) frameSize = atoi(argv[3]);
    if(argc == 5) silenceStep = atoi(argv[4]);
    FILE *fpin = NULL, *fpout = NULL, *fpbf = NULL;
    VadInst *handle = NULL;
    
    short *frame = (short*)malloc(frameSize*sizeof(short));
    int status = 0;

    handle = WebRtcVad_Create();
    if(handle == NULL){
        printf("webrtc vad create error%d\n", status);
        return status;
    }

    status = WebRtcVad_Init(handle);
    if(status){
        printf("webrtc vad init error:%d\n", status);
        return status;
    }

    status = WebRtcVad_set_mode(handle, 3);
    if(status){
        printf("webrtc vad mode set error:%d\n", status);
        return status;
    }

    fpin = fopen(argv[1], "rb");
    fpout = fopen("result.txt", "w");
    fpbf = fopen("result.pcm", "wb");
    int inx = 0;
    int cursen = 0, presen = 0;
    int step = 0;
    
    FILE *fpelber = NULL;

    if(fpin != NULL){
        while(!feof(fpin)){
            int len = fread(frame, sizeof(short), frameSize, fpin);
            if(len > 0){
                inx++;
                printf("index:%d len:%d sizeof(short):%ld\n", inx, len, sizeof(short));
                status = WebRtcVad_Process(handle, sampleRate, frame, len);
                if(status == -1){
                    printf("webrtc vad process is error:%d\n", status);
                    printf("frames:%d\n", inx);
                    return status;
                }else if(status == 1){
                    if(step > 50 || cursen == 0) {
                        if(fpelber != NULL){
                            fclose(fpelber);
                            fpelber = NULL;
                        }
                        char sentense[24];
                        sprintf(sentense, "sentence_%d.pcm", cursen);
                        fpelber = fopen(sentense, "wb");
                        printf("new sentence:%d\n", cursen++);
                    }

                    step = 0;
                    fwrite(frame, sizeof(short), len, fpbf);
                    fwrite(frame, sizeof(short), len, fpelber);
                }else{
                    step++;
                }
                if(inx % NEW_LINE == 0)fprintf(fpout, "%02d\n", status);
                else fprintf(fpout, "%02d ", status);
            }
        }
    }
    free(frame);
    frame = NULL;

    printf("frames:%d\n", inx);
    if(fpin != NULL)fclose(fpin);
    fclose(fpout);
    fclose(fpbf);
    WebRtcVad_Free(handle);
    return 0;
}