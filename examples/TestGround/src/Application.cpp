#include <glm/glm.hpp>

#include "Clap.h"

using namespace Clap;
namespace fs = std::filesystem;

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
    Ref<UI> ui = UI::Create(window);
    ui->Init();
    Ref<Element> elem = Element::Create(AABB({50.0f, 25.0f}, {100.0f, 50.0f}, {0.0f, 0.0f}));
    Ref<Element> elem2 = Element::Create(AABB({500.0f, 25.0f}, {100.0f, 100.0f}, {0.5f, 0.5f}));
    Ref<Element> elem3 = Element::Create(AABB({50.0f, 50.0f}, {50.0f, 50.0f}, {0.5f, 0.5f}));
    ui->AttachElement(elem);
    ui->AttachElement(elem2);
    elem2->AddChild(elem3);

    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 camPos = { 0.1f, 3.1f, 3.2f };
    glm::vec3 camTarget = { 0.0f, 0.0f, 0.0f };
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.05f, 300.0f);
    glm::mat4 viewMat = glm::lookAt(camPos, camTarget, up);

    CameraBuffer cam = {
        projectionMat * viewMat
    };

    Transform transform(glm::vec3(0.0, -0.5, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    glm::mat4 modelMat = transform.GetTransform();
    ModelData model = {
        modelMat
    };

    Graphics::Init();

    Ref<Shader> shader = Shader::Create("../../../TestGround/res/ShaderMain.glsl"); 
    Ref<UniformBuffer> cameraBuffer = UniformBuffer::Create(sizeof(CameraBuffer), 1);
    Ref<UniformBuffer> modelBuffer = UniformBuffer::Create(sizeof(ModelData), 2);
    shader->SetUniformBufferBinding(cameraBuffer, "GameCamera");
    shader->SetUniformBufferBinding(modelBuffer, "ModelData");
    

    cameraBuffer->SetData(&cam, sizeof(cam), 0);
    modelBuffer->SetData(&model, sizeof(model), 0);
            Batch::Init();

    Ref<Mesh> cube = ObjParser::Parse("../../../TestGround/res/spruce.obj", true);


    Ref<Texture2D> albedo = Texture2D::Create("../../../TestGround/res/textures/spruce_texturing_elka_BaseColor.png");
    TextureSpecification spec = albedo->GetSpecification();
    spec.MagFilter = FilterType::LINEAR;
    spec.MinFilter = FilterType::NEAREST;
    spec.GenerateMipmaps = true;
    albedo->SetSpecification(spec);

    Ref<Texture2D> normal = Texture2D::Create("../../../TestGround/res/textures/spruce_texturing_elka_Normal.png");

    while(running)
    {
        while(!EventSystem::IsEmpty())
        {
            Event e = EventSystem::GetEvent();
            ui->OnEvent(e);
            if(e.Handled) continue;

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
            transform.Rotation.y += glm::radians(1.0f);
            model.Transform = transform.GetTransform();
            modelBuffer->SetData(&model, sizeof(glm::mat4), 0);
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

        shader->Unbind();

        //TODO: Test graphics by writing a deferred renderer

        ui->OnRender();

        window->OnUpdate();
    }
    Batch::Shutdown();
    return 0;
}