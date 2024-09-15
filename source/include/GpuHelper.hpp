#ifndef GPU_HELPER_H_
#define GPU_HELPER_H_

#include <iostream>
//#define RGBA_COLOR(R, G, B, A) ((R) | (G << 8) | (B << 16) | (A << 24))
#define GPULAYER_ASSERT(cond, msg)     \
    if (cond)                          \
    {                                  \
    }                                  \
    else                               \
    {                                  \
        std::cout << msg << std::endl; \
        __debugbreak();                \
    }

class GpuLayer
{
public:
    static void Init(void *NativeWindow);
};

class FrameBuffer
{
public:
    FrameBuffer(uint32_t Width, uint32_t Height);
    virtual ~FrameBuffer();

    void Resize(uint32_t Width, uint32_t Height);
    void Present(void *Pixels);

private:
    void Invalidate(uint32_t Width, uint32_t Height);
    void UpdatePixels(void *Pixels);
    void Bind(int slot = 0) const;
    void Unbind() const;

private:
    uint32_t textureID;
    uint32_t m_Width;
    uint32_t m_Height;
};

#endif