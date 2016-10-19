#include "result.h"

Result::Result(uint32_t cls) : m_class(cls)
{}

void Result::add_detection(uint32_t detected_class)
{
    m_detection.push_back(detected_class);
}

float Result::get_percent_accuracy()
{
    int sum = 0, correct = 0;
    for (auto d : m_detection)
    {
        sum++;
        if (d == m_class) correct++;
    }
    return correct * 100.0f / sum;
}

const std::vector<uint32_t>& Result::get_detections()
{
    return m_detection;
}

uint32_t Result::get_class()
{
    return m_class;
}
