#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WEBRTC_WIN) || defined(WEBRTC_POSIX)
    //webrtc
    #include "common_audio/vad/include/webrtc_vad.h"
#endif 

#ifdef _WITH_DEBUG
FILE *fpc = NULL;
#endif

void webrtc_vad_create(){
    printf("vad create");
#ifdef _WITH_DEBUG
    fpc = fopen("fp_out.pcm", "wb+");
#endif
}

void webrtc_vad_destroy(){
#ifdef _WITH_DEBUG
    if(fpc != NULL){
        fclose(fpc);
        fpc = NULL;
    }
#endif
    printf("vad destroy");
}

VadInst* ffi_webrtc_vad_create(){
    return WebRtcVad_Create();
}


int ffi_webrtc_vad_init(VadInst *hd){
    return WebRtcVad_Init(hd);
}

int ffi_webrtc_vad_set_mode(VadInst *hd, int mode)
{
    return WebRtcVad_set_mode(hd, mode);
}

int ffi_webrtc_vad_process(VadInst *hd, int sampleRate, const int16_t *bf, int len){
#ifdef _WITH_DEBUG
    if(fpc){
        fwrite(bf, 1, len, fpc);
        fflush(fpc);
    }
#endif
    int ret = WebRtcVad_Process(hd, sampleRate, bf, len);
#ifdef _WITH_DEBUG
    printf("sample rate:%d  data len:%d  ret:%d\n", sampleRate, len, ret);
#endif
    return ret;
}