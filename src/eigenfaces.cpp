#include "eigenfaces.h"
#include <iostream>

EigenFaces::EigenFaces(const FaceCatalogue &fc) :
        m_fc(fc),
        m_width(fc.get_face(0).get_width()),
        m_height(fc.get_face(0).get_height())
{
    MatrixXf A = construct_A_matrix();
    MatrixXf L = A.transpose() * A;
    EigenSolver<MatrixXf> es(L);
    printf("Generated eigenvecs\n");
    MatrixXf eigenvectors = es.eigenvectors().real().cast<float>();
    for (int l = 0; l < eigenvectors.cols(); l++)
    {
        MatrixXf u = MatrixXf::Zero(m_fc.get_face(0).to_vector().size(), 1);
        for (int k = 0; k < eigenvectors.cols(); k++)
        {
            u += eigenvectors(k, l) * m_fc.get_face(k).to_vector();
        }
        m_eigenfaces.push_back(new FaceImage(u, m_height, m_width));
    }
}

EigenFaces::~EigenFaces()
{
    for (auto ef : m_eigenfaces)
    {
        delete(ef);
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

    int icol = 0;
    for (uint32_t i = 0; i < m_fc.get_num_classes(); i++)
    {
        for (auto s : m_fc.get_set_of_class(i, FaceCatalogue::TRAINING_SET))
        {
            A.col(icol++) << s->to_vector();
        }
    }
    return A;
}
