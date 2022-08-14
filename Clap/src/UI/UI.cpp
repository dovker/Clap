#include "pch.h"
#include "UI.h"

#include "Input/Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Clap
{
    Ref<Element> UI::RecursiveCollision(Ref<Element> element, glm::vec2 pos)
    {
        bool collision;
        if(element->GetParent())
            collision = element->GetAABB().Collides(element->GetParent()->GetAABB(), pos);
        else
            collision = element->GetAABB().Collides(pos);
        
        if(collision)
        {
            //If type is window, bring to front, add some sort of depth thing, maybe push_front?
            for(auto e : element->GetChildren())
            {
                if(e->IsCollidable())
                {
                    Ref<Element> collided = RecursiveCollision(e, pos);
                    if(collided)
                        return collided;
                }
            }
            return element;
        }
        else return nullptr;
    }

    void UI::RecursiveRendering(Ref<Element> element) //TODO: Chack bounds with camera
    {
        if(element->IsVisible()) element->OnRender();

        for(auto e : element->GetChildren())
        {
            if(e->IsVisible())
            {
                RecursiveRendering(e);
            }
        }
    }

    // void RecursiveInit(Ref<Element> element)
    // {
    //     for(auto e : element->GetChildren())
    //     {
    //         RecursiveInit(e);
    //     }
    //     element->OnInit();
    // }

    Ref<Element> UI::CalculateMouseCollision()
    {
        glm::vec2 mousePosition = { Input::GetMouseX(), Input::GetMouseY() };
        return RecursiveCollision(m_Beginning, mousePosition);
    }

    void UI::OnRender()
    {
        Graphics::ToggleDepthTest(false);
        Graphics::ToggleBlending(true);
        Graphics::ToggleBackfaceCulling(false);
        //Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});

        //Graphics::Clear();

        glm::mat4 cameraProjection = glm::ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, 100.0f, -100.0f);
        glm::mat4 cameraTransform = glm::mat4(1.0f);

        Batch::Begin(cameraProjection, cameraTransform);
        
        RecursiveRendering(m_Beginning);
        
        Batch::End();
    }

    void UI::OnEvent(Event& e)
    {
        switch(e.Type)
        {
            //TODO: HANDLE MOUSE BUTTONS SEPARATELY
            //TODO: CHECK IF THE EVENT WASHANDLED AND ONLY THEN PASS IT ON
            case EventType::MouseButtonPressed:
                {
                    m_PressedElement = CalculateMouseCollision();
                    if(m_PressedElement)
                    {
                        m_PressedElement->OnMouseDown();
                        m_MouseWasDown = true;
                        e.Handled = true;
                    }
                }
                break;
            case EventType::MouseButtonReleased:
                {
                    auto element = CalculateMouseCollision();
                    if(element)
                    {
                        if(element == m_PressedElement)
                            m_PressedElement->OnClicked();
                        else m_PressedElement->OnMouseUp();

                        e.Handled = true;
                    }
                    m_MouseWasDown = false;
                }
                break;
            case EventType::MouseMoved:
                {
                    auto hovered = CalculateMouseCollision();
                    if(hovered && hovered != m_HoveredElement)
                    {
                        if(m_HoveredElement)
                            m_HoveredElement->OnMouseLeft();
                        hovered->OnMouseHover();
                    }
                    m_HoveredElement = hovered;
                    //e.Handled = true;
                }
                break;
            
        }
    }

    void UI::Init()
    {
        m_Beginning = Element::Create(AABB({0.0f, 0.0f}, {m_Window->GetWidth(), m_Window->GetHeight()}));
        m_Beginning->SetVisible(false);
    }
} 
