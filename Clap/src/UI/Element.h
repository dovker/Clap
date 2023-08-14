#pragma once

#include "Math/Math.h"

#include "Graphics/Batch.h"

#include "Input/Input.h"

#include <glm/glm.hpp>

namespace Clap
{
    //enum class ElementType

    class Element : public std::enable_shared_from_this<Element>
    {
    public:
        glm::vec4 TEMP_COLOR;
        Element() {}
        Element(AABB2D box) : m_AABB(box) 
        {
            GEN_TEMP_COLOR();
        }
        void GEN_TEMP_COLOR()
        {
            TEMP_COLOR = glm::vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f);
        }

        virtual void OnInit()           {};
        virtual void OnMouseHover()     { if(m_Children.size() < 2) CLAP_INFO("Hovered"); GEN_TEMP_COLOR(); }; // Hold an animation object for this
        virtual void OnMouseLeft()      { if(m_Children.size() < 2) CLAP_INFO("Left"); GEN_TEMP_COLOR(); };
        virtual void OnMouseDown()      {};
        virtual void OnMouseUp()        {};
        virtual void OnClicked()        { 
            if(m_Children.size() < 2) 
                CLAP_INFO("PRESSED, ", Input::GetMouseY()); GEN_TEMP_COLOR();
        };
        virtual void OnFocused()        {};
        virtual void OnKeyTyped(char c) {};

        void OnRender()
        {
            glm::vec2 position = m_AABB.RealPosition();
            if(m_Parent && !m_Absolute)
                position += m_Parent->GetAABB().RealPosition();
            Batch::Submit(glm::vec3(position, 0.0f), m_AABB.Size, TEMP_COLOR);//TODO: CHECK AABB AND RENDERING
        }

        void AddChild(Ref<Element> element)
        {
            m_Children.push_back(element);
            element->SetParent(shared_from_this());
        }
        void SetParent(Ref<Element> element)
        {
            m_Parent = element;
        }
        void SetVisible(bool visible)
        {
            m_Visible = visible;
        }

        std::vector<Ref<Element>>& GetChildren() { return m_Children; }
        Ref<Element> GetParent() { return m_Parent; }
        AABB2D GetAABB() { return m_AABB; }
        bool IsCollidable() { return m_Collidable; }
        bool IsAbsolute() { return m_Absolute; }
        bool IsVisible() { return m_Visible; }
        
        static Ref<Element> Create()
        {
            return CreateRef<Element>();
        }
        static Ref<Element> Create(AABB2D box)
        {
            return CreateRef<Element>(box);
        }
    protected:
        // TODO: IMPLEMENT THIS: std::string m_Name;
        Ref<Element> m_Parent;
        std::vector<Ref<Element>> m_Children;
        AABB2D m_AABB;
        bool m_Collidable = true;
        bool m_Visible = true;
        bool m_Absolute = false; //TODO: Support collisions for these
        // TODO: IMPLEMENT THIS: bool m_InteractsWithChildren
    };
}