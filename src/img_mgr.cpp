#include <fstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "img_mgr.h"
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <dirent.h>
#include <iostream>

FaceImage::FaceImage(const MatrixXf &data_mat, uint32_t height, uint32_t width) :
    m_data_mat(data_mat),
    m_height(height),
    m_width(width)
{
    m_data_mat.resize(width, height);
    float min = m_data_mat.minCoeff();
    m_data_mat -= min * MatrixXf::Ones(width, height);
    float max = m_data_mat.maxCoeff();
    m_maxval = 255 / max;
}

FaceImage::FaceImage(const std::string &path)
{
    std::ifstream f(path.c_str(), std::ios_base::binary);

    if (!f) {
        throw std::runtime_error("Couldn't open image file");
    }

    char header[3];
    f.get(header, 3, ' ');

    if(std::strcmp(header, "P5") != 0)
    {
        throw std::runtime_error("Invalid file type");
    }

    f >> m_width >> m_height >> m_maxval;

    uint32_t bpp = (int) std::ceil(std::log2(1.0f * m_maxval) / 8.0);
    if(bpp != 1)
    {
        throw std::runtime_error("Doesn't support files with bytes per pixel != 1");
    }

    char buf[m_width * m_height];
    f.read(buf, m_width * m_height);
    load_from_pixel_data((const uint8_t*) buf);

}

void FaceImage::load_from_pixel_data(const uint8_t* buf)
{
    m_data_mat = MatrixXf(m_width, m_height);

    for (uint32_t j = 0; j < m_height; j++) {
        for (uint32_t i = 0; i < m_width; i++) {
            m_data_mat(i, j) = *(buf++) * 1.0f / m_maxval;
        }
    }
}

VectorXf FaceImage::to_vector() const
{
    MatrixXf ret(m_data_mat);
    ret.resize(m_width * m_height, 1);
    return VectorXf(ret);
}

std::unique_ptr<uint8_t[]> FaceImage::to_rgb_buffer() const
{
    int sz = 0;
    auto buf = std::unique_ptr<uint8_t[]> (new uint8_t[m_height * m_width * 3]);
    for (uint32_t j = 0; j < m_height; j++) {
        for (uint32_t i = 0; i < m_width; i++) {
            buf[sz] = (uint8_t)(m_data_mat(i, j) * m_maxval);
            buf[sz+1] = (uint8_t)(m_data_mat(i, j) * m_maxval);
            buf[sz+2] = (uint8_t)(m_data_mat(i, j) * m_maxval);
            sz += 3;
        }
    }
    return buf;
}

FaceCatalogue::FaceCatalogue(const std::string &path)
{
    DIR *dir, *dir2;
    struct dirent *ent, *ent2;
    char buf1[1024], buf2[1024];
    uint32_t i = 0;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            sprintf(buf1, "%s/%s", path.c_str(), ent->d_name);
            if ((dir2 = opendir(buf1)) != NULL) {
                m_class_members.push_back(std::list<uint32_t>());
                while ((ent2 = readdir(dir2)) != NULL) {
                    sprintf(buf2, "%s/%s", buf1, ent2->d_name);
                    try {
                        FaceImage* fi = new FaceImage(buf2);
                        m_face_images.push_back(fi);
                        m_class_members.back().push_back(i++);
                        m_class_lookup.push_back(m_class_members.size() - 1);
                    } catch(...) {
                        continue;
                    }
                }
                if (m_class_members.back().size() == 0)
                    m_class_members.pop_back();
            }
        }
    }
}

FaceCatalogue::~FaceCatalogue()
{
    for (auto fi : m_face_images)
    {
        delete fi;
    }
}

void FaceCatalogue::choose_training_sets(float proportion, uint32_t seed)
{
    if (seed) srand(seed); else srand(time(NULL));
    m_test_sets = m_class_members;
    m_training_sets.clear();

    for (auto &class_list : m_test_sets)
    {
        std::list<uint32_t> training_list;
        int num_in_training = class_list.size() * proportion;
        for (int i = 0; i < num_in_training; i++)
        {
            auto it = class_list.begin();
            for (int j = 0; j < rand() % class_list.size(); j++)
                it++;
            training_list.splice(training_list.end(), class_list, it);
        }
        m_training_sets.push_back(training_list);
    }
}


std::vector<FaceImage*> FaceCatalogue::get_set_of_class(uint32_t class_id, SetType type) const
{
    std::vector<FaceImage*> ret;
    std::vector<std::list<uint32_t> > member_list;
    switch(type) {
        case SetType::TEST_SET:
            member_list = m_test_sets;
            break;

        case SetType::TRAINING_SET:
            member_list = m_training_sets;
            break;

        case SetType::ALL_SET:
            member_list = m_class_members;
            break;
    }

    for (auto i : member_list[class_id])
    {
        ret.push_back(m_face_images[i]);
    }
    return ret;
}

wxString FaceCatalogue::get_info(uint32_t index) const
{
    return wxString::Format("Class ID %u", m_class_lookup[index]);
}
