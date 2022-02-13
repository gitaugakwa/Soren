#pragma once
#include <vector>

#include "Core.h"
#include "Base.h"
#include "CallbackFunctions.h"

#include "Log/Log.h"

#include "Events/Event.h"

namespace Soren {
namespace Neural {

	struct DataBuffer {
		struct DataElement {
			size_t NodeID = 0;
			Node_t NodeValue = 0;

			DataElement() = default;
			DataElement(const size_t node, const Node_t value)
				: NodeID(node), NodeValue(value) {}


			bool operator==(const DataElement& rhs) const
			{
				return (NodeID == rhs.NodeID &&
					NodeValue == rhs.NodeValue);
			}
			bool operator==(size_t rhs) const
			{
				return NodeID == rhs;
			}
			bool operator!=(size_t rhs) const
			{
				return !((*this) == rhs);
			}
		};

		std::vector<DataElement> data;
		bool initialized = false;

		bool empty() const
		{
			return data.empty();
		}

		size_t size() const
		{
			return data.size();
		}

		void reserve(size_t size)
		{
			return data.reserve(size);
		}

		void resize(size_t size)
		{
			return data.resize(size);
		}

		template <typename ...Args>
		DataElement& emplace_back(Args... args)
		{
			return data.emplace_back(std::forward<Args>(args)...);
		}

		// should this be for nodeid or pos in the vector. I dont think position maters. maybe, but what about convoluted
		// maybe position is at(unsigned int pos)???
		// elements to be placed according to position of nodes
		const DataElement& at(size_t pos) const
		{
			return data.at(pos);
		}

		DataElement& at(size_t pos)
		{
			return data.at(pos);
		}

		const DataElement& operator[](size_t rhs) const
		{
			return *std::find(data.begin(), data.end(), rhs);
		}

		DataElement& operator[](size_t rhs)
		{
			return *std::find(data.begin(), data.end(), rhs);
		}

		bool operator==(const DataBuffer& rhs) const
		{
			return data == rhs.data;
		}

		bool operator!=(const DataBuffer& rhs) const
		{
			return !(*this == rhs);
		}
	};

	struct LayerCallbacks {
		LayerEventfunc	created{};
		LayerEventfunc	deleted{};
		LayerEventfunc	reset{};
		LayerEventfunc	disabled{};
		LayerEventfunc	enabled{};
		LayerEventfunc	cleared{};
		LayerEventfunc	copied{};
		LayerEventfunc	copyassign{};
		LayerEventfunc	moved{};
		LayerEventfunc	moveassign{};
		LayerEventfunc	active{};
		LayerEventfunc	inactive{};

		LayerCallbacks() {
			Default();
		}
		void Default() {
			created			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			deleted			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			reset			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			disabled		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			enabled			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			cleared			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			copied			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			copyassign		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			moved			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			moveassign		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			active			= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
			inactive		= [](const Soren::Event& e) { OL_CORE_TRACE(e); };
		}
		bool operator==(const LayerCallbacks& rhs) const {
			return (created		== rhs.created &&
					deleted		== rhs.deleted &&
					reset		== rhs.reset &&
					disabled	== rhs.disabled &&
					enabled		== rhs.enabled &&
					cleared		== rhs.cleared &&
					copied		== rhs.copied &&
					copyassign	== rhs.copyassign &&
					moved		== rhs.moved &&
					moveassign	== rhs.moveassign &&
					active		== rhs.active &&
					inactive	== rhs.inactive);
		}
	};

	struct LayerEvents{
		std::vector<LayerEventfunc>	created{};
		std::vector<LayerEventfunc>	deleted{};
		std::vector<LayerEventfunc>	reset{};
		std::vector<LayerEventfunc>	disabled{};
		std::vector<LayerEventfunc>	enabled{};
		std::vector<LayerEventfunc>	cleared{};
		std::vector<LayerEventfunc>	copied{};
		std::vector<LayerEventfunc>	copyassign{};
		std::vector<LayerEventfunc>	moved{};
		std::vector<LayerEventfunc>	moveassign{};
		std::vector<LayerEventfunc>	active{};
		std::vector<LayerEventfunc>	inactive{};

		bool operator==(const LayerEvents& rhs) const {
			return (created		== rhs.created &&
					deleted		== rhs.deleted &&
					reset		== rhs.reset &&
					disabled	== rhs.disabled &&
					enabled		== rhs.enabled &&
					cleared		== rhs.cleared &&
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