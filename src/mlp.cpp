#include "mlp.h"
#include <stdexcept>
#include <cstdlib>

void Neuron::add_input(Neuron* n, float weight)
{
    if (!weight)
        weight = (rand() * 2.0f / RAND_MAX) - 1.0f;
    m_inputs.push_back(n);
    m_weights.push_back(weight);
}

MLP::MLP(std::vector<uint32_t> layers)
{
    for (int j = 0; j < layers[0]; j++)
    {
        m_inputs.push_back(InputNeuron());
    }

    for (int i = 1; i < layers.size(); i++)
    {
        m_neurons.push_back(std::vector<Neuron>());
        for (int j = 0; j < layers[i]; j++)
        {
            m_neurons[i].push_back(Neuron());
        }
    }
}

void MLP::generate_links()
{
    for (auto &n1 : m_inputs)
    {
        for (auto &n2 : m_neurons[0])
        {
            n1.add_output(&n2);
            n2.add_output(&n1);
        }
    }

    for(int i = 0; i < m_neurons.size() - 1; i++)
    {
        for (auto &n1 : m_neurons[i])
        {
            for (auto &n2 : m_neurons[i+1])
            {
                n1.add_output(&n2);
                n2.add_input(&n1);
            }
        }
    }
}

void MLP::train(std::vector<float> &input, uint32_t exp_class)
{

}

void MLP::set_input(std::vector<float> &input)
{
    if (input.size() != m_neurons.front().size())
        throw std::runtime_error("Wrong size input for MLP");

    int i = 0;
    for (auto &n : m_inputs)
    {
        n.set_value(input[i++]);
    }
}

void MLP::set_expected(uint32_t cl)
{
    if(cl >= m_neurons.back().size())
        throw std::runtime_error("Not enough outputs for class in MLP");

    uint32_t i = 0;
    for(auto &n : m_neurons.back())
    {
        n.set_expected(i == cl ? 1.0 : -1.0);
    }
}
