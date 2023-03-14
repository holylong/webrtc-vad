from cffi import FFI

# 创建一个ffi对象
ffi = FFI()

# 定义一个C语言的函数
ffi.cdef("""
    void my_c_function(int16_t* data, int length);
""")

# 加载动态库
lib = ffi.dlopen('./libmy_c_library.so')

# 读取文件内容
with open('../data/feier_16000_s16le_1.pcm', 'rb') as f:
    data = f.read(160*2)
    if data:
        # 将数据转换为 C 语言的 int16_t* 类型指针
        # c_data = ffi.cast("int16_t*", data)
        c_data = ffi.from_buffer("int16_t*", data)
        # 调用 C 语言的函数
        lib.my_c_function(c_data, len(data) // 2)
    
    f.close()
