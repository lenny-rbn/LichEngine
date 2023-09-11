#pragma once
#include <Resources/Shader.h>


namespace Renderer
{
	class LICH_ENGINE Kernels
	{
	public:
		Kernels();
		~Kernels();

		uint GetNoiseTexture() { return m_noiseTexture; }
		void SendKernelsToShader(Resources::Shader* shader);
		void Destroy();

	private:
		uint m_noiseTexture;
		std::vector<Vector3D> m_ssaoKernel;
	};
}