#include "pch.h"

#include "PBRRenderer.h"


namespace Clap
{
    struct CameraModelBuffer
    {
        glm::mat4 ViewProjection;
        glm::mat4 Model;
    };

    struct PBRBuffer
    {
        Ref<Framebuffer> GBuffer;
        Ref<Framebuffer> Frame;
        uint32_t Width, Height;
        PostProcessingUniform PostProcess;

        Ref<VertexArray> QuadVertexArray;

        Ref<UniformBuffer> CameraModelUniformBuffer;
        Ref<UniformBuffer> MaterialUniformBuffer;
        Ref<UniformBuffer> PostProcessingUniformBuffer;

        Ref<Texture2D> EmptyTexture;

        Ref<Shader> GBufferShader;
        Ref<Shader> PBRLightShader;
        Ref<Shader> PostProcessShader;
        
        Ref<VertexArray> QuadVA;
        
        std::vector<Model> RenderQueue;

        glm::mat4 Camera;
    };

    static PBRBuffer s_Buffer;

    void PBRRenderer::Init(uint32_t width, uint32_t height)
    {
        s_Buffer.Width = width;
        s_Buffer.Height = height;
        s_Buffer.QuadVertexArray = VertexArray::Create();
        
        float quadVertices[4 * 4] =
        {
            -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
        };

        uint32_t quadIndices[6] =
        {
            0, 2, 1, 3, 2, 0
        };
        
        auto quadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
        quadVertexBuffer->SetLayout({
            { GraphicsDataType::FLOAT2, Attribute::POSITION },
            { GraphicsDataType::FLOAT2, Attribute::TEXCOORD0 },
        });
        s_Buffer.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
        s_Buffer.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(quadIndices, 6));                                  

        s_Buffer.CameraModelUniformBuffer = UniformBuffer::Create(sizeof(CameraModelBuffer));
        s_Buffer.MaterialUniformBuffer = UniformBuffer::Create(sizeof(PBRMaterialUniform));
        s_Buffer.PostProcessingUniformBuffer = UniformBuffer::Create(sizeof(PostProcessingUniform));
        
        s_Buffer.GBufferShader = Shader::Create(CLAP_ENGINE_PATH("/res/GBuffer.glsl"));
        s_Buffer.PBRLightShader = Shader::Create(CLAP_ENGINE_PATH("/res/PBRShader.glsl"));
        s_Buffer.PostProcessShader = Shader::Create(CLAP_ENGINE_PATH("/res/PostProcess.glsl"));

        //TODO: Entity ID Encoding
        s_Buffer.GBuffer = Framebuffer::Create({s_Buffer.Width, s_Buffer.Height, {{TextureFormat::RGBA16F, FilterType::NEAREST, FilterType::NEAREST}, //Position
                                                                                  {TextureFormat::RGBA16F, FilterType::NEAREST, FilterType::NEAREST}, //Normal
                                                                                  {TextureFormat::RGBA8, FilterType::NEAREST, FilterType::NEAREST}, //Colour 
                                                                                  {TextureFormat::RGBA16F, FilterType::NEAREST, FilterType::NEAREST}, //Emission
                                                                                  {TextureFormat::RGBA8, FilterType::NEAREST, FilterType::NEAREST}, //Metallic, rough, AO, Material ID
                                                                                  TextureFormat::DEPTH24STENCIL8}, 1, false}); //TODO: More TExture slots, monochromatic R8 maps
        s_Buffer.Frame = Framebuffer::Create({s_Buffer.Width, s_Buffer.Height, {{TextureFormat::RGBA16F, FilterType::NEAREST, FilterType::NEAREST}, TextureFormat::DEPTH24STENCIL8}, 1, false});
    
