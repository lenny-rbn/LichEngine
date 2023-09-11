#include <pch.h>
#include <Renderer/GBuffer.h>



namespace Renderer
{
	GBuffer::GBuffer(int width, int height)
	{
		WrapperRHI::RHI::CreateGBuffer(m_gBuffer, m_gPosition, m_gViewPosition, m_gNormal, m_gViewNormal, m_gColor, m_gMetallicRoughnessAO, m_depthBuffer, width, height);
	}

	GBuffer::~GBuffer()
	{

	}

	void GBuffer::Destroy()
	{
		WrapperRHI::RHI::DeleteTexture(m_gPosition);
		WrapperRHI::RHI::DeleteTexture(m_gViewPosition);
		WrapperRHI::RHI::DeleteTexture(m_gNormal);
		WrapperRHI::RHI::DeleteTexture(m_gViewNormal);
		WrapperRHI::RHI::DeleteTexture(m_gColor);
		WrapperRHI::RHI::DeleteTexture(m_gMetallicRoughnessAO);
		WrapperRHI::RHI::DeleteRenderbuffer(m_depthBuffer);
		WrapperRHI::RHI::DeleteFramebuffer(m_gBuffer);
	}
}
