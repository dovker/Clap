#include <glm/glm.hpp>

#include "Clap.h"

using namespace Clap;

bool running = true;

void OnEvent(Event& e)
{
    switch(e.Type)
    {
        case EventType::WindowClose:
            running = false;
        break;
        default: break;
    }
}
struct CameraBuffer
{
    glm::mat4 ViewProjection;
};

struct ModelData
{
    glm::mat4 Transform;
};

int main()
{
    EventSystem::AttatchCallback(&OnEvent);

    Window* window = Window::Create();

    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 camPos = { 0.1f, 3.1f, 3.2f };
    glm::vec3 camTarget = { 0.0f, 0.0f, 0.0f };
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.05f, 300.0f);
    glm::mat4 viewMat = glm::lookAt(camPos, camTarget, up);

    CameraBuffer cam = {
        projectionMat * viewMat
    };

    glm::mat4 modelMat = glm::mat4(1.0f);
    ModelData model = {
        modelMat
    };

    Graphics::Init();

    Ref<Shader> shader = Shader::Create("../../TestGround/res/ShaderMain.glsl"); 
    Ref<UniformBuffer> cameraBuffer = UniformBuffer::Create(sizeof(CameraBuffer), 0);
    Ref<UniformBuffer> modelBuffer = UniformBuffer::Create(sizeof(ModelData), 1);
    shader->SetUniformBufferBinding(cameraBuffer, "Camera");
    shader->SetUniformBufferBinding(modelBuffer, "ModelData");
    

    cameraBuffer->SetData(&cam, sizeof(cam), 0);
    modelBuffer->SetData(&model, sizeof(model), 0);
    

    std::vector<float> vertices =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    std::vector<uint32_t> indices = 
    {
        0, 1, 2,
        2, 3, 0
    };

    Ref<Mesh> triangle = Mesh::Create(vertices, indices, true);
    CLAP_TRACE("AMOGUS");
    Ref<Mesh> cube = ObjParser::Parse("../../TestGround/res/spruce.obj", true);
    Ref<Texture2D> albedo = Texture2D::Create("../../TestGround/res/textures/spruce_texturing_elka_BaseColor.png");
    Ref<Texture2D> normal = Texture2D::Create("../../TestGround/res/textures/spruce_texturing_elka_Normal.png");
    CLAP_TRACE("AMOGUS");

    float rotation = 0.0f;

    while(running)
    {
        while(!EventSystem::IsEmpty())
        {
            Event e = EventSystem::GetEvent();
            switch(e.Type)
            {
                case EventType::WindowClose:
                    running = false;
                break;
                case EventType::KeyPressed:
                break;
                default: break;
            }
        }
        if(Input::IsKeyPressed(KEY_W))
        {
            camPos.z -=0.1;
            glm::mat4 viewMat = glm::lookAt(camPos, camTarget, up);
            cam.ViewProjection = projectionMat * viewMat;
            cameraBuffer->SetData(&cam, sizeof(cam), 0);
        }
        if(Input::IsKeyPressed(KEY_S))
        {
            camPos.z +=0.1;
            glm::mat4 viewMat = glm::lookAt(camPos, camTarget, up);
            cam.ViewProjection = projectionMat * viewMat;
            cameraBuffer->SetData(&cam, sizeof(cam), 0);
        }
        if(Input::IsKeyPressed(KEY_A))
        {
            rotation += 1.0f;
            model.Transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            model.Transform = glm::rotate(model.Transform, glm::radians(rotation/2), glm::vec3(2.0f, 0.0f, 0.0f));
            model.Transform = glm::scale(model.Transform, glm::vec3(0.01f, 0.01f, 0.01f));
            modelBuffer->SetData(&model, sizeof(model), 0);
        }
        Graphics::SetClearColor({0.3f, 0.53f, 0.5f, 1.0f});
        Graphics::Clear();
        Graphics::ToggleDepthTest(true);
        Graphics::ToggleBackfaceCulling(true);
        Graphics::ToggleBlending(true);


        shader->Bind();
        shader->SetInt("u_Albedo", 0);
        albedo->Bind(0);
        shader->SetInt("u_Normal", 1);
        normal->Bind(1);

        //triangle->Draw();
        
        cube->Draw();
        //TODO: Test immediate events and input polling
        //TODO: Test graphics by writing a deferred renderer

        window->OnUpdate();
    }

    return 0;
}