        uint32_t white = 0xFFFFFFFF;
        s_Buffer.EmptyTexture = Texture2D::Create(1, 1, {TextureFormat::RGBA8, FilterType::NEAREST, FilterType::NEAREST});
        s_Buffer.EmptyTexture->SetData(&white, sizeof(uint32_t));
    }

    void PBRRenderer::Begin(const glm::mat4& projection, const glm::mat4& view)
    {
        glm::mat4 ViewProjection = projection * glm::inverse(view);
        s_Buffer.Camera = ViewProjection;
        s_Buffer.CameraModelUniformBuffer->Bind(0);
        s_Buffer.MaterialUniformBuffer->Bind(1);
        s_Buffer.CameraModelUniformBuffer->SetData(&ViewProjection, sizeof(ViewProjection));
        
        s_Buffer.GBuffer->Bind();

        Graphics::ToggleDepthTest(true);
        Graphics::ToggleBackfaceCulling(true);
        Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        Graphics::SetViewport(0, 0, s_Buffer.Width, s_Buffer.Height);
        Graphics::Clear();
    }

    void PBRRenderer::Submit(const Ref<Model>& model, Transform transform)
    {
        glm::mat4 modelMatrix = transform.GetTransform();
        s_Buffer.CameraModelUniformBuffer->SetData(&modelMatrix, sizeof(modelMatrix), sizeof(glm::mat4));

        for(auto mesh : model->GetMeshes())
        {
            Ref<Material> material = model->GetMeshMaterial(mesh.first);
            if (!material->GetShader()) //TODO: Shaders for materials
                s_Buffer.GBufferShader->Bind(); //TODO Optimize by checking wether the last shader was the same
            else material->GetShader()->Bind();
            s_Buffer.MaterialUniformBuffer->SetData(material->GetData(), material->GetSize());

            Ref<Texture2D> currentTexture;
            currentTexture = material->GetTexture("Albedo");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 0);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 0);
            
            currentTexture = material->GetTexture("Normals");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 1);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 1);

            currentTexture = material->GetTexture("Metallic");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 2);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 2);

            currentTexture = material->GetTexture("Roughness");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 3);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 3);

            currentTexture = material->GetTexture("AO");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 4);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 4);

            currentTexture = material->GetTexture("Emission");
            if(currentTexture) Graphics::BindTexture(currentTexture->GetID(), 5);
            else Graphics::BindTexture(s_Buffer.EmptyTexture->GetID(), 5);

            mesh.second->Draw();
        }
    }
    //LIGHT CULLING SHOULD CREATE AABB ITSELF with radius
    void PBRRenderer::End()
    {
        s_Buffer.GBufferShader->Unbind();
        s_Buffer.GBuffer->Unbind();

        //LIGHT PASS
        s_Buffer.Frame->Bind();
        s_Buffer.PBRLightShader->Bind();

        auto gBufferSpecification = s_Buffer.GBuffer->GetSpecification();
        int i = 0;
        for(TextureSpecification spec : gBufferSpecification.Attachments.Attachments)
        {
            if(spec.Format != TextureFormat::DEPTH24STENCIL8)
            {
                Graphics::BindTexture(s_Buffer.GBuffer->GetColorAttachmentRendererID(i), i);
                i++;
            }
        }
        Graphics::BindTexture(s_Buffer.GBuffer->GetDepthAttachmentRendererID(), i);

        Graphics::SetViewport(0, 0, s_Buffer.Width, s_Buffer.Height);
        Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        Graphics::Clear();
        //Point, area, spot lights

        //Directional lightd and IBL
        Graphics::DrawIndexed(s_Buffer.QuadVertexArray);

        s_Buffer.PBRLightShader->Unbind();
        s_Buffer.Frame->Unbind();
        
        //POST-PROCESS PASS
        s_Buffer.PostProcessingUniformBuffer->Bind(0);
        s_Buffer.PostProcessShader->Bind();

        auto specification = s_Buffer.Frame->GetSpecification();
        int j = 0;
        for(TextureSpecification spec : specification.Attachments.Attachments)
        {
            if(spec.Format != TextureFormat::DEPTH24STENCIL8)
            {
                Graphics::BindTexture(s_Buffer.Frame->GetColorAttachmentRendererID(j), j);
                j++;
            }
        }
        Graphics::BindTexture(s_Buffer.Frame->GetDepthAttachmentRendererID(), j);

        Graphics::SetViewport(0, 0, s_Buffer.Width, s_Buffer.Height);
        Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        Graphics::Clear();

        Graphics::DrawIndexed(s_Buffer.QuadVertexArray);

        s_Buffer.PostProcessShader->Unbind();
    }
}