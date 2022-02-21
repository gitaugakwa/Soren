#include "sorenpch.h"
#include "Application.h"

#include "Core.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/NetworkEvent.h"
#include "Events/LayerEvent.h"
#include "Events/NodeEvent.h"
#include "Events/LinkEvent.h"

namespace Soren {

//#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		//for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		//{
		//	(*--it)->OnEvent(e);
		//	if (e.Handled)
		//		break;
		//}
	}
}
