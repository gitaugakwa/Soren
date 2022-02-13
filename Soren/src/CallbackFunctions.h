#pragma once

class Soren::Event;

// Network Callback
typedef void (*NetworkEventfunc)(const Soren::Event&);

// Layer Callback
typedef void (*LayerEventfunc)(const Soren::Event&);

// Node Callback
typedef void (*NodeEventfunc)(const Soren::Event&);

// Link Callback
typedef void (*LinkEventfunc)(const Soren::Event&);

