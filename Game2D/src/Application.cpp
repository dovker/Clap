#include <glm/glm.hpp>

#include "Clap.h"

using namespace Clap;

namespace Game2D
{
    void Start()
    {
        //EventSystem::AttatchCallback(&OnEvent);
        bool running = true;

        Window* window = Window::Create();

        Transform cameraTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 projectionMat = glm::ortho(-1280.0f * 0.5f, 1280.0f * 0.5f, 720.0f * 0.5f, -720.0f * 0.5f, 100.0f, -100.0f);

        Graphics::Init();
        Batch::Init();


        Ref<Texture2D> albedo = Texture2D::Create("../../Game2D/res/textures/awesomeface.png");

        Transform transform(glm::vec3(0.0f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 1.0f));
        Transform transform2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(100.0f, 100.0f, 100.0f));

        // layout (location = 0) in vec3 aPos;
        // layout (location = 1) in vec4 aColor;
        // layout (location = 2) in vec2 aTexCoords;
        // layout (location = 3) in float aTexIndex;

        float vertices[]
        {
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0,
            0.0f, 100.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0,
            100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
            100.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0
        };
        uint32_t indices[]
        {
            0, 1, 2, 2, 3, 0
        };

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
            if(Input::IsKeyPressed(KEY_A))
            {
                cameraTransform.Position.x -=0.1;
            }
            if(Input::IsKeyPressed(KEY_D))
            {
                cameraTransform.Position.x +=0.1;
            }
            if(Input::IsKeyPressed(KEY_W))
            {
                cameraTransform.Position.y -=0.1;
            }
            if(Input::IsKeyPressed(KEY_S))
            {
                cameraTransform.Position.y +=0.1;
            }
            Graphics::ToggleDepthTest(true);
            Graphics::ToggleBlending(true);
            Graphics::SetClearColor({0.4f, 0.3f, 0.6f, 1.0f});
            Graphics::Clear();

            Batch::Begin(projectionMat, cameraTransform.GetTransform());

            Batch::Submit(transform.GetTransform(), albedo);
            Batch::Submit(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec2(100.0f, 100.0f), albedo, {0.0f, 0.0f, 1.0f, 1.0f}, glm::vec4(1.0f));
            Batch::Submit(transform2.GetTransform(), {1.0f, 1.0f, 1.0f, 1.0f});

            Batch::End();

            window->OnUpdate();
        }
        Batch::Shutdown();
    }


}

int main()
{
    Game2D::Start();
    return 0;
}