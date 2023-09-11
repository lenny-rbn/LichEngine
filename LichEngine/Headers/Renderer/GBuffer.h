#pragma once
#include <Core/RHI.h>


namespace Renderer
{
	class LICH_ENGINE GBuffer
	{
	public:
		GBuffer(int width, int height);
		~GBuffer();

		void Destroy();

		uint& GetGBuffer() { return m_gBuffer; }
		uint GetGPosition() { return m_gPosition; }
		uint GetGViewPosition() { return m_gViewPosition; }
		uint GetGNormal () { return m_gNormal; }
		uint GetGViewNormal() { return m_gViewNormal; }
		uint GetGColor() { return m_gColor; }
		uint GetGMetallicRoughness() { return m_gMetallicRoughnessAO; }
		uint GetDepthBuffer() { return m_depthBuffer; }

	private:
		uint m_gBuffer;
		uint m_gPosition;
		uint m_gViewPosition;
		uint m_gNormal;
		uint m_gViewNormal;
		uint m_gColor;
		uint m_gMetallicRoughnessAO;
		uint m_depthBuffer;
	};
}