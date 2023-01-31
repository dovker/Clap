#pragma once
#include <pch.h>

#include "Graphics/API/Framebuffer.h"

namespace Clap
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        virtual void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        
        virtual glm::vec2 GetSize() const override { return {m_Specification.Width, m_Specification.Height}; }

        virtual uint32_t GetColorAttachmentRendererID(uint32_t attachment = 0)  const override { return m_ColorAttachments[attachment]; }

        virtual Ref<ByteBuffer> GetPixels(uint32_t attachment, int x, int y, int width, int height) const override;
        virtual int GetPixelInt(uint32_t attachment, int x, int y)  const override;
        virtual uint32_t GetPixelUInt(uint32_t attachment, int x, int y) const override;

        virtual void ClearColorAttachment(uint32_t attachment, void* value) const override;
        
        virtual uint32_t GetDepthAttachmentRendererID()  const override { return m_DepthAttachment; }
        

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    protected:
        uint32_t m_ID = 0;
        
        FramebufferSpecification m_Specification;

        std::vector<TextureSpecification> m_ColorAttachmentSpecifications;
        TextureSpecification m_DepthAttachmentSpecification = TextureFormat::NONE;
        
        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };

}