#pragma once
#include "img_mgr.h"

class Projector
{
protected:
    const FaceCatalogue &m_fc;

public:
    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality) = 0;
    Projector(const FaceCatalogue &fc) : m_fc(fc) {}
};
