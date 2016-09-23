#include <fstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "img_mgr.h"
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <dirent.h>

FaceImage::FaceImage(const std::string &path, uint32_t class_id) :
    m_class_id(class_id)
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
    printf("Loading PGM %d x %d, maxval = %d\n", m_width, m_height, m_maxval);

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
    m_data_mat = MatrixXd(m_width, m_height);

    for (uint32_t j = 0; j < m_height; j++) {
        for (uint32_t i = 0; i < m_width; i++) {
            m_data_mat(i, j) = *(buf++) * 1.0f / m_maxval;
        }
    }
}

MatrixXd FaceImage::to_vector() const
{
    MatrixXd ret(m_data_mat);
    ret.resize(m_width * m_height, 1);
    return ret;
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
    uint32_t class_id = 0, i = 0;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            sprintf(buf1, "%s/%s", path.c_str(), ent->d_name);
            if ((dir2 = opendir(buf1)) != NULL) {
                m_class_members[class_id].push_back(std::list<uint32_t>());
                while ((ent2 = readdir(dir2)) != NULL) {
                    sprintf(buf2, "%s/%s", buf1, ent2->d_name);
                    try {
                        FaceImage* fi = new FaceImage(buf2, class_id);
                        m_face_images.push_back(fi);
                        m_class_members[class_id].push_back(i++);
                    } catch(...) {
                        continue;
                    }
                }
                class_id++;
            }
        }
    }
}

FaceCatalogue::~FaceCatalogue()
{
    for (auto fi : m_face_images)
    {
        printf("Deleter\n") ;
        delete fi;
    }
}

void FaceCatalogue::choose_training_sets(float proportion, uint32_t seed)
{
    if (seed) srand(seed); else srand(time(NULL));
    for (auto class_list : m_class_members)
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
    }
}


std::vector<FaceImage*> FaceCatalogue::get_set_of_class(uint32_t class_id)
{
    std::vector<FaceImage*> ret;
    for (auto fi : m_face_images)
    {
        if (fi->get_class() == class_id) ret.push_back(fi);
    }
}
