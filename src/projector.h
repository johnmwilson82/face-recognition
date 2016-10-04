#pragma once
#include "img_mgr.h"

class Projector
{
protected:
    const FaceCatalogue &m_fc;
    uint32_t m_noutput_dims;

public:
    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality) = 0;
    virtual VectorXf project(const FaceImage& im) { return project(im, m_noutput_dims); }
    void set_noutput_dims(uint32_t noutput_dims) { printf("Set noutput dims = %u\n", noutput_dims); m_noutput_dims = noutput_dims; }
    uint32_t get_noutput_dims() { return m_noutput_dims; }
    Projector(const FaceCatalogue &fc) : m_fc(fc) {}
};
