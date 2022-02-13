#pragma once
#include <vector>
#include <variant>
#include <string>
#include <filesystem>

#include "Core.h"
#include "Base.h"
#include "Type.h"
#include "CallbackFunctions.h"

#include "Output/Output.h"
#include "Input/Input.h"

#include "Log/Log.h"

#include "Events/Event.h"

namespace Soren {
	namespace Neural {

		struct Bias
		{
			bool UpInit = false;
			Bias_t UpLimit = 0;
			bool DownInit = false;
			Bias_t DownLimit = 0;
		};


		struct Weight
		{
			bool UpInit = false;
			Weight_t UpLimit = 0;
			bool DownInit = false;
			Weight_t DownLimit = 0;
		};

		struct NetworkCallbacks {
			NetworkEventfunc	initialized{};
			NetworkEventfunc	created{};
			NetworkEventfunc	deleted{};
			NetworkEventfunc	reset{};
			NetworkEventfunc	cleared{};
			NetworkEventfunc	disabled{};
			NetworkEventfunc	enabled{};
			NetworkEventfunc	copied{};
			NetworkEventfunc	copyassign{};
			NetworkEventfunc	moved{};
			NetworkEventfunc	moveassign{};
			NetworkEventfunc	active{};
			NetworkEventfunc	inactive{};
			NetworkEventfunc	itstart{};
			NetworkEventfunc	itstop{};
			NetworkEventfunc	statuschange{};

			NetworkCallbacks() {
				Default();
			}
			void Default() {
				initialized		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
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
				itstart			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
				itstop			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
				statuschange	= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			}
			bool operator==(const NetworkCallbacks& rhs) const {
				return (initialized == rhs.initialized &&
					created == rhs.created &&
					deleted == rhs.deleted &&
					reset == rhs.reset &&
					cleared == rhs.cleared &&
					disabled == rhs.disabled &&
					enabled == rhs.enabled &&
					copied == rhs.copied &&
					copyassign == rhs.copyassign &&
					moved == rhs.moved &&
					moveassign == rhs.moveassign &&
					active == rhs.active &&
					inactive == rhs.inactive &&
					itstart == rhs.itstart &&
					itstop == rhs.itstop &&
					statuschange == rhs.statuschange);
			}
		};

		struct NetworkEvents {
			std::vector<NetworkEventfunc>	initialized{};
			std::vector<NetworkEventfunc>	created{};
			std::vector<NetworkEventfunc>	deleted{};
			std::vector<NetworkEventfunc>	reset{};
			std::vector<NetworkEventfunc>	cleared{};
			std::vector<NetworkEventfunc>	disabled{};
			std::vector<NetworkEventfunc>	enabled{};
			std::vector<NetworkEventfunc>	copied{};
			std::vector<NetworkEventfunc>	copyassign{};
			std::vector<NetworkEventfunc>	moved{};
			std::vector<NetworkEventfunc>	moveassign{};
			std::vector<NetworkEventfunc>	active{};
			std::vector<NetworkEventfunc>	inactive{};
			std::vector<NetworkEventfunc>	itstart{};
			std::vector<NetworkEventfunc>	itstop{};
			std::vector<NetworkEventfunc>	statuschange{};

			void clear() {
				initialized.clear();
				created.clear();
				deleted.clear();
				reset.clear();
				cleared.clear();
				disabled.clear();
				enabled.clear();
				copied.clear();
				copyassign.clear();
				moved.clear();
				moveassign.clear();
				active.clear();
				inactive.clear();
				itstart.clear();
				itstop.clear();
				statuschange.clear();
			}
			bool operator==(const NetworkEvents& rhs) const {
				return (initialized == rhs.initialized &&
					created == rhs.created &&
					deleted == rhs.deleted &&
					reset == rhs.reset &&
					cleared == rhs.cleared &&
					disabled == rhs.disabled &&
					enabled == rhs.enabled &&
					copied == rhs.copied &&
					copyassign == rhs.copyassign &&
					moved == rhs.moved &&
					moveassign == rhs.moveassign &&
					active == rhs.active &&
					inactive == rhs.inactive &&
					itstart == rhs.itstart &&
					itstop == rhs.itstop &&
					statuschange == rhs.statuschange);
			}
		};

	} // namespace Neural

} // namespace Soren