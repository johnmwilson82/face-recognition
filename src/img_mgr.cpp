#include <fstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "img_mgr.h"
#include <cstdio>
#include <cstdlib>

FaceImage::FaceImage(std::string path, uint32_t class_id) :
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

    uint32_t width, height;
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

void FaceImage::to_rgb_buffer(uint8_t** buf, uint32_t* size)
{
    int sz = 0;
    *buf = new uint8_t[m_height * m_width * 3];
    uint8_t* p = *buf;
    for (uint32_t j = 0; j < m_height; j++) {
        for (uint32_t i = 0; i < m_width; i++) {
            *(p++) = (uint8_t)(m_data_mat(i, j) * m_maxval);
            *(p++) = (uint8_t)(m_data_mat(i, j) * m_maxval);
            *(p++) = (uint8_t)(m_data_mat(i, j) * m_maxval);
            sz += 3;
        }
    }
    *size = sz;
}
