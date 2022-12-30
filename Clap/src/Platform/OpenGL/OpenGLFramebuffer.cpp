#include "pch.h"
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
            #ifdef CLAP_OPENGL_4_5
                glCreateTextures(TextureTarget(multisampled), count, outID);
            #else
                glGenTextures(count, outID);
            #endif
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
                glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLInternalFormat(specification.Format), width, height, 0, 
                                ToOpenGLDataFormat(specification.Format), ToOpenGLDataType(specification.Format), nullptr);

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
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
            
            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        
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
            glGenTextures(1, &m_DepthAttachment);
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
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
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

    void* OpenGLFramebuffer::GetPixels(uint32_t attachment, int x, int y, int width, int height) const
    {
        CLAP_ASSERT(attachment < m_ColorAttachments.size(), "Color attachment is not created");

        TextureFormat format = m_ColorAttachmentSpecifications[attachment].Format;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);

        void* pixels = malloc(TextureFormatToSize(format) * width * height);

        glReadPixels(x, y, width, height, ToOpenGLDataFormat(format), ToOpenGLDataType(format), pixels);

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

        #ifdef CLAP_OPENGL_4_5
		    glClearTexImage(m_ColorAttachments[attachment], 0, ToOpenGLDataFormat(format), ToOpenGLDataType(format), value);
        #else
            CLAP_WARN("Using ClearColorAttachment() in OpenGL 3.3 is not tested and may result in lower performance and / or glitches due to binding");
            std::vector<GLubyte> emptyData(m_Specification.Width * m_Specification.Height * TextureFormatToSize(format), 0);
            glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[attachment]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Specification.Width, m_Specification.Height, ToOpenGLDataFormat(format), ToOpenGLDataType(format), &emptyData[0]);
        #endif
    }
}
