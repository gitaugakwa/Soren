#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"
//#include "Eigen/Core"
//#include "Optimizer/Optimizer.h"

#include "nlohmann/json.hpp"
#pragma warning( disable : 5054 ) // Disable Warning C5054: operator '&': deprecated between enumerations of different types


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

		class BaseNetwork  {

			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Layer;
		public:

			BaseNetwork(): mId{ sId++ } {};
			~BaseNetwork() {};

			inline size_t id() const { return mId; }

			// Special member functions
			#pragma region
			BaseNetwork(const BaseNetwork&)
				: mId(sId++) {};
			BaseNetwork(BaseNetwork&& arg) noexcept
				: mId(std::exchange(arg.mId, 0)) {};
			BaseNetwork& operator=(const BaseNetwork&) {
				//mId = sId++;
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
		class SOREN_API Network : public EventEmmiter, public BaseNetwork // Should be NetworkEvent
		{
			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Network;
			//template<typename TValue, typename TBias, typename TWeight>
			//friend class Optimizer::OptimizerBase<TValue, TBias, TWeight>;

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
			inline const std::vector<std::shared_ptr<Layer<TValue, TBias, TWeight>>>& layers() const { return mLayers; }
			inline const size_t size() const { return mLayers.size(); }
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
				/*if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					(*pos)->link(layer);
				}*/
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace_back(std::shared_ptr<Layer<TValue, TBias, TWeight>>(std::move(pLayer)));
				return *this;
			}
			//template<typename TValue=double, typename TBias=double, typename TWeight= double>
			inline Network& addLayer(Layer<TValue, TBias, TWeight>&& layer, typename Layer<TValue, TBias, TWeight>::LinkGenerator linkGenerator) {
				layer.setLinkGenerator(linkGenerator);
				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					(*pos)->link(layer);
				}
				auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(std::move(layer));
				mLayers.emplace_back(std::shared_ptr<Layer<TValue, TBias, TWeight>>(std::move(pLayer)));
				return *this;
			}

			// Eigen

			inline Eigen::VectorX<typename std::common_type<TValue, TBias>::type> resolveMatrix() {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					for (size_t iLayers = 0; iLayers < mLayers.size() && std::distance(pos, mLayers.end()) > 1; iLayers++, pos++) {
						(*pos)->pipeMatrix(*(std::next(pos, 1)));
					}
					return (*pos)->outputMatrix();
				}
				throw std::range_error("No Layers in network");
			}

			// Basic
			#pragma region
			//template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::vector<TValue>& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					(*pos)->input(value);
				}
				return (*this);
			}
			//template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(std::vector<TValue>&& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					(*pos)->input(value);
				}
				return (*this);
			}
			//template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(const Eigen::Ref<const Eigen::MatrixX<TValue>>& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					(*pos)->input(value);
				}
				return *this;
			}
			//template<typename TInputValue, std::enable_if_t<std::is_integral<TInputValue>::value || std::is_floating_point<TInputValue>::value, int> = 0>
			inline Network& input(const Eigen::MatrixX<TValue>&& value) {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					(*pos)->input(value);
				}
				return *this;
			}

			//template<typename TValue>
			inline Eigen::MatrixX<TValue> output() const {

				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					return (*pos)->output();
				}
				throw std::range_error("No Layers in network");
				
			}
			/*inline Eigen::Vector<TValue, Eigen::Dynamic> output() const {

				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					return (*pos)->output();
				}
				throw std::range_error("No Layers in network");
				
			}
			inline Eigen::VectorX<TValue> outputMatrix() const {

				if (auto pos = mLayers.rbegin(); pos != mLayers.rend()) {
					return (*pos)->outputMatrix();
				}
				throw std::range_error("No Layers in network");
				
			}*/
			Network<TValue, TBias,TWeight>& resolve() {
				if (auto pos = mLayers.begin(); pos != mLayers.end()) {
					size_t size = mLayers.size();
					for (size_t iLayers = 0; iLayers < size - 1; iLayers++) {
						(*pos)->pipe(**((++pos)));
					}
					(*pos)->resolve();
					return (*this);
				}
				throw std::range_error("No Layers in network");
			}

			//template <typename TPipeNetwork>
			Network& pipe(Network& network) {
				network.input(resolve());
				return network;
			}

			void backPropagate(Eigen::Vector<TValue, Eigen::Dynamic> derivatives) {
				auto pos = mLayers.rbegin();
				(*pos)->backPropagate(derivatives, **(std::next(pos, 1)));
				pos++;
				for (; pos != std::prev(mLayers.rend(),1); pos++) {
					// Prevent backprop to layers with no links
					(*pos)->backPropagate((*std::prev(pos, 1))->mDLossDInput, **(std::next(pos, 1)));
				}
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

			// Special member functions
			#pragma region
			Network(const Network& arg)
				: BaseNetwork(arg),
				mLayers({}),
				mRunning(arg.mRunning),
				mLocked(arg.mLocked)
			{
				if (size_t size = mLayers.size(); size == arg.mLayers.size()) {
					auto curPos = mLayers.begin();
					auto argPos = arg.mLayers.begin();
					for (size_t iLayer = 0; iLayer < size; ++iLayer, ++curPos, ++argPos) {
						*(curPos) = *(argPos);
					}
				}
				else {
					std::transform(arg.mLayers.begin(), arg.mLayers.end(), std::inserter(mLayers, mLayers.end()),
						[](auto layerPair) {
							auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(*(layerPair.second));
							return std::make_pair<size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer));
						});
				}
				if (arg.mLayers.size() > 1) {
					// Relink copied layers
					auto argPos = std::next(arg.mLayers.begin(), 1);
					auto curPos = std::next(mLayers.begin(), 1);
					for (size_t iLayer = 1; iLayer < arg.mLayers.size(); iLayer++, argPos++, curPos++) {
						auto& argPrevNode = std::prev(argPos, 1)->mNodes;
						auto argPrevNodePos = argPrevNode.begin();
						auto& argNode = argPos->mNodes;
						auto& argLinks = argPos->mLinker.mLinks;
						auto argNodePos = argNode.begin();
						auto& curPrevNode = std::prev(curPos, 1)->mNodes;
						auto curPrevNodePos = curPrevNode.begin();
						auto& curNode = curPos->mNodes;
						auto& curLinks = curPos->mLinker.mLinks;
						auto curNodePos = curNode.begin();

						for (auto& [linked, links] : argLinks) {
							// Find position of node in the original layer
							auto distance = std::distance(argNodePos, argNode.find(linked->id()));
							// Get the node in the same position in the coppied layer
							auto copiedNode = std::next(curNodePos, distance);
							for (auto& [lowerLinked, link] : links) {
								auto innerDistance = std::distance(argPrevNodePos, argPrevNode.find(lowerLinked->id()));
								auto copiedInnerNode = std::next(curPrevNodePos, innerDistance);
								curLinks[copiedNode][copiedInnerNode] = link;
							}
						}
					}
				}


				NetworkCopiedEvent event{ arg.mId, mId };
				Emit(event);
			};
			Network(Network<TValue, TBias, TWeight>&& arg) noexcept
				: BaseNetwork(std::move(arg)),
				mLayers(std::exchange(arg.mLayers, {})),
				mRunning(std::exchange(arg.mRunning, false)),
				mLocked(std::exchange(arg.mLocked, true))
			{
				NetworkMovedEvent event{ mId };
				Emit(event);
			};
			Network& operator=(const Network& arg) {
				BaseNetwork::operator=(arg);
					//if (*this == arg) return *this;
				if (size_t size = mLayers.size(); size == arg.mLayers.size()) {
					auto curPos = mLayers.begin();
					auto argPos = arg.mLayers.begin();
					for (size_t iLayer = 0; iLayer < size; ++iLayer, ++curPos, ++argPos) {
						*(curPos) = *(argPos);
					}
				}
				else {
					std::transform(arg.mLayers.begin(), arg.mLayers.end(), std::inserter(mLayers, mLayers.end()),
						[](auto& layerPair) {
							auto pLayer = std::make_shared<Layer<TValue, TBias, TWeight>>(*(layerPair.second));
							return std::make_pair<const size_t, std::shared_ptr<Layer<TValue, TBias, TWeight>>>(pLayer->id(), std::move(pLayer));
						});
				}

				if (arg.mLayers.size() > 1) {
					// Relink copied layers
					auto argPos = std::next(arg.mLayers.begin(), 1);
					auto curPos = std::next(mLayers.begin(), 1);
					for (size_t iLayer = 1; iLayer < arg.mLayers.size(); iLayer++, argPos++, curPos++) {
						auto& argPrevNode = std::prev(argPos, 1)->mNodes;
						auto argPrevNodePos = argPrevNode.begin();
						auto& argNode = argPos->mNodes;
						auto& argLinks = argPos->mLinker.mLinks;
						auto argNodePos = argNode.begin();
						auto& curPrevNode = std::prev(curPos, 1)->mNodes;
						auto curPrevNodePos = curPrevNode.begin();
						auto& curNode = curPos->mNodes;
						auto& curLinks = curPos->mLinker.mLinks;
						auto curNodePos = curNode.begin();

						for (auto& [linked, links] : argLinks) {
							// Find position of node in the original layer
							auto distance = std::distance(argNodePos, argNode.find(linked->id()));
							// Get the node in the same position in the coppied layer
							auto copiedNode = std::next(curNodePos, distance);
							for (auto& [lowerLinked, link] : links) {
								auto innerDistance = std::distance(argPrevNodePos, argPrevNode.find(lowerLinked->id()));
								auto copiedInnerNode = std::next(curPrevNodePos, innerDistance);
								curLinks[copiedNode][copiedInnerNode] = link;
							}
						}
					}
				}

				mRunning = arg.mRunning;
				mLocked = arg.mLocked;

				NetworkCopyAssignedEvent event{};
				Emit(event);

				return *this;
			};
			Network& operator=(Network&& arg) noexcept {
				BaseNetwork::operator=(std::move(arg));

				mLayers = std::move(arg.mLayers);
				mRunning = std::move(arg.mRunning);
				mLocked = std::move(arg.mLocked);

				NetworkMoveAssignedEvent event{};
				Emit(event);
				return *this;
			};
			#pragma endregion



		protected:

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

			std::vector<std::shared_ptr<Layer<TValue,TBias,TWeight>>> mLayers{};

		};


	} // namespace Neural
} // namespace Soren

#pragma warning( default : 5054 ) // Reenable Warning C5054: 'zero': operator '&': deprecated between enumerations of different types
