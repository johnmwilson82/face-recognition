#include "mlp.h"
#include <stdexcept>
#include <cstdlib>

Edge::Edge(Neuron *input, Neuron *output, float weight) :
    m_input(input),
    m_output(output),
    m_weight(weight)
{
    if (std::isnan(m_weight)) {
        m_weight = (rand() * 2.0f / RAND_MAX) - 1.0f;
    }
    input->add_output(this);
    output->add_input(this);
}

float Edge::get_output()
{
    return m_input->get_output() * m_weight;
}

float Edge::get_unweighted_output()
{
    return m_input->get_output();
}

float Neuron::get_output()
{
    static int l = 0;
    l++;
    float sum = 0.0f;
    for (auto e : m_inputs)
    {
        sum += e->get_output();
        //printf("level = %d, input = %f, sum = %f\n", l, e->get_output(), sum);
    }
    l--;
    //printf("*sum = %f bias = %f, (%f)\n", sum, m_bias, activation_function(sum + m_bias));
    return activation_function(sum + m_bias);
}

MLP::MLP(uint32_t input_size, uint32_t output_size, const wxPropertyGridInterface& props)
{
    int nhidden_nodes = props.GetPropertyByName(wxT("propHiddenNodes"))->GetValue().GetLong();
    m_learning_rate = props.GetPropertyByName(wxT("propLearningRate"))->GetValue().GetDouble();
    m_inputs = std::vector<InputNeuron>(input_size);

    m_neurons.push_back(std::vector<Neuron>(nhidden_nodes));
    m_neurons.push_back(std::vector<Neuron>(output_size));

    printf("inputs length = %d\n", input_size);
    printf("hidden length = %d\n", nhidden_nodes);
    printf("outputs length = %d\n", output_size);

    generate_links();
}

void MLP::generate_links()
{
    for (auto &n1 : m_inputs)
    {
        for (auto &n2 : m_neurons[0])
        {
            m_edges.emplace_back(&n1, &n2);
        }
    }

    for(int i = 0; i < m_neurons.size() - 1; i++)
    {
        for (auto &n1 : m_neurons[i])
        {
            for (auto &n2 : m_neurons[i+1])
            {
                m_edges.emplace_back(&n1, &n2);
            }
        }
    }
}

void MLP::train(const VectorXf &input, uint32_t exp_class)
{
    VectorXf exp_vec = VectorXf::Ones(m_neurons.back().size()) * 0.0f;
    exp_vec(exp_class) = 1.0f;
    int i = 0;
    for (auto &n : m_inputs)
    {
        n.set_value(input(i++));
    }

    std::vector<std::vector<float> > minus_delta_e;
    int l = 0;
    for (auto layer = m_neurons.rbegin(); layer != m_neurons.rend(); layer++)
    {
        minus_delta_e.push_back(std::vector<float>(layer->size()));

        for (int j = 0; j < layer->size(); j++)
        {
            float v = (*layer)[j].get_output();
            if (layer == m_neurons.rbegin())
            {
                minus_delta_e.back()[j] =
                    (v - exp_vec[j]) * Neuron::derivative_af(v);
            }
            else
            {
                float sum_pseudo_error = 0.0f;
                int k = 0;
                for(Edge* e : (*layer)[j].get_output_edges())
                {
                    sum_pseudo_error += (*(minus_delta_e.rbegin() + 1))[k++] * e->get_weight();
                }
                minus_delta_e.back()[j] =
                    sum_pseudo_error * Neuron::derivative_af(v);
            }
            for (Edge* e : (*layer)[j].get_input_edges())
            {
                float delta_weight = m_learning_rate *
                    -minus_delta_e.back()[j] *
                    e->get_unweighted_output();

                e->set_weight(e->get_weight() + delta_weight);
            }
        }
    }
}

float MLP::sq_error(const VectorXf &d, const VectorXf &y)
{
    if (d.size() != y.size()) throw std::runtime_error("sq_error error in inputs");
    float ret = 0;
    for (int i = 0; i < d.size(); i++)
    {
        ret += (d[i] - y[i]) * (d[i] - y[i]);
    }
    return 0.5 * ret;
}

VectorXf MLP::get_output(const VectorXf &input)
{
    VectorXf ret(m_neurons.back().size());

    if (input.size() != m_inputs.size())
        throw std::runtime_error("Wrong size input for MLP");

    //for (int j = 0; j < input.size(); j++) printf("%.2f, ", input[j]); printf("\n");
    int i = 0;
    for (auto &n : m_inputs)
    {
        n.set_value(input(i++));
    }

    i = 0;
    for (auto &n : m_neurons.back())
    {
        float o = n.get_output();
        //printf("neuron output = %f\n", o);
        ret(i++) = o;
    }

    return ret;
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

uint32_t MLP::classify(const VectorXf &input)
{
    uint32_t ret = 0;
    auto output = get_output(input);
    float max_output = output(0);
    for (int i = 1; i < output.size(); i++)
    {
        if (max_output < output(i))
        {
            max_output = output(i);
            ret = i;
        }
    }
    return ret;
}

std::vector<std::shared_ptr<wxPGProperty> > MLP::get_props()
{
    auto prop1 = std::make_shared<wxIntProperty>(wxT("Number of iterations"), wxT("propIterations"));
    prop1->SetValue(500);
    auto prop2 = std::make_shared<wxIntProperty>(wxT("Number of hidden nodes"), wxT("propHiddenNodes"));
    prop2->SetValue(15);
    auto prop3 = std::make_shared<wxFloatProperty>(wxT("Learning rate"), wxT("propLearningRate"));
    prop3->SetValue(0.01);

    return {prop1, prop2, prop3};
}

REGISTER_CLASSIFIER(MLP)
