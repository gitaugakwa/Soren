#include "olpch.h"
#include "Status/Status.h"

// Status

// Give Different States meaning

NetworkStatus::NetworkStatus()
{
	Initialized	= false;
	Training	= false;
	Testing		= false;
	Idle		= true;
	Inputing	= false;
	Outputing	= false;
}

void NetworkStatus::SetStatus(const StatusCategory& status)
{
	switch (status)
	{
	case StatusCategory::Initialized:
	{
		Initialized = true;
		break;
	}
	case StatusCategory::Uninitialized:
	{
		Initialized = false;
		break;
	}
	case StatusCategory::Training:
	{
		Testing = false;
		Release = false;
		Idle = false;

		Training = true;
		break;
	}
	case StatusCategory::Testing:
	{
		Training = false;
		Release = false;
		Idle = false;

		Testing = true;
		break;
	}
	case StatusCategory::Release:
	{
		Training = false;
		Testing = false;
		Idle = false;

		Release = true;
		break;
	}
	case StatusCategory::Idle:
	{
		Training = false;
		Testing = false;
		Release = false;
		Inputing = false;
		Outputing = false;

		Idle = true;
		break;
	}
	case StatusCategory::Inputing:
	{
		Outputing = false;
		Idle = false;

		Inputing = true;
		break;
	}
	case StatusCategory::Outputing:
	{
		Inputing = false;
		Idle = false;

		Outputing = true;
		break;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid StatusCategory type");
		assert(false);
	}
	}
}

std::string NetworkStatus::str() const
{
	std::stringstream ss;
	if (Initialized)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Initialized";
	}
	else
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Uninitialized";
	}
	if (Training)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Training";
	}
	if (Testing)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Testing";
	}
	if (Release)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Release";
	}
	if (Idle)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Idle";
	}
	if (Inputing)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Inputing";
	}
	if (Outputing)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Outputing";
	}
	return ss.str();
}

bool NetworkStatus::IsInCategory(const std::initializer_list<StatusCategory>& status) const
{
	bool isin = true;

	for (auto stat : status)
	{
		bool temp;

		switch (stat)
		{
		case StatusCategory::Initialized:
		{
			temp = Initialized;
			break;
		}
		case StatusCategory::Uninitialized:
		{
			temp = !Initialized;
			break;
		}
		case StatusCategory::Training:
		{
			temp = Training;
			break;
		}
		case StatusCategory::Testing:
		{
			temp = Testing;
			break;
		}
		case StatusCategory::Release:
		{
			temp = Release;
			break;
		}
		case StatusCategory::Idle:
		{
			temp = Idle;
			break;
		}
		case StatusCategory::Inputing:
		{
			temp = Inputing;
			break;
		}
		case StatusCategory::Outputing:
		{
			temp = Outputing;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid StatusCategory type");
			assert(false);
			return false;
		}
		}

		if (temp)
		{
			return true;
		}
	}

	return isin;
}

bool NetworkStatus::IsInCategory(const StatusCategory& status) const
{
	switch (status)
	{
	case StatusCategory::Initialized:
	{
		return Initialized;
	}
	case StatusCategory::Uninitialized:
	{
		return !Initialized;
	}
	case StatusCategory::Training:
	{
		return Training;
	}
	case StatusCategory::Testing:
	{
		return Testing;
	}
	case StatusCategory::Release:
	{
		return Release;
	}
	case StatusCategory::Idle:
	{
		return Idle;
	}
	case StatusCategory::Inputing:
	{
		return Inputing;
	}
	case StatusCategory::Outputing:
	{
		return Outputing;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid StatusCategory type");
		assert(false);
		return false;
	}
	}
}

// InputStatus

InputStatus::InputStatus()
{
	Active		= false;
	Inactive	= false;
	Accessed	= false;
	Inaccessed	= false;
	Idle		= true;
	Retrieving	= false;
}

void InputStatus::SetStatus(const InputStatusCategory& status)
{
	switch (status)
	{
	case InputStatusCategory::Active:
	{
		Inactive = false;
		Idle = false;

		Active = true;
		break;
	}
	case InputStatusCategory::Inactive:
	{
		Active = false;
		Retrieving = false;
		Accessed = false;
		Inaccessed = false;
		Idle = false;

		Inactive = true;
		break;
	}
	case InputStatusCategory::Accessed:
	{
		Active = true;
		Inactive = false;
		Inaccessed = false;
		Retrieving = false;
		Idle = false;

		Accessed = true;
		break;
	}
	case InputStatusCategory::Inaccessed:
	{
		Accessed = false;
		Idle = false;
		Retrieving = false;
	
		Inaccessed = true;
		break;
	}
	case InputStatusCategory::Idle:
	{
		Active = false;
		Inactive = false;
		Accessed = false;
		Inaccessed = false;
		Retrieving = false;

		Idle = true;
		break;
	}
	case InputStatusCategory::Retrieving:
	{
		Inactive = false;
		Idle = false;

		Active = true;
		Retrieving = true;
		break;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid InputStatusCategory type");
		assert(false);
	}
	}
}

