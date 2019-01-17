#pragma once

#include <unistd.h>
#include <cstddef>

#include "Concept.hpp"
#include "Expected.hpp"

namespace Cpposix
{

class Fd
{
	public:
		Fd() = default;
		explicit Fd(int fd): _fd(fd) {}

		virtual ~Fd() { close(); }

		operator bool () const { return _fd >= 0; }

		void assign(int fd) { _fd = fd; }
		void close()
		{
			if (*this)
				::close(_fd);
		}

		template <typename T>
		Expected<std::size_t> write(const T& t, std::size_t size)
		{
			if constexpr (std::is_pointer<T>() || std::is_array<T>())
				return writePointer(t, size);
			else if constexpr (isWritable<T>())
				return writeWritable(t, size);
			else if constexpr (isWritableContainer<T>())
				return writeContainer(t, size);
			else
			{
				static_assert(isWritable<T>() || std::is_pointer<T>() || std::is_array<T>() || isWritableContainer<T>(), "You are trying to write something you should not try to write.");
				return {};
			}
		}

		template <typename T>
		Expected<std::size_t> write(const T& t)
		{
			if constexpr (isWritable<T>())
				return writeWritable(t);
			else if constexpr (isWritableContainer<T>())
				return writeContainer(t);
			else
			{
				static_assert(isWritable<T>() || isWritableContainer<T>(), "You are trying to write something you should not try to write.");
				return {};
			}
		}

		template <typename T>
		Fd& operator<<(const T& t) { write(t); return *this; }

		
		
		// read

		// oparator >>

	private:
		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writePointer(const Writable* writable_ptr, std::size_t size)
		{
			if (*this)
			{
				auto result = ::write(_fd, writable_ptr, size);
				if (result != -1)
					return static_cast<std::size_t>(result);
				else
					return Error();
			}
			else
				return Error(EBADF);
		}

		template <typename Readable, typename = IsReadable<Readable>>
		Expected<std::size_t>	readPointer(const Readable* readable_ptr, std::size_t size)
		{
			if (*this)
			{
					return Error();
			}
			else
				return Error(EBADF);
		}

		/*
		** Conveniance methods
		*/

		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writeWritable(Writable writable, std::size_t size = sizeof(Writable)) { return writePointer(&writable, size); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t> writeContainer(WritableContainer writableContainer) { return writePointer(writableContainer.data(), writableContainer.size()); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t> writeContainer(WritableContainer writableContainer, std::size_t size) { return writePointer(writableContainer.data(), size); }

		template <typename Readable, typename = IsReadable<Readable>>
		Expected<std::size_t>	readReadable(Readable readable, std::size_t size = sizeof(Readable)) { return readPointer(&readable, size); }

		template <typename ReadableContainer, typename = IsReadableContainer<ReadableContainer>>
		Expected<std::size_t> readContainer(ReadableContainer readableContainer) { return readPointer(readableContainer.data(), readableContainer.size()); }

		template <typename ReadableContainer, typename = IsReadableContainer<ReadableContainer>>
		Expected<std::size_t> readContainer(ReadableContainer readableContainer, std::size_t size) { return readPointer(readableContainer.data(), size); }

	protected:
		int _fd = -1;
};

}