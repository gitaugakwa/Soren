#pragma once
#include <iomanip>

#include "Core.h"
#include "Base.h"
#include "Status/Status.h"
#include "nlohmann/json.hpp"

#include "Events/Event.h"
#include "Events/OutputEvent.h"

#include <fstream>
#include <ios>

#define min(a,b) (((a) < (b)) ? (a) : (b))

namespace Soren
{
	template <typename T>
	class FileInput;

	template <typename T>
	class FileOutput
	{
		// maximum file size == ULLOU_MAX
	public:
		// will not open stream on construction
		FileOutput();
		FileOutput(const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::out);
		FileOutput(const std::string& name, const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::out);
		FileOutput(FileOutput<T>&& rhs) noexcept;
		FileOutput(FileOutput<T>& rhs);
		~FileOutput();

		// basic_ostream

		void swap(FileOutput<T>& other);

		bool is_open() const;

		// checks if it is currently open, closes if the file is not the same as the one currently open, else nothing
		void open(const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::out);

		void close();

		// basic_ofstream
		explicit operator std::basic_ofstream<T>&() noexcept;

		FileOutput<T>& put(T ch);
		FileOutput<T>& write(const T* data, std::streamsize count);
		FileOutput<T>& write(const T* data, std::streamsize count, bool writebyteslength);
		FileOutput<T>& write(const T* data, std::streamsize count, T delim);
		template <typename OutType, std::enable_if_t<!std::is_pointer<OutType>::value, int> = 0>
		FileOutput<T>& write(const OutType& data);

		std::streampos tellp();
		FileOutput<T>& seekp(unsigned long long pos);
		FileOutput<T>& seekp(unsigned long long off, std::ios_base::seekdir dir);

		FileOutput<T>& flush();

		void swap(FileOutput<T>& lhs, FileOutput<T>& rhs);
		std::ofstream& data();

		// custom functions
		bool init();
		void deinit();

		void name(const std::string& name);
		std::string name() const;
		void path(const std::filesystem::path& filename);
		std::filesystem::path path() const;
		std::ios::openmode mode() const;
		void mode(std::ios::openmode mode);

		bool isValid();

		// operators
		FileOutput<T>& operator<<(const char* rhs);
		FileOutput<T>& operator<<(char rhs);
		FileOutput<T>& operator<<(char* rhs);
		FileOutput<T>& operator<<(unsigned char rhs);
		FileOutput<T>& operator<<(short rhs);
		FileOutput<T>& operator<<(unsigned short rhs);
		FileOutput<T>& operator<<(int rhs);
		FileOutput<T>& operator<<(unsigned int rhs);
		FileOutput<T>& operator<<(long rhs);
		FileOutput<T>& operator<<(unsigned long long rhs);
		FileOutput<T>& operator<<(long long rhs);
		FileOutput<T>& operator<<(float rhs);
		FileOutput<T>& operator<<(double rhs);
		FileOutput<T>& operator<<(long double rhs);
		FileOutput<T>& operator<<(bool rhs);
		FileOutput<T>& operator<<(const void* rhs);
		FileOutput<T>& operator<<(std::_Smanip<std::streamsize> rhs);
		FileOutput<T>& operator<<(std::nullptr_t rhs);
		FileOutput<T>& operator<<(std::basic_streambuf<T>* rhs);
		FileOutput<T>& operator<<(std::ios_base& (*rhs)(std::ios_base&));
		FileOutput<T>& operator<<(std::basic_ios<T>& (*rhs)(std::basic_ios<T>&));
		FileOutput<T>& operator<<(std::basic_ostream<T>& (*rhs)(std::basic_ostream<T>&));

		// cause string has it's own specialization into a stream
		FileOutput<T>& operator<<(const std::string& rhs);
		FileOutput<T>& operator<<(const nlohmann::json& rhs);
		FileOutput<T>& operator<<(const nlohmann::json&& rhs);

		FileOutput<T>& operator=(FileOutput<T>&& rhs) noexcept;
		FileOutput<T>& operator=(FileOutput<T>& rhs);

