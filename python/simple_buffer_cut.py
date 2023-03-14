import sys
# from ctypes import *
import struct
import ctypes
from libvad import ffibuilder as _vad

NEW_LINE = 50

def Array2Bytes():
    data = [1,2,3]
    packed_data = struct.pack('<3h', *data)

def SilenceDel(file):
    vader = _vad.dlopen("build_ninja/libvad.so")
    vader.webrtc_vad_create()

    vadHandle = vader.ffi_webrtc_vad_create()
    status = vader.ffi_webrtc_vad_init(vadHandle)
    if status != 0:
        print('vad init failed', status)
        return -1
    status = vader.ffi_webrtc_vad_set_mode(vadHandle, 3)
    if status != 0:
        print('vad init failed', status)
        return -1
    inx = 0
    fwp = open("result.text", mode="w")
    with open(file, mode="rb") as fp:
        while True:
            data = fp.read(160*2)
            if data:
                # pyobject
                # LP_c_int16 = ctypes.POINTER(ctypes.c_int16)
                # c_data = ctypes.cast(data, ctypes.POINTER(ctypes.c_int16))
                # ffi array
                # c_data = _vad.new("int16_t[]", data)
                # ffi bytes is error
                # c_data = _vad.cast("int16_t*", data)
                c_data = _vad.from_buffer("int16_t*", data)
                status = vader.ffi_webrtc_vad_process(vadHandle, 16000, c_data, len(data)//2)
                # if status == -1:
                    # print('silence process failed', status)
                # elif status == 1:
                    # print('silence process ok active', status)
                # else:
                    # print('silence process not active', status)
                inx+=1
                if (inx % NEW_LINE == 0):
                    print('%d index:%d' % (status, inx));
                    fwp.write(str(status) + '\n')
                else:
                    print('%d index:%d' % (status, inx), end=' ');
                    fwp.write(str(status) + ' ')
            else:
                print('\nbuf check ok')
                break;
    vader.webrtc_vad_destroy()        

if __name__=='__main__':
    file = sys.argv[1]
    SilenceDel(file)