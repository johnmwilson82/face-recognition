#pragma once

#include "img_mgr.h"
#include <eigen3/Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::EigenSolver;

class EigenFaces : public FaceObservable
{
private:
    MatrixXf m_A, m_average_face, m_eigenfaces;
    const FaceCatalogue &m_fc;
    uint32_t m_width, m_height;

    MatrixXf construct_A_matrix();
    void get_average_face();

public:
    EigenFaces(const FaceCatalogue &fc);
    ~EigenFaces();

    virtual FaceImage get_face(uint32_t index) const;
    virtual uint32_t get_num_faces() const { return m_eigenfaces.size(); }
    virtual const std::string get_name() const { return std::string("Eigenfaces"); }

    VectorXf project(const FaceImage& im, uint32_t dimensionality);
};
