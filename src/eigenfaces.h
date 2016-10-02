#pragma once

#include "img_mgr.h"
#include "projector.h"
#include <eigen3/Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::EigenSolver;

class EigenFaces : public FaceObservable, public Projector
{
private:
    MatrixXf m_A, m_average_face, m_eigenfaces;
    uint32_t m_width, m_height;

    MatrixXf construct_A_matrix();
    void sort_eigenvectors(MatrixXf& eigenvectors, VectorXf& eigenvalues);
    void get_average_face();

public:
    EigenFaces(const FaceCatalogue &fc);
    ~EigenFaces();

    virtual FaceImage get_face(uint32_t index) const;
    virtual uint32_t get_num_faces() const { return m_eigenfaces.cols(); }
    virtual const std::string get_name() const { return std::string("Eigenfaces"); }

    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality);
};
