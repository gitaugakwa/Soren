#pragma once
#include <vector>

#include "Core.h"
#include "Base.h"
#include "CallbackFunctions.h"

#include "Log/Log.h"

#include "Events/Event.h"



namespace Soren {
namespace Neural {

	struct LinkEvents {
			std::vector<LinkEventfunc>	created{};
			std::vector<LinkEventfunc>	deleted{};
			std::vector<LinkEventfunc>	reset{};
			std::vector<LinkEventfunc>	cleared{};
			std::vector<LinkEventfunc>	disabled{};
			std::vector<LinkEventfunc>	enabled{};
			std::vector<LinkEventfunc>	copied{};
			std::vector<LinkEventfunc>	copyassign{};
			std::vector<LinkEventfunc>	moved{};
			std::vector<LinkEventfunc>	moveassign{};
			std::vector<LinkEventfunc>	active{};
			std::vector<LinkEventfunc>	inactive{};

			bool operator==(const LinkEvents& rhs) const {
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

	struct LinkCallbacks {
			LinkEventfunc	created{};
			LinkEventfunc	deleted{};
			LinkEventfunc	reset{};
			LinkEventfunc	cleared{};
			LinkEventfunc	disabled{};
			LinkEventfunc	enabled{};
			LinkEventfunc	copied{};
			LinkEventfunc	copyassign{};
			LinkEventfunc	moved{};
			LinkEventfunc	moveassign{};
			LinkEventfunc	active{};
			LinkEventfunc	inactive{};

			LinkCallbacks() {
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
			bool operator==(const LinkCallbacks& rhs) const {
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