std::string InputStatus::str() const
{
	std::stringstream ss;
	if (Active)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Active";
	}
	if (Inactive)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Inactive";
	}
	if (Accessed)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Accessed";
	}
	if (Inaccessed)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Inaccessed";
	}
	if (Idle)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Idle";
	}
	if (Retrieving)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Retrieving";
	}
	return ss.str();
}

bool InputStatus::IsInCategory(const std::initializer_list<InputStatusCategory>& status) const
{
	bool isin = true;
	for (auto stat : status)
	{
		bool temp;

		switch (stat)
		{
		case InputStatusCategory::Active:
		{
			temp = Active;
			break;
		}
		case InputStatusCategory::Inactive:
		{
			temp = Inactive;
			break;
		}
		case InputStatusCategory::Accessed:
		{
			temp = Accessed;
			break;
		}
		case InputStatusCategory::Inaccessed:
		{
			temp = Inaccessed;
			break;
		}
		case InputStatusCategory::Idle:
		{
			temp = Idle;
			break;
		}
		case InputStatusCategory::Retrieving:
		{
			temp = Retrieving;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid InputStatusCategory type");
			assert(false);
			return false;
		}
		}

		if (!temp)
		{
			return false;
		}
	}

	return isin;
}

bool InputStatus::IsInCategory(const InputStatusCategory& status) const
{
	switch (status)
	{
	case InputStatusCategory::Active:
	{
		return Active;
	}
	case InputStatusCategory::Inactive:
	{
		return Inactive;
	}
	case InputStatusCategory::Accessed:
	{
		return Accessed;
	}
	case InputStatusCategory::Inaccessed:
	{
		return Inaccessed;
	}
	case InputStatusCategory::Idle:
	{
		return Idle;
	}
	case InputStatusCategory::Retrieving:
	{
		return Retrieving;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid InputStatusCategory type");
		assert(false);
		return false;
	}
	}
}

// OutputStatus

OutputStatus::OutputStatus()
{
	Active		= false;
	Inactive	= false;
	Accessed	= false;
	Inaccessed	= false;
	Idle		= true;
	Outputing	= false;
}

void OutputStatus::SetStatus(const OutputStatusCategory& status)
{
	switch (status)
	{
	case OutputStatusCategory::Active:
	{
		Inactive = false;
		Idle = false;

		Active = true;
		break;
	}
	case OutputStatusCategory::Inactive:
	{
		Active = false;
		Outputing = false;
		Accessed = false;
		Inaccessed = false;
		Idle = false;

		Inactive = true;
		break;
	}
	case OutputStatusCategory::Accessed:
	{
		Inaccessed = false;
		Idle = false;
		Outputing = false;

		Accessed = true;
		break;
	}
	case OutputStatusCategory::Inaccessed:
	{
		Accessed = false;
		Idle = false;
		Outputing = false;

		Inaccessed = true;
		break;
	}
	case OutputStatusCategory::Idle:
	{
		Active = false;
		Inactive = false;
		Accessed = false;
		Inaccessed = false;
		Outputing = false;

		Idle = true;
		break;
	}
	case OutputStatusCategory::Outputing:
	{
		Inactive = false;
		Idle = false;

		Active = true;
		Outputing = true;
		break;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid OutputStatusCategory type");
		assert(false);
	}
	}
}

std::string OutputStatus::str() const
{
	std::stringstream ss;
	if (Active)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Active";
	}
	if (Inactive)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Inactive";
	}
	if (Accessed)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Accessed";
	}
	if (Inaccessed)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Inaccessed";
	}
	if (Idle)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Idle";
	}
	if (Outputing)
	{
		if (ss.tellp() > 0)
		{
			ss << ", ";
		}
		ss << "Outputing";
	}
	return ss.str();
}

bool OutputStatus::IsInCategory(const std::initializer_list<OutputStatusCategory>& status) const
{
	bool isin = true;
	for (auto stat : status)
	{
		bool temp;

		switch (stat)
		{
		case OutputStatusCategory::Active:
		{
			temp = Active;
			break;
		}
		case OutputStatusCategory::Inactive:
		{
			temp = Inactive;
			break;
		}
		case OutputStatusCategory::Accessed:
		{
			temp = Accessed;
			break;
		}
		case OutputStatusCategory::Inaccessed:
		{
			temp = Inaccessed;
			break;
		}
		case OutputStatusCategory::Idle:
		{
			temp = Idle;
			break;
		}
		case OutputStatusCategory::Outputing:
		{
			temp = Outputing;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid OutputStatusCategory type");
			assert(false);
			return false;
		}
		}

		if (!temp)
		{
			return false;
		}
	}

	return isin;
}

bool OutputStatus::IsInCategory(const OutputStatusCategory& status) const
{
	switch (status)
	{
	case OutputStatusCategory::Active:
	{
		return Active;
	}
	case OutputStatusCategory::Inactive:
	{
		return Inactive;
	}
	case OutputStatusCategory::Accessed:
	{
		return Accessed;
	}
	case OutputStatusCategory::Inaccessed:
	{
		return Inaccessed;
	}
	case OutputStatusCategory::Idle:
	{
		return Idle;
	}
	case OutputStatusCategory::Outputing:
	{
		return Outputing;
	}
	default:
	{
		OL_CORE_ERROR("Enter Valid OutputStatusCategory type");
		assert(false);
		return false;
	}
	}
}
