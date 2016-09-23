#pragma once

#include <eigen3/Eigen/Dense>
#include <string>
#include <list>
#include <memory>
#include <cstdio>

using Eigen::MatrixXd;

class FaceImage {
private:
    MatrixXd m_data_mat;
    uint32_t m_class_id;
    uint32_t m_width, m_height, m_maxval;

    void load_from_pixel_data(const uint8_t* buf);

public:
    FaceImage(const std::string &path, uint32_t class_id);
    std::unique_ptr<uint8_t[]> to_rgb_buffer() const;
    MatrixXd to_vector() const;

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }
    uint32_t get_class() const { return m_class_id; }
};


class FaceCatalogue {
private:
    std::vector<FaceImage*> m_face_images;
    std::vector<std::string> m_names_map;

    std::vector<std::list<uint32_t> > m_class_members;
    std::vector<std::list<uint32_t> > m_training_sets;

public:
    FaceCatalogue(const std::string &dir);
    ~FaceCatalogue();
    const FaceImage& get_face(uint32_t index) const { return *(m_face_images[index]); }

    uint32_t get_num_faces() const { return m_face_images.size(); }

    void choose_training_sets(float proportion, uint32_t seed = NULL);
    std::vector<FaceImage*> get_set_of_class(uint32_t class_id);
};
