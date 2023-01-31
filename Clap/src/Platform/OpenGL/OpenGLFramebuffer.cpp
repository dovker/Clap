#include "pch.h"

#include "Data/ByteBuffer.h"

#include "Graphics/API/Shader.h"
#include "Graphics/API/Utils.h"
#include "OpenGLFramebuffer.h"
#include "OpenGL.h"

#include <glad/glad.h>

namespace Clap
{
    namespace Utils {

        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, TextureSpecification specification, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,  ToOpenGLInternalFormat(specification.Format), width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLInternalFormat(specification.Format), width, height, 
                            0, ToOpenGLDataFormat(specification.Format), 
                            ToOpenGLDataType(specification.Format), nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterTypeToOpenGLBaseType(specification.MinFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterTypeToOpenGLBaseType(specification.MagFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, WrapTypeToOpenGLBaseType(specification.Wrap));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapTypeToOpenGLBaseType(specification.Wrap));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapTypeToOpenGLBaseType(specification.Wrap));
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, TextureSpecification specification, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, ToOpenGLInternalFormat(specification.Format), width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, ToOpenGLInternalFormat(specification.Format), width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterTypeToOpenGLBaseType(specification.MinFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterTypeToOpenGLBaseType(specification.MagFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, WrapTypeToOpenGLBaseType(specification.Wrap));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapTypeToOpenGLBaseType(specification.Wrap));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapTypeToOpenGLBaseType(specification.Wrap));
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(TextureFormat format)
        {
            switch (format)
            {
                case TextureFormat::DEPTH24STENCIL8:  return true;
                default: break;
            }

            return false;
        }

    }


    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(spec.Format))
                m_ColorAttachmentSpecifications.emplace_back(spec);
            else
                m_DepthAttachmentSpecification = spec;
        }

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_ID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_ID)
        {
            glDeleteFramebuffers(1, &m_ID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
            
            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        
        bool multisample = m_Specification.Samples > 1;

        // Attachments
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColorAttachments[i]);
                Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, m_ColorAttachmentSpecifications[i], m_Specification.Width, m_Specification.Height, i);
            }
        }

        if (m_DepthAttachmentSpecification.Format != TextureFormat::NONE)
        {
            Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
            Utils::BindTexture(multisample, m_DepthAttachment);
            Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, m_DepthAttachmentSpecification, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
        }

        if (m_ColorAttachments.size() > 1)
        {
            GLenum buffers[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }
        
        CLAP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
        
        Invalidate();
    }

    Ref<ByteBuffer> OpenGLFramebuffer::GetPixels(uint32_t attachment, int x, int y, int width, int height) const
    {
        CLAP_ASSERT(attachment < m_ColorAttachments.size(), "Color attachment is not created");

        TextureFormat format = m_ColorAttachmentSpecifications[attachment].Format;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        
        Ref<ByteBuffer> pixels = ByteBuffer::Create(TextureFormatToSize(format) * width * height);
        glReadPixels(x, y, width, height, ToOpenGLDataFormat(format), ToOpenGLDataType(format), (void*)pixels->GetData().data());

        return pixels;
    }

    int OpenGLFramebuffer::GetPixelInt(uint32_t attachment, int x, int y) const
    {
        CLAP_ASSERT(attachment < m_ColorAttachments.size(), "Color attachment is not created");
        CLAP_ASSERT(m_ColorAttachmentSpecifications[attachment].Format == TextureFormat::INT32, "Color attachment is not of type INT32");

        TextureFormat format = m_ColorAttachmentSpecifications[attachment].Format;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);

        int pixel;
        glReadPixels(x, y, 1, 1, ToOpenGLDataFormat(format), ToOpenGLDataType(format), &pixel);

        return pixel;
    }
    
    uint32_t OpenGLFramebuffer::GetPixelUInt(uint32_t attachment, int x, int y) const
    {
        CLAP_ASSERT(attachment < m_ColorAttachments.size(), "Color attachment is not created");
        CLAP_ASSERT(m_ColorAttachmentSpecifications[attachment].Format == TextureFormat::UINT32, "Color attachment is not of type UINT32");

        TextureFormat format = m_ColorAttachmentSpecifications[attachment].Format;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);

        uint32_t pixel;
        glReadPixels(x, y, 1, 1, ToOpenGLDataFormat(format), ToOpenGLDataType(format), &pixel);

        return pixel;
    }

    void OpenGLFramebuffer::ClearColorAttachment(uint32_t attachment, void* value) const
    {
        CLAP_ASSERT(attachment < m_ColorAttachments.size(), "Color attachment is not created");

        TextureFormat format = m_ColorAttachmentSpecifications[attachment].Format;

		glClearTexImage(m_ColorAttachments[attachment], 0, ToOpenGLDataFormat(format), ToOpenGLDataType(format), value);

    }
}
