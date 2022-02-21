#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "CallbackFunctions.h"

#include "nlohmann/json.hpp"

namespace Soren {

	template <typename T>
	class FileOutput;
	template <typename T>
	class FileInput;

	namespace Neural {

		class Event;

		struct Linkable;
		template<typename TWeight>
		class Linker;

		template<typename TValue, typename TBias, typename TWeight>
		class Node;
		template<typename TValue, typename TBias, typename TWeight>
		class Layer;

		class BaseLayer;

		class BaseNetwork : public Linkable {

			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Layer;
		public:

			BaseNetwork(): mId{ sId++ } {};
			~BaseNetwork() {};

			inline size_t id() const override { return mId; }

			// Special member functions
			#pragma region
			BaseNetwork(const BaseNetwork&)
				: mId(sId++) {};
			BaseNetwork(BaseNetwork&& arg) noexcept
				: mId(std::exchange(arg.mId, 0)) {};
			BaseNetwork& operator=(const BaseNetwork&) {
				mId = sId++;
				return *this;
			};
			BaseNetwork& operator=(BaseNetwork&& arg) noexcept {
				mId = std::move(arg.mId);
				return *this;
			};
			#pragma endregion

		protected:
			inline static size_t sId = 1;
			size_t mId;
		};

		template<typename TValue = double, typename TBias = TValue, typename TWeight = TValue>
		class SOREN_API Network : public EventEmmiter, public BaseNetwork// Should be NetworkEvent
		{
			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Network;

		public:

			Network()
				//: mId{sId++}
			{
				NetworkCreatedEvent event{};
				Emit(event);
			}
			~Network() {
				NetworkDeletedEvent event{};
				Emit(event);
			};

			//inline size_t id() const override { return mId; }
			//inline TValue value() const { return mValue; }
			//inline TBias bias() const { return mBias; }
			inline const std::map<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>& layers() const { return mLayers; }
			inline bool running() const { return mRunning; }
			inline bool locked() const { return mLocked; }


			void clear() {
				if (mLocked) return;

				mLayers.clear();

				NetworkClearedEvent event{};
				Emit(event);

				// Reset Activation function if stored by Link
			};

			void halt() {

				//const running = m_Running;
				mRunning = FALSE;

				NetworkDisabledEvent event{};
				Emit(event);

			};
			void resume() {
				mRunning = TRUE;

				NetworkEnabledEvent event{};
				Emit(event);
			};

			void reset() {
				NetworkResetEvent event{};
				Emit(event);
			};

