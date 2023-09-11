#include <pch.h>
#include <random>

#include <Renderer/Kernel.h>


namespace Renderer
{
    Kernels::Kernels()
    {
        std::uniform_real_distribution<float> rnd(0.0, 1.0);
        std::default_random_engine generator;

        for (unsigned int i = 0; i < 10; ++i)
        {
            float scale = float(i) / 10.f;
            Vector3D sample(rnd(generator) * 2.f - 1.f, rnd(generator) * 2.f - 1.f, rnd(generator));

            sample.Normalize();
            sample *= rnd(generator) * Lerp(0.1f, 1.f, scale * scale);
            m_ssaoKernel.push_back(sample);
        }

        std::vector<Vector3D> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++)
        {
            // rotate around z-axis (in tangent space)
            Vector3D noise(rnd(generator) * 2.f - 1.f, rnd(generator) * 2.f - 1.f, 0.f);
            ssaoNoise.push_back(noise);
        }

        WrapperRHI::RHI::CreateNoiseTexture(m_noiseTexture, ssaoNoise);
    }

    void Kernels::SendKernelsToShader(Resources::Shader* shader)
    {
        WrapperRHI::RHI::UseShader(shader->GetShaderProgram());

        for (unsigned int i = 0; i < 10; ++i)
            WrapperRHI::RHI::Send3fToShader(shader, ("samples[" + std::to_string(i) + "]").c_str(), m_ssaoKernel[i]);
    }

    void Kernels::Destroy()
    {
        WrapperRHI::RHI::DeleteTexture(m_noiseTexture);
    }

    Kernels::~Kernels()
    {

    }
}