import os
import sys
from cffi import FFI

ffibuilder = FFI()
ffibuilder.cdef("""
/* typedef struct VadInst VasNst ; */

typedef struct VadInst VadInst ;

typedef struct BufferVadInst{
    VadInst *handler;
    FILE    *fpc;
}BufferVadInst;

void        webrtc_vad_create(void) ;
void        webrtc_vad_destroy(void) ;
VadInst*    ffi_webrtc_vad_create() ;
int         ffi_webrtc_vad_init(VadInst *hd) ;
int         ffi_webrtc_vad_set_mode(VadInst *hd, int mode) ;
int         ffi_webrtc_vad_process(VadInst *hd, int sampleRate, const int16_t *bf, int len) ;

VadInst*        WebRtcVad_Create(void) ;

int        WebRtcVad_Init(VadInst *hd) ;

int        WebRtcVad_set_mode(VadInst *hd, int type) ;

int        WebRtcVad_Process(VadInst *hd, int sampleRate, const int16_t *bf, int len) ;

""")

# lib = ffibuilder.dlopen("build_ninja/libvad.so")
