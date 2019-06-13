#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <cstddef>
#include <cstdlib>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <functional>

#include "Concept.hpp"
#include "Expected.hpp"

namespace Cpposix
{

struct FdFlags
{
	FdFlags() = default;
	FdFlags(int f): flags(f) {}

	template <typename ...Args>
	FdFlags(Args... args): flags((... | args)) {}

	int flags = 0;
};

class Fd
{
	public:
		Fd() = default;
		explicit Fd(int fd): _fd(fd) {}
		explicit Fd(bool) = delete;

		Fd(const Fd&) = delete;
		Fd(Fd&& other)
		{
			close();
			_fd = other._fd;
			other._fd = -1;
		}

		virtual ~Fd()
		{
			if (_fd != 0 && _fd != 1 && _fd != 2)
				close();
		}

		Fd& operator=(const Fd&) = delete;
		Fd& operator=(Fd&& other)
		{
			close();
			_fd = other._fd;
			other._fd = -1;
			return *this;
		}

		int getFdAsInt() const { return _fd; }

		operator bool () const { return _fd >= 0; }

		template <typename T>
		Fd& operator<<(const T& t) { write(t); return *this; }

		template <typename T>
		Fd& operator>>(T& buffer) { readIn(buffer); return *this; }

		void	assign(int fd) { _fd = fd; }
		void	close()
		{
			if (*this)
				::close(_fd);
		}

		Expected<Void>	fSync()
		{
			if (::fsync(_fd) != -1)
				return Void();
			else
				return Error();
		}

		Expected<Void>	fDataSync()
		{
			if (::fsync(_fd) != -1)
				return Void();
			else
				return Error();
		}

		template <typename ...Args>
		Expected<int> ioctl(unsigned long request, Args... args)
		{
			int result = ::ioctl(_fd, request, args...);
			if (result != -1)
				return result;
			else
				return Error();
		}

		template <typename ...Args>
		Expected<Void> fcntl(int cmd, Args... args)
		{
			int result = ::fcntl(_fd, cmd, args...);
			if (result != -1)
				return Void();
			else
				return Error();
		}

		/* Write */
		template <typename T>
		Expected<std::size_t>	write(const T* t, std::size_t size) { return writePointer(t, size); }

