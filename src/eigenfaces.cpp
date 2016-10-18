#include "eigenfaces.h"
#include <iostream>
#include <algorithm>

EigenFaces::EigenFaces(const FaceCatalogue &fc, const wxPropertyGridInterface& props) :
        Projector(fc, props),
        m_width(fc.get_face(0).get_width()),
        m_height(fc.get_face(0).get_height())
{
    MatrixXf A = construct_A_matrix();
    m_eigenfaces = MatrixXf(m_fc.get_face(0).get_width() * m_fc.get_face(0).get_height(),
                            A.cols());

    MatrixXf L = A.transpose() * A;
    EigenSolver<MatrixXf> es(L);

    MatrixXf eigenvectors = es.eigenvectors().real().cast<float>();
    VectorXf eigenvalues = es.eigenvalues().real().cast<float>();

    sort_eigenvectors(eigenvectors, eigenvalues);
    printf("Generated eigenvecs\n");

    for (int l = 0; l < eigenvectors.cols(); l++)
    {
        MatrixXf u = MatrixXf::Zero(m_fc.get_face(0).to_vector().size(), 1);
        for (int k = 0; k < eigenvectors.cols(); k++)
        {
            u += eigenvectors(k, l) * m_fc.get_face(k).to_vector();
        }
        m_eigenfaces.col(l) << u;
        set_info(l, "Index", wxString::Format("%u", l));
        set_info(l, "Eigenvalue", wxString::Format("%f", eigenvalues(l)));
    }
    printf("Generated eigenfaces\n");
}

EigenFaces::~EigenFaces()
{
}

void EigenFaces::sort_eigenvectors(MatrixXf& eigenvectors, VectorXf& eigenvalues)
{
    std::vector<uint32_t> idx(eigenvalues.size());
    std::iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(), [&eigenvalues](uint32_t i1, uint32_t i2) {
            return eigenvalues[i1] > eigenvalues[i2]; });

    MatrixXf evec_copy(eigenvectors);
    VectorXf eval_copy(eigenvalues);
    int j = 0;
    for (auto i : idx)
    {
        eigenvalues[j] = eval_copy[i];
        eigenvectors.col(i) = evec_copy.col(j);
        j++;
    }
}

MatrixXf EigenFaces::construct_A_matrix()
{
    // First construct A
    uint32_t num_training_faces = 0;
    for (uint32_t i = 0; i < m_fc.get_num_classes(); i++)
    {
        num_training_faces += m_fc.get_set_of_class(i, FaceCatalogue::TRAINING_SET).size();
    }
    MatrixXf A = MatrixXf(m_fc.get_face(0).get_width() * m_fc.get_face(0).get_height(), num_training_faces);

    get_average_face();
    int icol = 0;
    for (uint32_t i = 0; i < m_fc.get_num_classes(); i++)
    {
        for (auto s : m_fc.get_set_of_class(i, FaceCatalogue::TRAINING_SET))
        {
            A.col(icol++) << s->to_vector() - m_average_face;
        }
    }
    return A;
}

void EigenFaces::get_average_face()
{
    MatrixXf sum = MatrixXf::Zero(m_fc.get_face(0).get_width() * m_fc.get_face(0).get_height(), 1);
    int n = 0;
    for (uint32_t i = 0; i < m_fc.get_num_classes(); i++)
    {
        for (auto s : m_fc.get_set_of_class(i, FaceCatalogue::TRAINING_SET))
        {
            sum += s->to_vector();
            n++;
        }
    }
    m_average_face = sum / n;
}

FaceImage EigenFaces::get_face(uint32_t index) const
{
    return FaceImage(m_eigenfaces.col(index), m_height, m_width, index);
}

VectorXf EigenFaces::project(const FaceImage& im, uint32_t dimensionality)
{
    auto transform_mat = m_eigenfaces.block(0, 0, m_eigenfaces.rows(), dimensionality);
    VectorXf ret = transform_mat.transpose() * (im.to_vector() - m_average_face);
    ret = ret - VectorXf::Ones(ret.size()) * ret.minCoeff();
    return ret / ret.maxCoeff();
}

std::vector<std::shared_ptr<wxPGProperty> > EigenFaces::get_props()
{
    return Projector::get_props();
}

REGISTER_PROJECTOR(EigenFaces)
