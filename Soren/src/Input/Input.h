#pragma once
#include <iomanip>

#include "Core.h"
#include "Base.h"
#include "Status/Status.h"
#include "nlohmann/json.hpp"

#include "Events/Event.h"
#include "Events/InputEvent.h"

#include <fstream>
#include <ios>
#include <variant>

namespace Soren
{
	// fix

	template <typename T>
	class FileInput
	{
		// maximum file size == ULLOU_MAX
	public:
		FileInput();
		FileInput(const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::in);
		FileInput(const std::string& name, const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::in);
		FileInput(FileInput<T>&& rhs) noexcept;
		FileInput(FileInput<T>& rhs);

		~FileInput();

		// basic_istream
		explicit operator std::basic_ifstream<T>&() noexcept;

		void swap(FileInput<T>& other);

		bool is_open() const;

		void open(const std::filesystem::path& filename, std::ios::openmode mode = std::ios_base::in);

		void close();

		// basic_ifstream

		T get();
		FileInput<T>& get(T& ch);
		FileInput<T>& get(T* s, std::streamsize count);
		FileInput<T>& get(T* s, std::streamsize count, T delim);
		FileInput<T>& get(std::basic_streambuf<T>& strbuf);
		FileInput<T>& get(std::basic_streambuf<T>& strbuf, T delim);
		T peek();
		FileInput<T>& unget();
		FileInput<T>& putback(T ch);
		FileInput<T>& getline(T* s, std::streamsize count);
		FileInput<T>& getline(T* s, std::streamsize count, T delim);
		FileInput<T>& ignore(std::streamsize count = 1, T delim = std::ios::eofbit);
		FileInput<T>& read(T* s, std::streamsize count);
		template <typename InType, std::enable_if_t<!std::is_pointer<InType>::value, int> = 0>
		FileInput<T>& read(InType& s, bool readLength = false);
		std::streamsize readsome(T* s, std::streamsize count);
		std::streamsize gcount();

		std::streampos tellg();
		FileInput<T>& seekg(unsigned long long pos);
		FileInput<T>& seekg(unsigned long long off, std::ios_base::seekdir dir);

		FileInput<T>& sync();

		void swap(FileInput<T>& lhs, FileInput<T>& rhs);
		std::ifstream& data();

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
		FileInput<T>& operator>>(char& rhs);
		FileInput<T>& operator>>(char* rhs);
		FileInput<T>& operator>>(unsigned char& rhs);
		FileInput<T>& operator>>(short& rhs);
		FileInput<T>& operator>>(unsigned short& rhs);
		FileInput<T>& operator>>(int& rhs);
		FileInput<T>& operator>>(unsigned int& rhs);
		FileInput<T>& operator>>(long& rhs);
		FileInput<T>& operator>>(unsigned long long& rhs);
		FileInput<T>& operator>>(long long& rhs);
		FileInput<T>& operator>>(float& rhs);
		FileInput<T>& operator>>(double& rhs);
		FileInput<T>& operator>>(long double& rhs);
		FileInput<T>& operator>>(bool& rhs);
		FileInput<T>& operator>>(void*& rhs);
		FileInput<T>& operator>>(std::_Smanip<std::streamsize> rhs);
		FileInput<T>& operator>>(std::ios_base& (*rhs)(std::ios_base&));
		FileInput<T>& operator>>(std::basic_ios<T>& (*rhs)(std::basic_ios<T>&));
		FileInput<T>& operator>>(std::basic_istream<T>& (*rhs)(std::basic_istream<T>&));
		FileInput<T>& operator>>(std::basic_streambuf<T>* sb);

		// cause string has it's own specialization into a stream
		FileInput<T>& operator>>(std::string& rhs);
		FileInput<T>& operator>>(nlohmann::json& rhs);
		FileInput<T>& operator>>(nlohmann::json&& rhs);

		FileInput<T>& operator=(FileInput<T>&& rhs) noexcept;
		FileInput<T>& operator=(FileInput<T>& rhs);

