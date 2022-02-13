#include "olpch.h"
#include "Layer/Layer.h"

#include "Output/Output.h"
#include "Input/Input.h"

namespace Soren {
namespace Neural {

	// can we export and import callbacks and functions?? like the whole process of compiling code at runtime or even using other languages like c# 

	bool Layer::SeekBegin(Soren::FileInput<Input_t>& fi)
	{
		bool found = false;
		for (; !found && (std::ifstream&)(fi);)
		{
			Input_t buffer{};

			// loop till it finds the first character of the opening tag
			// change std::ifstream if we are gonna support custom types
			for (fi.get(buffer); buffer != m_OpeningTag[0] && (std::ifstream&)(fi); fi.get(buffer));
			fi.unget();

			// check if it's equal to the open tag
			for (size_t i = 0; i < m_OpeningTag.size(); ++i) {
				if (fi.get(buffer); buffer != m_OpeningTag[i] || !(std::ifstream&)(fi))
				{
					// if it is going through the tag and doesn't find the full tag
					found = false;
					break;
				}
				found = true;
			}
		}
		return found;
	}

	bool Layer::SeekEnd(Soren::FileInput<Input_t>& fi)
	{
		bool found = false;
		for (; !found && (std::ifstream&)(fi);)
		{
			Input_t buffer{};

			// loop till it finds the first character of the opening tag
			// change std::ifstream if we are gonna support custom types
			for (fi.get(buffer); buffer != m_ClosingTag[0] && (std::ifstream&)(fi); fi.get(buffer));
			fi.unget();

			// check if it's equal to the open tag
			for (size_t i = 0; i < m_ClosingTag.size(); ++i) {
				if (fi.get(buffer); buffer != m_ClosingTag[i] || !(std::ifstream&)(fi))
				{
					// if it is going through the tag and doesn't find the full tag
					found = false;
					break;
				}
				found = true;
			}
		}
		return found;
	}

} // namespace Neural
} // namespace Soren