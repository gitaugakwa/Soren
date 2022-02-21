#pragma once
#include "Core.h"
#include <cmath>
#include "Layer/layer.h"
#include "Node/node.h"

#include <functional>


namespace Soren {

	namespace Linking {

		enum class LinkingFunc
		{
			SquareConvolution
		};

		// Types

		// Square
		template <typename TCurrentValue = double, typename TCurrentBias = TCurrentValue, typename TCurrentWeight = TCurrentValue, typename TInputValue = TCurrentValue, typename TInputBias = TCurrentValue, typename TInputWeight = TCurrentValue>
		static constexpr std::function<std::vector<size_t>(std::shared_ptr<Soren::Neural::Node<TCurrentValue, TCurrentBias, TCurrentWeight>> current, Soren::Neural::Layer<TInputValue, TInputBias, TInputWeight>& inputLayer)> SquareConvolution(const size_t radius, const size_t dataWidth, const size_t dataHeight, const size_t previousLayerNodes)
		{
			return [=](std::shared_ptr<Soren::Neural::Node<TCurrentValue, TCurrentBias, TCurrentWeight>> current, Soren::Neural::Layer<TInputValue, TInputBias, TInputWeight>& inputLayer) -> std::vector<size_t> {
				dataHeight;
				size_t focus = current->id() - previousLayerNodes;

				size_t convolutionWidth = (radius * 2) + 1;

				std::vector<size_t> ids{};
				ids.reserve(convolutionWidth * convolutionWidth);


				// Square	
				if (auto pos = inputLayer.nodes().find(focus); pos != inputLayer.nodes().end()) {
					for (size_t iRad = 0; iRad < (radius * 2) + 1; iRad++) {
						if (((((long long)iRad - (long long)radius) * (long long)dataWidth) + (long long)focus) < 0) {
							continue;
						}
						size_t lineFocus = ((iRad - radius) * dataWidth) + focus;
						ids.push_back(lineFocus);
						for (size_t iDiff = 1; iDiff <= radius; iDiff++) {
							if (lineFocus - (radius * std::clamp<size_t>((lineFocus - 1) % dataWidth, 0, 1)) <= lineFocus - iDiff &&
								lineFocus - iDiff <= lineFocus + (radius * std::clamp<size_t>((lineFocus) % dataWidth, 0, 1))) {
								ids.push_back(lineFocus - iDiff);
							}
							if (lineFocus - (radius * std::clamp<size_t>((lineFocus - 1) % dataWidth, 0, 1)) <= lineFocus + iDiff &&
								lineFocus + iDiff <= lineFocus + (radius * std::clamp<size_t>((lineFocus) % dataWidth, 0, 1))) {
								ids.push_back(lineFocus + iDiff);
							}
						}
					}
				}

				return ids;
			};
		}
	}
}
