#include <eigen3/Eigen/Dense>
#include <string>

using Eigen::MatrixXd;

class FaceImage {
private:
    MatrixXd m_data_mat;
    uint32_t m_class_id;
    uint32_t m_width, m_height, m_maxval;

    void load_from_pixel_data(const uint8_t* buf);

public:
    FaceImage(std::string path, uint32_t class_id);
    void to_rgb_buffer(uint8_t** buf, uint32_t* size);
    std::vector<uint8_t> to_rgb_buffer();

    uint32_t get_width() { return m_width; }
    uint32_t get_height() { return m_height; }
};
