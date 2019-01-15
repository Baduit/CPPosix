#pragma once

#include <vector>
#include <array>
#include <cstddef>

#include "Concept.hpp"

namespace Cpposix
{

constexpr std::size_t DEFAULT_STATIC_BUFFER_SIZE = 1024;

class DynamicBuffer
{
	public:
		DynamicBuffer() = default;

		/* template <typename T>
		DynamicBuffer& operator<<(const T& t)
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