		template <typename T>
		Expected<std::size_t>	write(const T& t, std::size_t size)
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
				return Expected<std::size_t>();
			}
		}

		template <typename T>
		Expected<std::size_t>	write(const T& t)
		{
			if constexpr (isWritable<T>())
				return writeWritable(t);
			else if constexpr (isWritableContainer<T>())
				return writeContainer(t);
			else
			{
				static_assert(isWritable<T>() || isWritableContainer<T>(), "You are trying to write something you should not try to write.");
				return Expected<std::size_t>();
			}
		}

		/* Read in */
		template <typename T>
		Expected<std::size_t>	readIn(T* t, std::size_t size) { return readInPointer(t, size); }
		
		template <typename T>
		Expected<std::size_t>	readIn(T& buffer, std::size_t size)
		{
			if constexpr (isReadable<T>())
				return readInReadable(buffer, size);
			else if constexpr (isReadableInContainer<T>())
				return readInContainer(buffer, size);
			else
			{
				static_assert(isReadable<T>() || std::is_pointer<T>() || std::is_array<T>() || isReadableInContainer<T>(), "You are trying to read in something you should not.");
				return Expected<std::size_t>();
			}
		}

		template <typename T>
		Expected<std::size_t>	readIn(T& t)
		{
			if constexpr (isReadable<T>())
				return readInReadable(t);
			else if constexpr (isReadableInContainer<T>())
				return readInContainer(t);
			else
			{
				static_assert(isReadable<T>() || isReadableInContainer<T>(), "You are trying to read in something you should not.");
				return Expected<std::size_t>();
			}
		}


		/* Read */
		template <typename T>
		Expected<T>	read() { return readReadable<T>(); }

		template <typename T>
		Expected<T>	read(std::size_t size)
		{
			if constexpr (isReadable<T>())
				return readReadable<T>(size);
			else if constexpr (isReadableContainer<T>())
				return readContainer<T>(size);
			else
			{
				static_assert(isReadable<T>() || isReadableInContainer<T>(), "You are trying to write something you should not.");
				return {};
			}
		}

		/* Read exact */
		template <typename T>
		Expected<T>	readExact() { return readExactReadable<T>(); }

		template <typename T>
		Expected<T>	readExact(std::size_t size)
		{
			if constexpr (isReadable<T>())
				return readExactReadable<T>(size);
			else if constexpr (isReadableContainer<T>())
				return readExactContainer<T>(size);
			else
			{
				static_assert(isReadable<T>() || isReadableInContainer<T>(), "You are trying to write something you should not.");
				return {};
			}
		}


	public:
		/* Write */
		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writePointer(const Writable* writable_ptr, std::size_t size)
		{
			auto nb_byte_writed = ::write(_fd, writable_ptr, size);
			if (nb_byte_writed != -1)
				return static_cast<std::size_t>(nb_byte_writed);
			else
				return Error();
		}

		template <typename Writable, typename = IsWritable<Writable>>
		Expected<std::size_t>	writeWritable(const Writable& writable, std::size_t size = sizeof(Writable)) { return writePointer(&writable, size); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t>	writeContainer(const WritableContainer& writableContainer) { return writePointer(writableContainer.data(), writableContainer.size()); }

		template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
		Expected<std::size_t>	writeContainer(const WritableContainer& writableContainer, std::size_t size)
		{
			return (size <= writableContainer.size()) ? writePointer(writableContainer.data(), size) : Error(EFAULT);
		}

		/* Read in */
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

		template <typename Readable, typename = IsReadable<Readable>>
		Expected<std::size_t>	readInReadable(Readable& readable, std::size_t size = sizeof(Readable)) { return readInPointer(&readable, size); }

		template <typename ReadableInContainer, typename = IsReadableInContainer<ReadableInContainer>>
		Expected<std::size_t>	readInContainer(ReadableInContainer& readableInContainer) { return readInPointer(readableInContainer.data(), readableInContainer.size()); }

		template <typename ReadableInContainer, typename = IsReadableInContainer<ReadableInContainer>>
		Expected<std::size_t>	readInContainer(ReadableInContainer& readableInContainer, std::size_t size)
		{
			return (size <= readableInContainer.size()) ? readInPointer(readableInContainer.data(), size) : Error(EFAULT);
		}

		/* Read */
		template <typename Readable, typename = IsReadable<Readable>>
		Expected<Readable>	readReadable(std::size_t size = sizeof(Readable))
		{
			Readable readable {};
			auto result = readInReadable(readable, size);
			if (result)
				return readable;
			else
				return Expected<Readable>(result.getError());

		}

		template <typename ReadableContainer, typename = IsReadableContainer<ReadableContainer>>
		Expected<ReadableContainer>	readContainer(std::size_t size)
		{
			ReadableContainer readableContainer;
			readableContainer.resize(size);
			auto result = readInContainer(readableContainer, size);
			if (result)
				return readableContainer;
			else
				return Expected<ReadableContainer>(result.getError());
		}

		/* Read Exact */
		template <typename Readable, typename = IsReadable<Readable>>
		Expected<Readable>	readExactReadable(std::size_t size = sizeof(Readable))
		{
			Readable readable {};
			std::size_t total_readed_size = 0;
			while (total_readed_size < size)
			{
				auto result = readInPointer((&readable) + total_readed_size, size - total_readed_size);
				if (!result)
					return Expected<Readable>(result.getError());
				total_readed_size += result.get();
			}
			return readable;

		}

		template <typename ReadableContainer, typename = IsReadableContainer<ReadableContainer>>
		Expected<ReadableContainer>	readExactContainer(std::size_t size)
		{
			ReadableContainer readableContainer;
			readableContainer.resize(size);
			std::size_t total_readed_size = 0;
			while (total_readed_size < size)
			{
				auto result = readInPointer(&readableContainer[total_readed_size], size - total_readed_size);
				if (!result)
					return Expected<ReadableContainer>(result.getError());
				total_readed_size += result.get();
			}
			return readableContainer;
		}


	protected:
		int _fd = -1;
};

using FdRef = std::reference_wrapper<Fd>;

}