#include "olpch.h"
#include "Network/Network.h"

#include "Layer/Layer.h"

namespace Soren {
namespace Neural {

	// Layer function implementation

	Accessor<Layer>& Network::Layers(const size_t layers, const std::string& name)
	{
		if (layers <= m_Layers.size())
		{
			m_Layers.erase(m_Layers.begin() + layers, m_Layers.end());

			m_Layers.shrink_to_fit();

			return NetLayers;
		}

		m_Layers.reserve(layers);

		if (!DefaultNodeCount)
		{
			for (size_t i = 0; i != layers; i++)
			{
				m_Layers.emplace_back(*this,name);
			}
		}
		else
		{
			for (size_t i = 0; i != layers; i++)
			{
				m_Layers.emplace_back(*this,name);
				m_Layers.back().Nodes(DefaultNodeCount);
			}

		}
		return NetLayers;
	}

	Accessor<Layer>& Network::Layers(const size_t layers, size_t nodes, const std::string& name)
	{
		if (layers <= m_Layers.size())
		{
			m_Layers.erase(m_Layers.begin() + layers, m_Layers.end());

			m_Layers.shrink_to_fit();

			return NetLayers;
		}

		m_Layers.reserve(layers);

		for (size_t i = 0; i != layers; i++)
		{
			m_Layers.emplace_back(*this,name);
			m_Layers.back().Nodes(nodes);
		}
		return NetLayers;
	}

	Accessor<Layer>& Network::AddLayers(const size_t layers, const std::string& name)
	{
		m_Layers.reserve(m_Layers.size() + layers);

		if (!DefaultNodeCount)
		{
			for (size_t i = 0; i != layers; i++)
			{
				//Threads().enqueue([&, name]() -> void
					//{
						m_Layers.emplace_back(*this,name);
					//});
			}
		}
		else
		{
			for (size_t i = 0; i != layers; i++)
			{
						m_Layers.emplace_back(*this,name);
						m_Layers.back().Nodes(DefaultNodeCount);
			}
		}
		return NetLayers;
	}

	Accessor<Layer>& Network::AddLayers(const size_t layers, const size_t nodes, const std::string& name)
	{
		m_Layers.reserve(m_Layers.size() + layers);

		for (size_t i = 0; i != layers; i++)
		{
			m_Layers.emplace_back(*this,name);
			m_Layers.back().Nodes(nodes);
		}
		return NetLayers;
	}

	Layer* Network::GetLayer(const size_t id)
	{
		auto layer = std::find(m_Layers.begin(), m_Layers.end(), id);
		return (layer == m_Layers.end()) ? nullptr : &*layer;
	}

	Layer* Network::GetLayer(const Node& node)
	{
		auto layer = std::find(m_Layers.begin(), m_Layers.end(), node);
		return (layer == m_Layers.end())? nullptr : &*layer;
	}

	Layer* Network::GetLayerPos(const size_t pos)
	{
		if (pos < m_Layers.size())
		{
			return &*(m_Layers.begin() + pos);
		}
		OL_CORE_ERROR("Position is larger than the current number of layer");
		assert(false);
		return nullptr;
	}

	size_t Network::FindLayer(const Layer& layer) const
	{
		return (size_t)std::distance(m_Layers.begin(), std::find(m_Layers.begin(), m_Layers.end(), layer));
	}

	size_t Network::FindLayer(const size_t layerid) const
	{
		return (size_t)std::distance(m_Layers.begin(), std::find(m_Layers.begin(), m_Layers.end(), layerid));
	}

	void Network::InsertLayer(const size_t pos, const std::string& name)
	{
		if (pos > m_Layers.size())
		{
			OL_CORE_ERROR("Position is greater than the total number of elements");
			assert(false);
		}

		auto insert = m_Layers.begin() + pos;
		m_Layers.emplace(insert, *this, name);

		//NetLayers.clear();
		//std::for_each(m_Layers.begin(), m_Layers.end(), NetLayers.emplace_back());
	}

	void Network::DeleteLayer(const size_t id)
	{
		std::vector<Layer>::iterator pos = std::find(m_Layers.begin(), m_Layers.end(), id);
		if (pos != m_Layers.end())
		{
			m_Layers.erase(pos);
		}

		//NetLayers.clear();
		//std::for_each(m_Layers.begin(), m_Layers.end(), NetLayers.emplace_back());

		OL_CORE_ERROR("Layer with ID: {0} not found", id);
		assert(false);
	}

	void Network::DeleteLayer(const Layer& layer)
	{
		std::vector<Layer>::iterator pos = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (pos != m_Layers.end())
		{
			m_Layers.erase(pos);
		}

		//NetLayers.clear();
		//std::for_each(m_Layers.begin(), m_Layers.end(), NetLayers.emplace_back());

		OL_CORE_ERROR("Layer not found");
		assert(false);
	}

	void Network::SetPutFunc(void(*func)(Layer& layer))
	{
		m_PutFunc = func;
	}

	void Network::SetPullFunc(void(*func)(const Layer& layer))
	{
		m_PullFunc = func;
	}

	void Network::Put(Layer& layer)
	{
		// user must ensure that the first layer has a sufficient number of node
		// or at least add a check in their input function

		// add functionality for user to access private members

		if (!m_PutFunc)
		{
			OL_CORE_ERROR("Put function uninitialized");
			assert(false);
		}

		m_PutFunc(layer);

		//if (!FindLayer(layer))
		//{
		//	if (layer.size() != InputSource().GetWidth())
		//	{
		//		layer.Nodes(InputSource().GetWidth());
		//	}

		//	InputBuffer& input = InputSource().GetBuffer()->Get();

		//	// conversion from std::vector<Input_t> to std::vector<Node_t> 
		//	layer.InBuffer.assign(input.GetData().second.begin(), input.GetData().second.end());
		//	layer.CurrentCall = input.GetData().first;

		//}
		//else
		//{
		//	layer.InBuffer = GetLayer(FindLayer(layer) - 1).OutBuffer;
		//	layer.CurrentCall = GetLayer(FindLayer(layer) - 1).CurrentCall;
		//}
	}

	void Network::Pull(const Layer& layer)
	{
		// convert from std::vector<Node_t> to std::vector<Output_t>

		// output takes in a template type then does sizeof(T)/sizeof(Output_t)

		// add functionality for user to access private members

		if (!m_PullFunc)
		{
			OL_CORE_ERROR("Pull function uninitialized");
			assert(false);
		}

		m_PullFunc(layer);
	}

} // namespace Neural
} // namespace Soren