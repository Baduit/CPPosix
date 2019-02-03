#pragma once

#include <optional>

#include "Fd.hpp"

namespace Cpposix
{

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
		template <typename ...Args>
		auto	write(Args&&... args) { return _writer.write(args...); }
		
		template <typename ...Args>
		auto	read(Args&&... args) { return _reader.read(args...); }
		
		template <typename ...Args>
		auto	readIn(Args&&... args) { return _reader.readIn(args...); }

		template <typename ...Args>
		auto	readExact(Args&&... args) { return _reader.readExact(args...); }

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
			if (filename[filename.size()])
				throw CpposixException("The string argument is not null terminated");
			_fd = ::open(filename.data(), flags.flags);
		}
};

}