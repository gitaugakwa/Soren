#pragma once
#include <vector>

#include "Core.h"
#include "Base.h"
#include "CallbackFunctions.h"

#include "Log/Log.h"

#include "Events/Event.h"

namespace Soren {
namespace Neural {

	struct NodeCallbacks{
		NodeEventfunc	created{};
		NodeEventfunc	deleted{};
		NodeEventfunc	reset{};
		NodeEventfunc	cleared{};
		NodeEventfunc	disabled{};
		NodeEventfunc	enabled{};
		NodeEventfunc	copied{};
		NodeEventfunc	copyassign{};
		NodeEventfunc	moved{};
		NodeEventfunc	moveassign{};
		NodeEventfunc	active{};
		NodeEventfunc	inactive{};

		NodeCallbacks() {
			Default();
		}
		void Default() {
			created			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			deleted			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			reset			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			cleared			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			disabled		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			enabled			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			copied			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			copyassign		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			moved			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			moveassign		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			active			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			inactive		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
		}
		bool operator==(const NodeCallbacks& rhs) const {
			return (created		== rhs.created &&
					deleted		== rhs.deleted &&
					reset		== rhs.reset &&
					cleared		== rhs.cleared &&
					disabled	== rhs.disabled &&
					enabled		== rhs.enabled &&
					copied		== rhs.copied &&
					copyassign	== rhs.copyassign &&
					moved		== rhs.moved &&
					moveassign	== rhs.moveassign &&
					active		== rhs.active &&
					inactive	== rhs.inactive);
		}
	};

	struct NodeEvents{
		std::vector<NodeEventfunc>	created{};
		std::vector<NodeEventfunc>	deleted{};
		std::vector<NodeEventfunc>	reset{};
		std::vector<NodeEventfunc>	cleared{};
		std::vector<NodeEventfunc>	disabled{};
		std::vector<NodeEventfunc>	enabled{};
		std::vector<NodeEventfunc>	copied{};
		std::vector<NodeEventfunc>	copyassign{};
		std::vector<NodeEventfunc>	moved{};
		std::vector<NodeEventfunc>	moveassign{};
		std::vector<NodeEventfunc>	active{};
		std::vector<NodeEventfunc>	inactive{};

		bool operator==(const NodeEvents& rhs) const {
			return (created		== rhs.created &&
					deleted		== rhs.deleted &&
					reset		== rhs.reset &&
					cleared		== rhs.cleared &&
					disabled	== rhs.disabled &&
					enabled		== rhs.enabled &&
					copied		== rhs.copied &&
					copyassign	== rhs.copyassign &&
					moved		== rhs.moved &&
					moveassign	== rhs.moveassign &&
					active		== rhs.active &&
					inactive	== rhs.inactive);
		}
	};

}
}
