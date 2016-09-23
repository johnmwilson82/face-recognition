#pragma once

#include "img_mgr.h"
#include <eigen3/Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::EigenSolver;

class EigenFaces
{
private:
    MatrixXf m_A;
    const FaceCatalogue &m_fc;
    uint32_t m_width, m_height;

    std::vector<FaceImage*> m_eigenfaces;

    MatrixXf construct_A_matrix();
public:
    EigenFaces(const FaceCatalogue &fc);
    ~EigenFaces();

    const FaceImage& get_face(uint32_t index) const { return *(m_eigenfaces[index]); }
    uint32_t get_num_faces() const { return m_eigenfaces.size(); }
};
