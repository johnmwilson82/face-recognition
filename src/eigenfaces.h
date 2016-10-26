#pragma once

#include "img_mgr.h"
#include "projector.h"
#include <eigen3/Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::EigenSolver;

class FisherFaces;

class EigenFaces : public FaceObservable, public Projector
{
    friend FisherFaces;

private:
    MatrixXf m_A, m_average_face, m_eigenfaces;
    uint32_t m_width, m_height;

    MatrixXf construct_A_matrix();
    void sort_eigenvectors(MatrixXf& eigenvectors, VectorXf& eigenvalues);

public:
    EigenFaces(const FaceCatalogue &fc, const wxPropertyGridInterface& props);
    virtual ~EigenFaces();

    virtual FaceImage get_face(uint32_t index) const;
    virtual uint32_t get_num_faces() const { return m_eigenfaces.cols(); }

    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality);

    static const wxString get_name() { return "Eigenfaces"; }
    static std::vector<std::shared_ptr<wxPGProperty> > get_props();
    
    virtual const MatrixXf& get_projection_matrix() const { return m_eigenfaces; }
};
