#pragma once

#include "img_mgr.h"
class EigenFaces
{
private:
    const FaceCatalogue &fc;
    std::vector<FaceImage*> m_eigenfaces;

public:
    EigenFaces(const FaceCatalogue &fc);

};
