#include "fisherfaces.h"
#include <iostream>
#include <algorithm>
#include "eigenfaces.h"

FisherFaces::FisherFaces(const FaceCatalogue &fc, const wxPropertyGridInterface& props) :
        Projector(fc, props),
        m_width(fc.get_face(0).get_width()),
        m_height(fc.get_face(0).get_height())
{
    update_props(props);
    m_preprojector = new EigenFaces(fc, props);
    MatrixXf Sb = construct_Sb_matrix();
    MatrixXf Sw = construct_Sw_matrix();

    MatrixXf L = Sw.inverse() * Sb;
    EigenSolver<MatrixXf> es(L);

    MatrixXf eigenvectors = es.eigenvectors().real().cast<float>();
    VectorXf eigenvalues = es.eigenvalues().real().cast<float>();

    m_fisherfaces = MatrixXf(m_fc.get_face(0).get_width() * m_fc.get_face(0).get_height(),
                             eigenvectors.cols());

    for (int l = 0; l < eigenvectors.cols(); l++)
    {
        const MatrixXf& full_preproj_mat = m_preprojector->get_projection_matrix();
        MatrixXf preproj_mat = full_preproj_mat.block(0, 0, full_preproj_mat.rows(), m_preproj_dims);
        printf("preproj mat size = (%ld x %ld)\n", preproj_mat.rows(), preproj_mat.cols());
        printf("vector mat size = (%ld x %ld)\n", eigenvectors.col(l).rows(), eigenvectors.col(l).cols());
        VectorXf u = preproj_mat * eigenvectors.col(l);
        u.normalize();
        printf("u size = (%ld x %ld)\n", u.rows(), u.cols());
        m_fisherfaces.col(l) << u.transpose();
        set_info(l, "Index", wxString::Format("%u", l));
        set_info(l, "Eigenvalue", wxString::Format("%f", eigenvalues(l)));
    }
    printf("Generated fisherfaces\n");
}

FisherFaces::~FisherFaces()
{
    delete m_preprojector;
}

bool FisherFaces::update_props(const wxPropertyGridInterface& props)
{
    m_preproj_dims = props.GetPropertyByName(wxT("propPreProjDims"))->GetValue().GetLong();
    printf("set preproj_dims = %d\n", m_preproj_dims);
    return true;
}

MatrixXf FisherFaces::get_average_projected() const
{
    MatrixXf sum = MatrixXf::Zero(m_preproj_dims, 1);
    int n = 0;
    for (uint32_t i = 0; i < m_fc.get_num_classes(); i++)
    {
        for (auto s : m_fc.get_set_of_class(i, FaceCatalogue::TRAINING_SET))
        {
            sum += m_preprojector->project(*s, m_preproj_dims);
            n++;
        }
    }
    return sum / n;
}

MatrixXf FisherFaces::get_average_projected(uint32_t cls) const
{
    MatrixXf sum = MatrixXf::Zero(m_preproj_dims, 1);
    int n = 0;
    for (auto s : m_fc.get_set_of_class(cls, FaceCatalogue::TRAINING_SET))
    {
        sum += m_preprojector->project(*s, m_preproj_dims);
        n++;
    }
    return sum / n;
}

MatrixXf FisherFaces::construct_Sb_matrix()
{
    uint32_t num_training_faces = 0;
    MatrixXf Sb = MatrixXf::Zero(m_preproj_dims, m_preproj_dims);

    for (uint32_t j = 0; j < m_fc.get_num_classes(); j++)
    {
        num_training_faces += m_fc.get_set_of_class(j, FaceCatalogue::TRAINING_SET).size();
        VectorXf class_average_face = get_average_projected(j);
        class_average_face -= get_average_projected();
        Sb += class_average_face * class_average_face.transpose();
    }

    return Sb;
}

MatrixXf FisherFaces::construct_Sw_matrix()
{
    uint32_t num_training_faces = 0;
    MatrixXf Sw = MatrixXf::Zero(m_preproj_dims, m_preproj_dims);

    for (uint32_t j = 0; j < m_fc.get_num_classes(); j++)
    {
        auto class_faces = m_fc.get_set_of_class(j, FaceCatalogue::TRAINING_SET);
        VectorXf class_average_face = get_average_projected(j);

        for (auto face : class_faces)
        {
            VectorXf norm_face = m_preprojector->project(*face) - class_average_face;;
            Sw += norm_face * norm_face.transpose();
        }
    }

    return Sw;
}

FaceImage FisherFaces::get_face(uint32_t index) const
{
    return FaceImage(m_eigenfaces.col(index), m_height, m_width, index);
}

VectorXf FisherFaces::project(const FaceImage& im, uint32_t dimensionality)
{
    auto transform_mat = m_fisherfaces.block(0, 0, m_fisherfaces.rows(), dimensionality);
    VectorXf ret = transform_mat.transpose() * (im.to_vector() - m_average_face);
    return ret;
}

std::vector<std::shared_ptr<wxPGProperty> > FisherFaces::get_props()
{
    auto props = Projector::get_props();

    props.push_back(std::make_shared<wxIntProperty>(wxT("PCA preproj dims"), wxT("propPreProjDims")));
    props.back()->SetValue(10);
    return props;
}

REGISTER_PROJECTOR(FisherFaces)
