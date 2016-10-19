#pragma once

#include <vector>
#include <cstdint>

class Result
{
private:
    uint32_t m_class;
    std::vector<uint32_t> m_detection;

public:
    Result(uint32_t cls);
    void add_detection(uint32_t detected_class);
    float get_percent_accuracy();
    const std::vector<uint32_t>& get_detections();
    uint32_t get_class();
};
