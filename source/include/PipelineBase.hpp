#ifndef PIPELINE_BASE_H_
#define PIPELINE_BASE_H_

#include <memory>
#include "GpuHelper.hpp"

class PipelineBase
{
protected:
    static std::shared_ptr<FrameBuffer> frame_buffer_;
    static uint32_t *pixels_;
    static float *z_buffer_;
    static int view_port_x_, view_port_y_;
    static uint32_t view_port_width_, view_port_height_;
};

#endif