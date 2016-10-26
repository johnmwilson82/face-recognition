#pragma once

#include "img_mgr.h"
#include "projector.h"
#include <eigen3/Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::EigenSolver;

class FisherFaces : public FaceObservable, public Projector
{
private:
    MatrixXf m_A, m_fisherfaces;
    uint32_t m_width, m_height, m_preproj_dims;

    MatrixXf construct_Sb_matrix();
    MatrixXf construct_Sw_matrix();
    Projector* m_preprojector;

    MatrixXf get_average_projected() const;
    MatrixXf get_average_projected(uint32_t cls) const;

public:
    FisherFaces(const FaceCatalogue &fc, const wxPropertyGridInterface& props);
    virtual ~FisherFaces();

    virtual FaceImage get_face(uint32_t index) const;
    virtual uint32_t get_num_faces() const { return m_fisherfaces.cols(); }

    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality);

    static const wxString get_name() { return "Fisherfaces"; }
    static std::vector<std::shared_ptr<wxPGProperty> > get_props();
    virtual bool update_props(const wxPropertyGridInterface& props);
    
    virtual const MatrixXf& get_projection_matrix() const { return m_fisherfaces; }
};
