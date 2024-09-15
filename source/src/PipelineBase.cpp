#include "PipelineBase.hpp"
std::shared_ptr<FrameBuffer> PipelineBase::frame_buffer_ = nullptr;
uint32_t *PipelineBase::pixels_ = nullptr;
float *PipelineBase::z_buffer_ = nullptr;
int PipelineBase::view_port_x_ = 0;
int PipelineBase::view_port_y_ = 0;
uint32_t PipelineBase::view_port_width_ = 0;
uint32_t PipelineBase::view_port_height_ = 0;