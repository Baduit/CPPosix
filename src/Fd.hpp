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
#include <concepts>
#include <ranges>

#include "Expected.hpp"

namespace Cpposix
{

template <typename T>
concept UniqueObjectRepresentation =
	std::has_unique_object_representations_v<T> &&
	!std::ranges::contiguous_range<T> &&
	!std::is_pointer_v<T>;

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

		/* Stream Operator overload */
		template <typename T>
		Fd& operator<<(const T& t)
		{
			write(t);
			return *this;
		}

		/* Write */
		template <typename T>
		Expected<std::size_t>	write(const T* ptr, std::size_t size)
		{
			if (*this)
			{
				auto nb_byte_writed = ::write(_fd, ptr, size);
				if (nb_byte_writed != -1)
					return static_cast<std::size_t>(nb_byte_writed);
				else
					return Error();
			}
			else
				return Error(EBADF);
		}

		template <UniqueObjectRepresentation T>
		Expected<std::size_t>	write(const T& t)
		{
			return write(&t, sizeof(t));
		}

		template <std::ranges::contiguous_range ContiguousRange>
		Expected<std::size_t>	write(const ContiguousRange& contiguous_range)
		{
			auto ptr = std::ranges::data(contiguous_range);
			auto size = std::ranges::size(contiguous_range);
			return write(ptr, size);
		}

		/* Read */
		template <typename T>
		Expected<std::size_t>	read(T* ptr, std::size_t size)
		{
			if (*this)
			{
				auto nb_bytes_readed = ::read(_fd, ptr, size);
				if (nb_bytes_readed != -1)
					return static_cast<std::size_t>(nb_bytes_readed);
				else
					return Error();
			}
			else
				return Error(EBADF);
		}

		template <UniqueObjectRepresentation T>
		Expected<std::size_t>	read(T& t)
		{
			return read(&t, sizeof(t));
		}

		template <std::ranges::contiguous_range ContiguousRange>
		Expected<std::size_t>	read(ContiguousRange& contiguous_range)
		{
			auto ptr = std::ranges::data(contiguous_range);
			auto size = std::ranges::size(contiguous_range);
			return read(ptr, size);
		}


	protected:
		int _fd = -1;
};

using FdRef = std::reference_wrapper<Fd>;

}