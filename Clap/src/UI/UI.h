#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

#include "Events/Event.h"

#include "Element.h"

#include <glm/glm.hpp>

namespace Clap
{
    class UI
    {
    public:
        UI(Window* window) : m_Window(window) {}
        void Init();
        
        void OnRender();
        void OnEvent(Event& e);

        void AttachElement(Ref<Element> element) { m_Beginning->AddChild(element); }

        Ref<Element> CalculateMouseCollision();

        static Ref<UI> Create(Window* window)
        {
            return CreateRef<UI>(window);
        }

    private:
        //Ref<LuaState>
        Ref<Element> m_Beginning; //Decide how to store elements, maybe every element is the same size, but stored differently
        Ref<Element> m_Selected;
        Ref<Element> m_Subject;
        Window* m_Window;

    private:
        bool m_MouseWasDown = false;
        Ref<Element> m_PressedElement;
        Ref<Element> m_HoveredElement;

        Ref<Element> RecursiveCollision(Ref<Element> element, glm::vec2 pos);
        void RecursiveRendering(Ref<Element> element);
    };
}