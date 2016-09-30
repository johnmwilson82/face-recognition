#pragma once

#include <eigen3/Eigen/Dense>
#include <string>
#include <list>
#include <memory>
#include <cstdio>
#include <wx/string.h>
#include <map>

using Eigen::MatrixXf;
using Eigen::VectorXf;

class FaceImage {
private:
    MatrixXf m_data_mat;
    uint32_t m_width, m_height, m_maxval;

    void load_from_pixel_data(const uint8_t* buf);

public:
    FaceImage(const std::string &path);
    FaceImage(const MatrixXf& data_mat, uint32_t height, uint32_t width);
    std::unique_ptr<uint8_t[]> to_rgb_buffer() const;
    VectorXf to_vector() const;

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }
};

class FaceObservable {
protected:
    std::map<const FaceImage*, std::map<wxString, wxString> > m_info;

public:
    virtual const std::string get_name() const = 0;
    virtual FaceImage get_face(uint32_t index) const = 0;
    virtual uint32_t get_num_faces() const = 0;

    wxString get_info(const FaceImage* im);
    void set_info(const FaceImage* im, const wxString &category, const wxString &info);
};


class FaceCatalogue : public FaceObservable{
private:
    std::vector<FaceImage*> m_face_images;
    std::vector<std::string> m_names_map;

    std::vector<uint32_t> m_class_lookup;
    std::vector<std::list<uint32_t> > m_class_members;
    std::vector<std::list<uint32_t> > m_training_sets;
    std::vector<std::list<uint32_t> > m_test_sets;

public:
    enum SetType { TRAINING_SET, TEST_SET, ALL_SET };
    FaceCatalogue(const std::string &dir);
    ~FaceCatalogue();

    void choose_training_sets(float proportion, uint32_t seed = (uint32_t) NULL);
    std::vector<FaceImage*> get_set_of_class(uint32_t class_id, SetType type=SetType::ALL_SET) const;
    uint32_t get_num_classes() const { return m_class_members.size(); }

    virtual FaceImage get_face(uint32_t index) const { return *(m_face_images[index]); }
    virtual uint32_t get_num_faces() const { return m_face_images.size(); }
    virtual const std::string get_name() const { return std::string("Face catalogue"); }
};
