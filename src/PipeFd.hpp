#pragma once

#include <optional>

#include "Fd.hpp"

namespace Cpposix
{

// Pipe FD seems totally useless
class PipeFd: public Fd
{
	public:
		PipeFd() = default;
		explicit PipeFd(int fd): Fd(fd) {}
};

class Pipe
{
	public:
		Pipe(FdFlags flags = {})
		{
			int fd_array[2];
			pipe2(fd_array, flags.flags);
			_reader = PipeFd(fd_array[0]);
			_writer = PipeFd(fd_array[1]);
		}

		operator bool() const { return _reader && _writer; }

		// big forwarding
		template <typename T>
		Expected<std::size_t>	write(const T* ptr, std::size_t size)
		{
			return _writer.write(ptr, size);
		}

		template <UniqueObjectRepresentation T>
		Expected<std::size_t>	write(const T& t)
		{
			return _writer.write(t);
		}

		template <std::ranges::contiguous_range ContiguousRange>
		Expected<std::size_t>	write(const ContiguousRange& contiguous_range)
		{
			return _writer.write(contiguous_range);
		}

		template <typename T>
		Expected<std::size_t>	read(T* ptr, std::size_t size)
		{
			return _reader.read(ptr, size);
		}

		template <UniqueObjectRepresentation T>
		Expected<std::size_t>	read(T& t)
		{
			return _reader.read(t);
		}

		template <std::ranges::contiguous_range ContiguousRange>
		Expected<std::size_t>	read(ContiguousRange& contiguous_range)
		{
			return _reader.read(contiguous_range);
		}

		PipeFd&	getReaderFd() { return _reader; }
		const PipeFd&	getReaderFd() const { return _reader; }
		PipeFd&	getWriterFd() { return _writer; }
		const PipeFd&	getWriterFd() const { return _writer; }

	private:
		PipeFd _reader;
		PipeFd _writer;
};

class NamedPipeFd: public PipeFd
{
	public:
		NamedPipeFd() = default;
		explicit NamedPipeFd(int fd): PipeFd(fd) {}

		explicit NamedPipeFd(std::string_view filename, FdFlags flags = {})
		{
			check_string_view_null_terminated(filename);
			_fd = ::open(filename.data(), flags.flags);
		}
};

}