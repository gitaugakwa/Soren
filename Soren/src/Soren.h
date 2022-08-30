#pragma once

//#define EIGEN_USE_MKL_ALL

#include "sorenpch.h"

#include "Base.h"

#include "Input/Input.h"
#include "Output/Output.h"

// Events
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/LayerEvent.h"
#include "Events/LinkEvent.h"
#include "Events/MouseEvent.h"
#include "Events/NetworkEvent.h"
#include "Events/NodeEvent.h"

// Base Neural
#include "Link/Link.h"
#include "Node/Node.h"
#include "Layer/Layer.h"
#include "Network/Network.h"
#include "Application/Application.h"


// Async Neural
#include "Layer/LayerAsync.h"
#include "Network/NetworkAsync.h"

#include "Model/Model.h"

#include "Test/Test.h"
#include "Train/Train.h"
#include "Train/TrainAsync.h"

//#include "dnnUtils.h"


//	---Entry Point-----------------------
#include "EntryPoint.h"
//	-------------------------------------
