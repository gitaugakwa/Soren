#pragma once

#include <string>

enum class StatusCategory
{
	None = 0,
	Initialized, Uninitialized,

	Training, Testing, Release,
	
	Idle,

	Inputing, Outputing
};

enum class InputStatusCategory
{
	None = 0,
	Active, Inactive,

	Accessed, Inaccessed,

	Idle, Retrieving
};

enum class OutputStatusCategory
{
	None = 0,
	Active, Inactive,

	Accessed, Inaccessed,

	Idle, Outputing
};

struct NetworkStatus
{
private:
	bool Initialized: 1; // State whether the network has been initialized or not
	bool Training	: 1; // State that weights and biases are being modified
	bool Testing	: 1; // State that no data is modified and is used when determining the accuracy
	bool Release	: 1; // State that no data is modified and is the final version of the network
	bool Idle		: 1; // State that the network is currently doing nothing
	bool Inputing	: 1; // State where data form an outside source is being captured in order to be processed
	bool Outputing	: 1; // State where data is streamed out in order to save or give error data

public:

	NetworkStatus();
	void SetStatus(const StatusCategory& status);
	std::string str() const;
	bool IsInCategory(const std::initializer_list<StatusCategory>& status) const;
	bool IsInCategory(const StatusCategory& status) const;

	bool operator==(const NetworkStatus& rhs) const {
		return ((*this).Training == rhs.Training &&
				(*this).Testing == rhs.Testing &&
				(*this).Release == rhs.Release &&
				(*this).Idle == rhs.Idle &&
				(*this).Inputing == rhs.Inputing &&
				(*this).Outputing == rhs.Outputing);
	}
};

struct InputStatus
{
private:
	bool Active		: 1; // State when a stream is open to an input file
	bool Inactive	: 1; // State when no stream is open
	bool Accessed	: 1; // State when Data from the stream is being used by the network
	bool Inaccessed	: 1; // State when no Data from the stream is in use
	bool Idle		: 1; // State where no stream is open and no data is being accessed or retrieved
	bool Retrieving	: 1; // State where data from the stream is being captured

public:

	InputStatus();
	void SetStatus(const InputStatusCategory& status);
	std::string str() const;
	bool IsInCategory(const std::initializer_list<InputStatusCategory>& status) const;
	bool IsInCategory(const InputStatusCategory& status) const;

	bool operator==(const InputStatus& rhs) const {
		return ((*this).Active == rhs.Active &&
				(*this).Inactive == rhs.Inactive &&
				(*this).Inaccessed == rhs.Inaccessed &&
				(*this).Idle == rhs.Idle &&
				(*this).Retrieving == rhs.Retrieving);

	}
};

struct OutputStatus
{
private:
	bool Active		: 1; // State when a stream is open to an output file
	bool Inactive	: 1; // State when no stream is open
	bool Accessed	: 1; // State when Data from network is being put into the output
	bool Inaccessed	: 1; // State when no Data from the network is being put
	bool Idle		: 1; // State where no stream is open and no data is being put or output
	bool Outputing	: 1; // State where data from the stream is being pushed to the output file

public:

	OutputStatus();
	void SetStatus(const OutputStatusCategory& status);
	std::string str() const;
	bool IsInCategory(const std::initializer_list<OutputStatusCategory>& status) const;
	bool IsInCategory(const OutputStatusCategory& status) const;

	bool operator==(const OutputStatus& rhs) const {
		return ((*this).Active == rhs.Active &&
				(*this).Inactive == rhs.Inactive &&
				(*this).Accessed == rhs.Accessed &&
				(*this).Inaccessed == rhs.Inaccessed &&
				(*this).Idle == rhs.Idle &&
				(*this).Outputing == rhs.Outputing);
	}
};