			// Layer
			//template<typename TValue=double, typename TBias=double, typename TWeight= double>
			inline Network& addLayer(Layer<TValue, TBias, TWeight>&& layer) {
				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					(pos->second)->link(layer);
				}
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace(std::make_pair<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer) ));
				return *this;
			}
			//template<typename TValue=double, typename TBias=double, typename TWeight= double>
			inline Network& addLayer(Layer<TValue, TBias, TWeight>&& layer, std::function<bool(BaseNode& current, BaseNode& input)> shouldLink) {
				layer.setShouldLink(shouldLink);
				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					static_cast<Layer<>*>((pos->second))->link(*static_cast<Layer<TValue, TBias, TWeight>*>(&layer));
				}
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace(std::make_pair<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer)));
				return *this;
			}
			//template<typename TValue=double, typename TBias=double, typename TWeight= double>
			inline Network& addLayer(Layer<TValue, TBias, TWeight>&& layer, std::function<TWeight(void)> weightGenerator) {
				layer.setWeightGenerator(weightGenerator);
				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					(pos->second)->link(layer);
				}
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace(std::make_pair<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer)));
				return *this;
			}
			//template<typename TValue=double, typename TBias=double, typename TWeight= double>
			inline Network& addLayer(Layer<TValue, TBias, TWeight>&& layer, std::function<std::vector<size_t>(std::shared_ptr<Node<TValue, TBias, TWeight>> current, Layer<TValue, TBias, TWeight>& inputLayer)> linkGenerator, std::function<TWeight(void)> weightGenerator) {
				layer.setLinkGenerator(linkGenerator);
				layer.setWeightGenerator(weightGenerator);
				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					(pos->second)->link(layer);
				}
				//auto layer = new Layer(std::move(layer));
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace(std::make_pair<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer)));
				return *this;
			}

			// Eigen

			inline Eigen::VectorX<typename std::common_type<TValue, TBias>::type> resolveMatrix() {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					for (size_t iLayers = 0; iLayers < mLayers.size() && std::distance(pos, mLayers.end()) > 1; iLayers++, pos++) {
						(pos->second)->pipeMatrix(*(std::next(pos, 1)->second));
					}
					return (pos->second)->outputMatrix();
				}
				throw std::range_error("No Layers in network");
			}

			// Basic
			#pragma region
			template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::vector<TInputValue>& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					pos->second->input(value);
				}
				return (*this);
			}
			template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::vector<TInputValue>&& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					(pos->second)->input(value);
				}
				return (*this);
			}
			template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::map<size_t, TInputValue>& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					pos->second->input(value);
				}
				return *this;
			}
			template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::map<size_t, TInputValue>&& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					pos->second->input(value);
				}
				return *this;
			}

			template<typename TValue>
			inline std::vector<TValue> output() const {

				if (auto pos = mLayers.rbegin(); pos != mLayers.end()) {
					return pos->second.output();
				}
				throw std::range_error("No Layers in network");
				
			}

			template<typename TLinkNetworkValue>
			std::map<size_t, TLinkNetworkValue> resolveMap() {

				std::map<size_t, TLinkNetworkValue> data;

				for (auto& [node, links] : mLinker.links()) {
					for (auto& [linkedNode, link] : links) {
						data[linkedNode.get().id()] += static_cast<TLinkNetworkValue>(link.pipe<TValue>(dynamic_cast<Node<TValue, TBias, TWeight>*>(&(node.get()))->output()));
					}
				}

				for (auto& [node, links] : Network.mLinker.links()) {
					for (auto& [linkedNode, link] : links) {
						auto pos = mNodes.find(linkedNode.get().id());
						if (pos != mNodes.end()) {
							data[node.get().id()] += static_cast<TLinkNetworkValue>(link.pipe<TLinkNetworkValue>(dynamic_cast<Node<TLinkNetworkValue, TLinkNetworkBias, TLinkNetworkWeight>*>(&(linkedNode.get()))->output()));
						}
					}
				}

				return data;
			}

			std::vector<typename std::common_type<TValue, TBias>::type> resolve() {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					for (size_t iLayers = 0; iLayers < mLayers.size() && std::distance(pos, mLayers.end()) > 1; iLayers++, pos++) {
						(pos->second)->pipe(*(std::next(pos, 1)->second));
					}
					return (pos->second)->output();
				}
				throw std::range_error("No Layers in network");
			}

			//template <typename TPipeNetwork>
			Network& pipe(Network& network) {
				network.input(resolve());
				return network;
			}
			#pragma endregion

			// Operators

			template<typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
			friend Network& operator>>(std::vector<T> lhs, Network& rhs) {
				rhs.input(lhs);
				return rhs;
			}
			inline Network& operator>>(Network& rhs) {
				return pipe(rhs);
			}



		private:

			// Events
			#pragma region
			//inline void Bind() { SetEventCallbackFunction(BIND_EVENT_FN(Link::OnEvent)); }

			void OnEvent(const Soren::Event& e) {
				if (!mRunning) return;
				EventDispatcher dispatcher(e);

				//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Network::Init)); <- Might be virtuals
				//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
				//SOREN_CORE_TRACE(e);
				return;

			};
			#pragma endregion

			//size_t mId;

			bool mRunning = true; // So as to allow links to be disabled and enabled
			bool mLocked = false;

			//std::function<TValue(const TValue)> mActivation = Activation::Identity<TValue>;

			std::map<size_t, std::shared_ptr<Layer<TValue,TBias,TWeight>>> mLayers{};

		};


	} // namespace Neural
} // namespace Soren