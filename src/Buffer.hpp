#pragma once

#include <vector>
#include <array>
#include <cstddef>

#include "Concept.hpp"
#include "Expected.hpp"

namespace Cpposix
{

constexpr std::size_t DEFAULT_STATIC_BUFFER_SIZE = 1024;

class DynamicBuffer
{
	public:
		DynamicBuffer() = default;

		template <typename T>
		DynamicBuffer& operator<<(const T& t) { write(t); return *this; }

		template <typename T>
		DynamicBuffer& operator>>(T& buffer) { readIn(buffer); return *this; }

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
				return {};
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
				return {};
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
				return {};
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
				return {};
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
			std::size_t begin = resize_and_return_old_size(sizeof(Writable) * size);
			auto result = std::memcpy(get_pointer_at(begin), writable_ptr, sizeof(Writable) * size);
			if (result != nullptr)
				return static_cast<std::size_t>(sizeof(Writable)  *size);
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
			size = (size > _container.size()) ? _container.size(): size;
			auto result = std::memcpy(readable_ptr, _container.data(), sizeof(Readable) * size);
			_container.erase(_container.begin(), _container.begin() + static_cast<int>(size));
			if (result != nullptr)
				return static_cast<std::size_t>(sizeof(Readable) * size);
			else
				return Error();
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

	public:
		auto	begin()	{ return _container.begin(); }
		auto	cbegin() const { return _container.cbegin(); }
		auto	end() { return _container.end(); }
		auto	cend() const { return _container.cend(); }

		auto	rbegin()	{ return _container.rbegin(); }
		auto	crbegin() const { return _container.crbegin(); }
		auto	rend() { return _container.rend(); }
		auto	crend() const { return _container.crend(); }

		bool		empty() const { return _container.empty(); }
		std::size_t size() const { return _container.size(); }
		std::size_t max_size() const { return _container.max_size(); }
		std::size_t capacity() const { return _container.capacity(); }

		void	clear() { _container.clear(); }
		void	shrink_to_fit() { _container.shrink_to_fit(); }

		void	reserve(std::size_t new_cap) { _container.reserve(new_cap); }
	

		std::byte	operator[](std::size_t position) { return _container[position]; }
		std::byte	at(std::size_t position) { return _container.at(position); }

		std::byte*			data() { return _container.data(); }
		const std::byte*	data() const { return _container.data(); }

	private:
		std::size_t	resize_and_return_old_size(std::size_t size_to_add)
		{
			std::size_t old_size = _container.size();
			_container.resize(old_size + size_to_add);
			return old_size;
		}

		std::byte*	get_pointer_at(std::size_t position) { return &_container[position]; }

	private:
		std::vector<std::byte> _container;
		// probably store read index
};

template<std::size_t N = DEFAULT_STATIC_BUFFER_SIZE>
class StaticBuffer
{
	public:
		StaticBuffer() = default;

		/* template <typename T>
		StaticBuffer& operator<<(const T& t)
		{
			push(t);
			return *this;
		} */

	public:
		auto	begin()	{ return _container.begin(); }
		auto	cbegin() const { return _container.cbegin(); }
		auto	end() { return _container.end(); }
		auto	cend() const { return _container.cend(); }

		auto	rbegin()	{ return _container.rbegin(); }
		auto	crbegin() const { return _container.crbegin(); }
		auto	rend() { return _container.rend(); }
		auto	crend() const { return _container.crend(); }

		bool		empty() const { return _container.empty(); }
		std::size_t size() const { return _container.size(); }
		std::size_t max_size() const { return _container.max_size(); }

		void	clear() { /*probably memset*/ }

	

		std::byte	operator[](std::size_t position) { return _container[position]; }
		std::byte	at(std::size_t position) { return _container.at(position); }

		std::byte*			data() { return _container.data(); }
		const std::byte*	data() const { return _container.data(); }

	private:
		std::byte*	get_pointer_at(std::size_t position) { return &_container[position]; }

	private:
		std::array<std::byte, N> _container;
		// probably store read and write index
};
}