#pragma once

#include "Core/Core.h"
#include "API.h"

namespace Clap
{
    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<TextureSpecification> attachments)
            : Attachments(attachments) {}

        std::vector<TextureSpecification> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        // FramebufferFormat Format =
        FramebufferAttachmentSpecification Attachments;

        uint32_t Samples = 1;
        
        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        
        virtual glm::vec2 GetSize() const = 0;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t attachment = 0) const = 0;

        virtual void* GetPixels(uint32_t attachment, int x, int y, int width, int height) const = 0;
        virtual int GetPixelInt(uint32_t attachment, int x, int y) const = 0;
        virtual uint32_t GetPixelUInt(uint32_t attachment, int x, int y) const = 0;

        virtual void ClearColorAttachment(uint32_t attachment, void* value) const = 0;
        
        virtual uint32_t GetDepthAttachmentRendererID() const = 0;
        //TODO: Stencil support

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

    };
}