		bool operator==(const std::string& rhs) const;
		bool operator==(const FileOutput<T>& rhs) const;
		bool operator==(FileOutput<T>& rhs);
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const FileOutput<T>& rhs) const;
		bool operator!=(FileOutput<T>& rhs);

	private:
		std::string m_name{};
		std::filesystem::path m_path{};
		std::ios::openmode m_mode{};

		std::basic_ofstream<T> m_ofstream{};
		std::basic_filebuf<T> m_filebuf{ std::move(*m_ofstream.rdbuf()) };
	};

	template<typename T>
	inline FileOutput<T>::FileOutput()
	{
	}

	template<typename T>
	inline FileOutput<T>::FileOutput(const std::filesystem::path& filename, std::ios::openmode mode)
		: m_path(filename), m_mode(mode) {}

	template<typename T>
	inline FileOutput<T>::FileOutput(const std::string& name, const std::filesystem::path& filename, std::ios::openmode mode)
		: m_name(name), m_path(filename), m_mode(mode) {}

	template<typename T>
	inline FileOutput<T>::FileOutput(FileOutput<T>&& rhs) noexcept
		:	m_name		(std::move(rhs.m_name)),
			m_path		(std::move(rhs.m_path)),
			m_mode		(std::move(rhs.m_mode)),
			m_ofstream	(std::move(rhs.m_ofstream)),
			m_filebuf	(std::move(rhs.m_filebuf))
	{
	}

	template<typename T>
	inline FileOutput<T>::FileOutput(FileOutput<T>& rhs)
		:	m_name		(rhs.m_name),
			m_path		(rhs.m_path),
			m_mode		(rhs.m_mode)
	{
		if (rhs.is_open()) {
			init();
			seekp(rhs.tellp());
		}
	}

	template<typename T>
	inline FileOutput<T>::~FileOutput()
	{
		deinit();
	}

	template<typename T>
	inline void FileOutput<T>::swap(FileOutput<T>& other)
	{
		m_name		.swap(other.m_name);
		m_path		.swap(other.m_path);
		m_mode		.swap(other.m_mode);
		m_ofstream	.swap(other.m_ofstream);
		m_filebuf	.swap(other.m_filebuf);
	}

	template<typename T>
	inline bool FileOutput<T>::is_open() const
	{
		// for now, will not initialize when called
		return m_ofstream.is_open();
	}

	template<typename T>
	inline void FileOutput<T>::open(const std::filesystem::path& filename, std::ios::openmode mode)
	{
		if (m_ofstream.is_open())
		{
			bool newFile = false;
			if (filename != m_path) {
				newFile = true;
				if (!m_name.empty()) {
					SOREN_CORE_INFO("{0} (FileOutput): Closing file {1}", m_name, m_path);
					SOREN_CORE_INFO("{0} (FileOutput): Openning file {1}", m_name, filename);
				}
				else {
					SOREN_CORE_INFO("FileOutput: Closing file {0}", m_path);
					SOREN_CORE_INFO("FileOutput: Openning file {0}", filename);
				}
			}

			m_path = (newFile) ? filename : m_path;

			bool newMode = false;
			if (mode != m_mode) {
				newMode = true;
				if (!m_name.empty()) {
					SOREN_CORE_INFO("{0} (FileOutput): Closing file {1} in mode {2}", m_name, m_path, m_mode);
					SOREN_CORE_INFO("{0} (FileOutput): Openning file {1} in mode {2}", m_name, filename, mode);
				}
				else {
					SOREN_CORE_INFO("FileOutput: Closing file {0} in mode {1}", m_path, m_mode);
					SOREN_CORE_INFO("FileOutput: Openning file {0} in mode {1}", filename, mode);
				}
				m_mode = mode;
			}

			if (newMode || newFile) {
				m_ofstream.close();
			}
			else {
				return;
			}
		}
		m_path = filename;
		m_mode = mode;
		m_ofstream.open(filename, mode);
	}

	template<typename T>
	inline void FileOutput<T>::close()
	{
		m_ofstream.close();
	}

	template<typename T>
	inline FileOutput<T>::operator std::basic_ofstream<T>& () noexcept
	{
		init();
		// not sure if we need to log here
		return m_ofstream;
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::put(T ch)
	{
		if (!init()) return (*this);
		m_ofstream.put(ch);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::write(const T* data, std::streamsize count)
	{
		if (!init()) return (*this);
		m_ofstream.write(data, count);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::write(const T* data, std::streamsize count, bool writebyteslength)
	{
		if (!init()) return (*this);
		char byteswritten = (char)count;

		if (writebyteslength) {
			m_ofstream.write(&byteswritten, 1);
		}
		m_ofstream.write(data, count);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::write(const T* data, std::streamsize count, T skip)
	{
		init();
		if (count <= 0) {
			if (m_ofstream.is_open()) {
				SOREN_CORE_WARN(m_name, "{0} (FileOutput): Write to {1} has count {2} and skip {3}", m_name, m_path, count, skip);
				SOREN_CORE_WARN("FileOutput: Write to {0} has count {1} and skip {2}", m_path, count, skip);
			}
			return (*this); }

		const T* tempdata = data;
		std::streamsize internalcount = count;
		// goes to the end of the data, then recedes through the skip
		tempdata += (count - 1);

		// recedes through and skips the skip characters
		for (;
			*tempdata == skip && internalcount <= count;
			--tempdata, --internalcount);

		char byteswritten = (char)min(internalcount, count);

		// outputs the smaller size,
		// which should technically only account for count being 0
		// also write the number of bytes written

		if (count > 0 && internalcount <= 0)
		{
			// in a case where all the data provided is actually skip characters
			// should output at least one skip character
			byteswritten = 1;

			m_ofstream.write(&byteswritten, 1);
			m_ofstream.write(data, 1);

			return (*this);
		}

		m_ofstream.write(&byteswritten, 1);
		m_ofstream.write(data, min(internalcount, count));

		return (*this);
	}


	template<typename T>
	template<typename OutType, std::enable_if_t<!std::is_pointer<OutType>::value, int>>
	inline FileOutput<T>& FileOutput<T>::write(const OutType& data)
	{
		if (!init()) return (*this);
		m_ofstream.write((const T*)&data, sizeof(OutType));
		return (*this);
	}

	template<typename T>
	inline std::streampos FileOutput<T>::tellp()
	{
		// let this be handled by the ofstream
		init();
		return m_ofstream.tellp();
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::seekp(unsigned long long pos)
	{
		if (!init()) return (*this);
		m_ofstream.seekp(pos);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::seekp(unsigned long long off, std::ios_base::seekdir dir)
	{
		if (!init()) return (*this);
		m_ofstream.seekp(off, dir);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::flush()
	{
		if (!init()) return (*this);
		m_ofstream.flush();
		return (*this);
	}

	template<typename T>
	inline void FileOutput<T>::swap(FileOutput<T>& lhs, FileOutput<T>& rhs)
	{
		lhs.swap(rhs);
	}

	template<typename T>
	inline std::ofstream& FileOutput<T>::data()
	{
		return m_ofstream;
	}

	template<typename T>
	inline bool FileOutput<T>::init()
	{
		if (m_ofstream.is_open()) {
			return true;
		}
		if (!m_ofstream.is_open() && !m_path.empty()) {
			if (m_mode) {
				if (!m_name.empty()) {
					SOREN_CORE_INFO("{0} (FileOutput): Openning file {1} in mode {2}", m_name, m_path, m_mode);
				}
				else {
					SOREN_CORE_INFO("FileOutput: Openning file {0} in mode {1}", m_path, m_mode);
				}
				m_ofstream.open(m_path, m_mode);
			}
			else {
				if (!m_name.empty()) {
					SOREN_CORE_INFO("{0} (FileOutput): Openning file {1}", m_name, m_path);
				}
				else {
					SOREN_CORE_INFO("FileOutput: Openning file {0}", m_path);
				}
				m_ofstream.open(m_path);
			}
			return true;
		}
		else {
			if (!m_name.empty()) {
				SOREN_CORE_WARN("{0} (FileOutput): Function occured while unopened nor filepath given", m_name);
			}
			else {
				SOREN_CORE_WARN("FileOutput: Function occured while unopened nor filepath given");
			}
			return false;
		}
	}

	template<typename T>
	inline void FileOutput<T>::deinit()
	{
	}

	template<typename T>
	inline void FileOutput<T>::name(const std::string& name)
	{
		m_name = name;
	}

	template<typename T>
	inline std::string FileOutput<T>::name() const
	{
		return m_name;
	}

	template<typename T>
	inline void FileOutput<T>::path(const std::filesystem::path& filename)
	{
		m_path = filename;
	}

	template<typename T>
	inline std::filesystem::path FileOutput<T>::path() const
	{
		return m_path;
	}

	template<typename T>
	inline std::ios::openmode FileOutput<T>::mode() const
	{
		return m_mode;
	}

	template<typename T>
	inline void FileOutput<T>::mode(std::ios::openmode mode)
	{
		m_mode = mode;
	}

	template<typename T>
	inline bool FileOutput<T>::isValid()
	{
		return (!m_path.empty());
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator=(FileOutput<T>&& rhs) noexcept
	{
		m_name		= std::move(rhs.m_name);
		m_path		= std::move(rhs.m_path);
		m_mode		= std::move(rhs.m_mode);
		m_ofstream	= std::move(rhs.m_ofstream);
		m_filebuf	= std::move(rhs.m_filebuf);
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator=(FileOutput<T>& rhs)
	{
		m_name = rhs.m_name;
		m_path = rhs.m_path;
		m_mode = rhs.m_mode;

		// we close then if rhs was open reinitialize
		close();
		if (rhs.is_open()) {
			init();
			seekp(rhs.tellp());
		}
		return (*this);
	}

	template<typename T>
	inline bool FileOutput<T>::operator==(const std::string& rhs) const
	{
		return m_name == rhs;
	}

	template<typename T>
	inline bool FileOutput<T>::operator==(const FileOutput<T>& rhs) const
	{
		// tellp() is not called and thus this will result in an a less accurate equation
		return (m_name		== rhs.m_name &&
				m_path		== rhs.m_path &&
				m_mode		== rhs.m_mode &&
				is_open()	== rhs.is_open());
	}

	template<typename T>
	inline bool FileOutput<T>::operator==(FileOutput<T>& rhs)
	{
		return (m_name		== rhs.m_name &&
				m_path		== rhs.m_path &&
				m_mode		== rhs.m_mode &&
				is_open()	== rhs.is_open() &&
				tellp()		== rhs.tellp());
	}

	template<typename T>
	inline bool FileOutput<T>::operator!=(const std::string& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline bool FileOutput<T>::operator!=(const FileOutput<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline bool FileOutput<T>::operator!=(FileOutput<T>& rhs)
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(const char* rhs)
	{
		if(!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}	
	
	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(char rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(char* rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(unsigned char rhs)
	{
		if (!init()) return (*this);
		m_ofstream << std::hex << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(short rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(unsigned short rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(int rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(unsigned int rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(long rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(unsigned long long rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(long long rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(float rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(double rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(long double rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(bool rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(const void* rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::_Smanip<std::streamsize> rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::nullptr_t rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::basic_streambuf<T>* rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::ios_base& (*rhs)(std::ios_base&))
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::basic_ios<T>& (*rhs)(std::basic_ios<T>&))
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(std::basic_ostream<T>& (*rhs)(std::basic_ostream<T>&))
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(const std::string& rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(const nlohmann::json& rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}

	template<typename T>
	inline FileOutput<T>& FileOutput<T>::operator<<(const nlohmann::json&& rhs)
	{
		if (!init()) return (*this);
		m_ofstream << rhs;
		return (*this);
	}


}