		bool operator==(const std::string& rhs) const;
		bool operator==(const FileInput<T>& rhs) const;
		bool operator==(FileInput<T>& rhs);
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const FileInput<T>& rhs) const;
		bool operator!=(FileInput<T>& rhs);

		private:
		std::string m_name{};
		std::filesystem::path m_path{};
		std::ios::openmode m_mode{};

		std::basic_ifstream<T> m_ifstream{};
		std::basic_filebuf<T> m_filebuf{ std::move(*m_ifstream.rdbuf()) };
	};

	template<typename T>
	inline FileInput<T>::FileInput()
	{
	}

	template<typename T>
	inline FileInput<T>::FileInput(const std::filesystem::path& filename, std::ios::openmode mode)
		: m_path(filename), m_mode(mode) {}

	template<typename T>
	inline FileInput<T>::FileInput(const std::string& name, const std::filesystem::path& filename, std::ios::openmode mode)
		: m_name(name), m_path(filename), m_mode(mode) {}

	template<typename T>
	inline FileInput<T>::FileInput(FileInput<T>&& rhs) noexcept
		:	m_name(std::move(rhs.m_name)),
			m_path(std::move(rhs.m_path)),
			m_mode(std::move(rhs.m_mode)),
			m_ifstream(std::move(rhs.m_ifstream)),
			m_filebuf(std::move(rhs.m_filebuf))
	{
	}

	template<typename T>
	inline FileInput<T>::FileInput(FileInput<T>& rhs)
		:	m_name(rhs.m_name),
			m_path(rhs.m_path),
			m_mode(rhs.m_mode)
	{
		if (rhs.is_open()) {
			if (!m_ifstream.is_open() && !m_path.empty()) {
				if (m_mode) {
					m_ifstream.open(m_path, m_mode);
				}
				else {
					m_ifstream.open(m_path);
				}
			}
			seekg(rhs.tellg());
		}
	}

	template<typename T>
	inline FileInput<T>::~FileInput()
	{
		deinit();
	}

	template<typename T>
	inline FileInput<T>::operator std::basic_ifstream<T>& () noexcept
	{
		return m_ifstream;
	}

	template<typename T>
	inline void FileInput<T>::swap(FileInput<T>& other)
	{
		m_name		.swap(other.m_name);
		m_path		.swap(other.m_path);
		m_mode		.swap(other.m_mode);
		m_ifstream	.swap(other.m_ifstream);
		m_filebuf	.swap(other.m_filebuf);
	}

	template<typename T>
	inline bool FileInput<T>::is_open() const
	{
		return m_ifstream.is_open();
	}

	template<typename T>
	inline void FileInput<T>::open(const std::filesystem::path& filename, std::ios::openmode mode)
	{
		if (m_ifstream.is_open())
		{
			bool newFile = false;
			if (filename != m_path) {
				newFile = true;
				if (!m_name.empty()) {
					OL_CORE_INFO("{0} (FileInput): Closing file {1}", m_name, m_path);
					OL_CORE_INFO("{0} (FileInput): Openning file {1}", m_name, filename);
				}
				else {
					OL_CORE_INFO("FileInput: Closing file {0}", m_path);
					OL_CORE_INFO("FileInput: Openning file {0}", filename);
				}
			}

			m_path = (newFile) ? filename : m_path;

			bool newMode = false;
			if (mode != m_mode) {
				newMode = true;
				if (!m_name.empty()) {
					OL_CORE_INFO("{0} (FileInput): Closing file {1} in mode {2}", m_name, m_path, m_mode);
					OL_CORE_INFO("{0} (FileInput): Openning file {1} in mode {2}", m_name, filename, mode);
				}
				else {
					OL_CORE_INFO("FileInput: Closing file {0} in mode {1}", m_path, m_mode);
					OL_CORE_INFO("FileInput: Openning file {0} in mode {1}", filename, mode);
				}
				m_mode = mode;
			}

			if (newMode || newFile) {
				m_ifstream.close();
			}
			else {
				return;
			}
		}
		m_path = filename;
		m_mode = mode;
		m_ifstream.open(filename, mode);
	}

	template<typename T>
	inline void FileInput<T>::close()
	{
		m_ifstream.close();
	}

	template<typename T>
	inline T FileInput<T>::get()
	{
		// this function is wonky and since the original returns int_type, this is not really gonna be okay.
		// just don't call it till we fix
		if (!init()) return T();
		return m_ifstream.get();
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::get(T& ch)
	{
		if (!init()) return (*this);
		m_ifstream.get(ch);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::get(T* s, std::streamsize count)
	{
		if (!init()) return (*this);
		m_ifstream.get(s, count);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::get(T* s, std::streamsize count, T delim)
	{
		if (!init()) return (*this);
		m_ifstream.get(s, count, delim);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::get(std::basic_streambuf<T>& strbuf)
	{
		if (!init()) return (*this);
		m_ifstream.get(strbuf);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::get(std::basic_streambuf<T>& strbuf, T delim)
	{
		if (!init()) return (*this);
		m_ifstream.get(strbuf, delim);
		return (*this);
	}

	template<typename T>
	inline T FileInput<T>::peek()
	{
		if (!init()) return T();
		return m_ifstream.peek();
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::unget()
	{
		if (!init()) return (*this);
		m_ifstream.unget();
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::putback(T ch)
	{
		if (!init()) return (*this);
		m_ifstream.putback(ch);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::getline(T* s, std::streamsize count)
	{
		if (!init()) return (*this);
		m_ifstream.getline(s, count);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::getline(T* s, std::streamsize count, T delim)
	{
		if (!init()) return (*this);
		m_ifstream.getline(s, count, delim);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::ignore(std::streamsize count, T delim)
	{
		if (!init()) return (*this);
		m_ifstream.ignore(count, delim);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::read(T* s, std::streamsize count)
	{
		if (!init()) return (*this);
		m_ifstream.read(s,count);
		return (*this);
	}

	template<typename T>
	template<typename InType, std::enable_if_t<!std::is_pointer<InType>::value, int>>
	inline FileInput<T>& FileInput<T>::read(InType& s, bool readLength)
	{
		switch (readLength)
		{
		case true:
		{
			if (!init()) return (*this);

			// get length
			T length;
			m_ifstream.read(&length, sizeof(T));

			// get data
			m_ifstream.read((T*)&s, length);
			return (*this);
		}
		case false:
		{
			if (!init()) return (*this);
			m_ifstream.read((T*)&s, sizeof(InType));
			return (*this);
		}
		}
		return (*this);
	}

	template<typename T>
	inline std::streamsize FileInput<T>::readsome(T* s, std::streamsize count)
	{
		// let ifstream handle
		init();
		return m_ifstream.readsome(s, count);
	}

	template<typename T>
	inline std::streamsize FileInput<T>::gcount()
	{
		// let ifstream handle
		init();
		return m_ifstream.gcount();
	}

	template<typename T>
	inline std::streampos FileInput<T>::tellg()
	{
		// let ifstream handle
		init();
		return m_ifstream.tellg();
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::seekg(unsigned long long pos)
	{
		if (!init()) return (*this);
		m_ifstream.seekg(pos);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::seekg(unsigned long long off, std::ios_base::seekdir dir)
	{
		if (!init()) return (*this);
		m_ifstream.seekg(off, dir);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::sync()
	{
		if (!init()) return (*this);
		m_ifstream.sync();
		return (*this);
	}

	template<typename T>
	inline void FileInput<T>::swap(FileInput<T>& lhs, FileInput<T>& rhs)
	{
		lhs.swap(rhs);
	}

	template<typename T>
	inline std::ifstream& FileInput<T>::data()
	{
		return m_ifstream;
	}

	template<typename T>
	inline bool FileInput<T>::init()
	{
		if (m_ifstream.is_open()) {
			return true;
		}
		if (!m_ifstream.is_open() && !m_path.empty()) {
			if (m_mode) {
				if (!m_name.empty()) {
					OL_CORE_INFO("{0} (FileInput): Openning file {1} in mode {2}", m_name, m_path, m_mode);
				}
				else {
					OL_CORE_INFO("FileInput: Openning file {0} in mode {1}", m_path, m_mode);
				}
				m_ifstream.open(m_path, m_mode);
			}
			else {
				if (!m_name.empty()) {
					OL_CORE_INFO("{0} (FileInput): Openning file {1}", m_name, m_path);
				}
				else {
					OL_CORE_INFO("FileInput: Openning file {0}", m_path);
				}
				m_ifstream.open(m_path);
			}
			return true;
		}
		else {
			if (!m_name.empty()) {
				OL_CORE_WARN("{0} (FileInput): Function occured while unopened nor filepath given", m_name);
			}
			else {
				OL_CORE_WARN("FileInput: Function occured while unopened nor filepath given");
			}
			return false;
		}
	}

	template<typename T>
	inline void FileInput<T>::deinit()
	{
	}

	template<typename T>
	inline void FileInput<T>::name(const std::string& name)
	{
		m_name = name;
	}

	template<typename T>
	inline std::string FileInput<T>::name() const
	{
		return m_name;
	}

	template<typename T>
	inline void FileInput<T>::path(const std::filesystem::path& filename)
	{
		m_path = filename;
	}

	template<typename T>
	inline std::filesystem::path FileInput<T>::path() const
	{
		return m_path;
	}

	template<typename T>
	inline std::ios::openmode FileInput<T>::mode() const
	{
		return m_mode;
	}

	template<typename T>
	inline void FileInput<T>::mode(std::ios::openmode mode)
	{
		m_mode = mode;
	}

	template<typename T>
	inline bool FileInput<T>::isValid()
	{
		return (!m_path.empty());
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator=(FileInput<T>&& rhs) noexcept
	{
		m_name		= std::move(rhs.m_name);
		m_path		= std::move(rhs.m_path);
		m_mode		= std::move(rhs.m_mode);
		m_ifstream	= std::move(rhs.m_ifstream);
		m_filebuf	= std::move(rhs.m_filebuf);
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator=(FileInput<T>& rhs)
	{
		m_name = rhs.m_name;
		m_path = rhs.m_path;
		m_mode = rhs.m_mode;

		// we close then if rhs was open reinitialize
		close();
		if (rhs.is_open()) {
			init();
			seekg(rhs.tellg());
		}
		return (*this);
	}

	template<typename T>
	inline bool FileInput<T>::operator==(const std::string& rhs) const
	{
		return m_name == rhs;
	}

	template<typename T>
	inline bool FileInput<T>::operator==(const FileInput<T>& rhs) const
	{
		// tellg() is not called and thus this will result in an a less accurate equation
		return (m_name		== rhs.m_name &&
				m_path		== rhs.m_path &&
				m_mode		== rhs.m_mode &&
				is_open()	== rhs.is_open());
	}

	template<typename T>
	inline bool FileInput<T>::operator==(FileInput<T>& rhs)
	{
		return (m_name		== rhs.m_name &&
				m_path		== rhs.m_path &&
				m_mode		== rhs.m_mode &&
				is_open()	== rhs.is_open() &&
				tellg()		== rhs.tellg());
	}

	template<typename T>
	inline bool FileInput<T>::operator!=(const std::string& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline bool FileInput<T>::operator!=(const FileInput<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline bool FileInput<T>::operator!=(FileInput<T>& rhs)
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(char& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(char* rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(unsigned char& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> std::hex >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(short& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(unsigned short& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(int& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(unsigned int& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(long& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(unsigned long long& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(long long& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(float& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(double& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(long double& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(bool& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(void*& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::_Smanip<std::streamsize> rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::ios_base& (*rhs)(std::ios_base&))
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::basic_ios<T>& (*rhs)(std::basic_ios<T>&))
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::basic_istream<T>& (*rhs)(std::basic_istream<T>&))
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::basic_streambuf<T>* rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(std::string& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(nlohmann::json& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}

	template<typename T>
	inline FileInput<T>& FileInput<T>::operator>>(nlohmann::json&& rhs)
	{
		if (!init()) return (*this);
		m_ifstream >> rhs;
		return (*this);
	}


}

