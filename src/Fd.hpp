#pragma once

#include <unistd.h>
#include <cstddef>
#include <cstdlib>

#include "Concept.hpp"
#include "Expected.hpp"

namespace Cpposix
{

class Fd
{
	public:
		Fd() = default;
		explicit Fd(int fd): _fd(fd) {}

		virtual ~Fd()
		{
			if (_fd != 0 && _fd != 1 && _fd != 2)
				close();
		}

		operator bool () const { return _fd >= 0; }

		void assign(int fd) { _fd = fd; }
		void close()
		{
			if (*this)
				::close(_fd);
		}

		template <typename T>
		Expected<std::size_t> write(const T* t, std::size_t size) { return writePointer(t, size); }

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

		
		template <typename T>
		Expected<std::size_t> readIn(T* t, std::size_t size) { return readInPointer(t, size); }
		
		template <typename T>
		Expected<std::size_t> readIn(T& buffer, std::size_t size)
		{
			if constexpr (isReadable<T>())
				return readInReadable(buffer, size);
			else if constexpr (isReadableInContainer<T>())
				return readInContainer(buffer, size);
			else
			{
				static_assert(isReadable<T>() || std::is_pointer<T>() || std::is_array<T>() || isReadableInContainer<T>(), "You are trying to write something you should not try to write.");
				return {};
			}
		}

		template <typename T>
		Expected<std::size_t> readIn(T& t)
		{
			if constexpr (isReadable<T>())
				return readInReadable(t);
			else if constexpr (isReadableInContainer<T>())
				return readInContainer(t);
			else
			{
				static_assert(isReadable<T>() || isReadableInContainer<T>(), "You are trying to write something you should not try to write.");
				return {};
			}
		}

		template <typename T>
		Fd& operator>>(T& buffer) { readIn(buffer); return *this; }

	private:
		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writePointer(const Writable* writable_ptr, std::size_t size)
		{
			if (*this)
			{
				auto nb_byte_writed = ::write(_fd, writable_ptr, size);
				if (nb_byte_writed != -1)
					return static_cast<std::size_t>(nb_byte_writed);
				else
					return Error();
			}
			else
				return Error(EBADF);
		}

		template <typename Readable, typename = IsReadable<Readable>>
		Expected<std::size_t>	readInPointer(Readable* readable_ptr, std::size_t size)
		{
			if (*this)
			{
				auto nb_bytes_readed = ::read(_fd, readable_ptr, size);
				if (nb_bytes_readed != -1)
					return static_cast<std::size_t>(nb_bytes_readed);
				else
					return Error();
			}
			else
				return Error(EBADF);
		}

		/*
		** Conveniance methods
		*/

		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writeWritable(const Writable& writable, std::size_t size = sizeof(Writable)) { return writePointer(&writable, size); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t> writeContainer(const WritableContainer& writableContainer) { return writePointer(writableContainer.data(), writableContainer.size()); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t> writeContainer(const WritableContainer& writableContainer, std::size_t size)
		{
			return (size <= writableContainer.size()) ? writePointer(writableContainer.data(), size) : Error(EFAULT);
		}


		template <typename Readable, typename = IsReadable<Readable>>
		Expected<std::size_t>	readInReadable(Readable& readable, std::size_t size = sizeof(Readable)) { return readInPointer(&readable, size); }

		template <typename ReadableInContainer, typename = IsReadableInContainer<ReadableInContainer>>
		Expected<std::size_t> readInContainer(ReadableInContainer& readableInContainer) { return readInPointer(readableInContainer.data(), readableInContainer.size()); }

		template <typename ReadableInContainer, typename = IsReadableInContainer<ReadableInContainer>>
		Expected<std::size_t> readInContainer(ReadableInContainer& readableInContainer, std::size_t size)
		{
			return (size <= readableInContainer.size()) ? readInPointer(readableInContainer.data(), size) : Error(EFAULT);
		}

	protected:
		int _fd = -1;
};

}