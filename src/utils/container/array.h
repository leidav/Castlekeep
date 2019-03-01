#ifndef ARRAY_H
#define ARRAY_H

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>

namespace container
{
template <typename T, typename Allocator = std::allocator<T>>
class Array
{
	using traits = std::allocator_traits<Allocator>;

public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using difference_type = ptrdiff_t;
	using size_type = size_t;

	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using allocator_type = Allocator;

	Array(size_t size, allocator_type allocator = allocator_type())
	    : m_size(size), m_allocator(allocator)
	{
		// m_data = allocator.allocate(size);
		m_data = traits::allocate(allocator, size);
		if constexpr (!(std::is_trivial<T>::value &&
		                std::is_standard_layout<T>::value)) {
			for (size_t i = 0; i < size; i++) {
				// m_allocator.construct(std::addressof(m_data[i]));
				traits::construct(m_allocator, std::addressof(m_data[i]));
			}
		}
	}
	Array(size_t size, const T& value,
	      allocator_type allocator = allocator_type())
	    : m_size(size), m_allocator(allocator)
	{
		// m_data = allocator.allocate(size);
		m_data = traits::allocate(allocator, size);
		for (size_t i = 0; i < size; i++) {
			// m_allocator.construct(std::addressof(m_data[i]), value);
			traits::construct(m_allocator, std::addressof(m_data[i]), value);
		}
	}
	Array(size_t size, const T data[],
	      allocator_type allocator = allocator_type())
	    : m_size(size), m_allocator(allocator)
	{
		// m_data = allocator.allocate(size);
		m_data = traits::allocate(allocator, size);
		for (size_t i = 0; i < size; i++) {
			// m_allocator.construct(std::addressof(m_data[i]), data[i]);
			traits::construct(m_allocator, std::addressof(m_data[i]), data[i]);
		}
	}
	// Array(const Array& array, allocator = allocator_type()) :
	// m_size(array.m_size),m_allocator(allocator)
	Array(const Array& array) : m_size(array.m_size)
	{
		m_allocator =
		    traits::select_on_container_copy_construction(array.m_allocator);
		// m_data = allocator.allocate(m_size);
		m_data = traits::allocate(m_allocator, m_size);
		if constexpr (!(std::is_trivial<T>::value &&
		                std::is_standard_layout<T>::value)) {
			for (size_t i = 0; i < m_size; i++) {
				//	m_allocator.construct(std::addressof(m_data[i]),
				// array.m_data[i]);
				traits::construct(m_allocator, std::addressof(m_data[i]),
				                  array.m_data[i]);
			}
		}
	}
	Array(Array&& array) noexcept : m_size(array.m_size), m_data(array.m_data)
	{
		m_allocator = std::move(array.m_allocator);
		array.array.m_data = nullptr;
	}
	~Array()
	{
		if constexpr (!(std::is_trivial<T>::value &&
		                std::is_standard_layout<T>::value)) {
			for (size_t i = 0; i < m_size; i++) {
				// m_allocator.destroy(std::addressof(m_data[i]));
				traits::destroy(m_allocator, std::addressof(m_data[i]));
			}
		}
		// m_allocator.deallocate(m_data, m_size);
		traits::deallocate(m_allocator, m_data, m_size);
	}

	size_t size() const { return m_size; }

	T* data() { return m_data; }
	const T* data() const { return m_data; }

	Array& operator=(const Array& array)
	{
		if (this != &array) {
			if (m_size != array.m_size) {
				if constexpr (!(std::is_trivial<T>::value &&
				                std::is_standard_layout<T>::value)) {
					for (size_t i = 0; i < m_size; i++) {
						// m_allocator.destroy(std::addressof(m_data[i]));
						traits::destroy(m_allocator, std::addressof(m_data[i]));
					}
				}
				// m_allocator.deallocate(m_data, m_size);
				traits::deallocate(m_allocator, m_data, m_size);

				m_size = array.m_size;
				if (traits::propagate_on_container_copy_assignment::value) {
					m_allocator = array.m_allocator;
				}
				// m_data = m_allocator.allocate(m_size);
				m_data = traits::allocate(m_allocator, m_size);
				for (size_t i = 0; i < m_size; i++) {
					// m_allocator.construct(std::addressof(m_data[i]),
					//                      array.m_data[i]);
					traits::construct(m_allocator, std::addressof(m_data[i]),
					                  array.m_data[i]);
				}
			} else {
				if (traits::propagate_on_container_copy_assignment::value) {
					m_allocator = array.m_allocator;
				}
				std::copy_n(array.m_data, m_size, m_data);
			}
		}
		return *this;
	}

	Array& operator=(Array&& array)
	{
		constexpr bool propagate =
		    traits::propagate_on_container_move_assignment::value;
		if (propagate || m_allocator == array.m_allocator) {
			if constexpr (!(std::is_trivial<T>::value &&
			                std::is_standard_layout<T>::value)) {
				for (size_t i = 0; i < m_size; i++) {
					// m_allocator.destroy(std::addressof(m_data[i]));
					traits::destroy(m_allocator, std::addressof(m_data[i]));
				}
			}
			// m_allocator.deallocate(m_data, m_size);
			traits::deallocate(m_allocator, m_data, m_size);

			m_size = array.m_size;
			m_data = array.m_data;
			if (propagate) {
				m_allocator = std::move(array.m_allocator);
			}
			array.m_data = nullptr;
		} else {
			operator=(array);
		}
		return *this;
	}

	T& operator[](size_t index) { return m_data[index]; }

	const T& operator[](size_t index) const { return m_data[index]; }

	iterator begin() { return m_data; }
	iterator end() { return &m_data[m_size]; }
	const_iterator begin() const { return m_data; }
	const_iterator end() const { return &m_data[m_size]; }
	const_iterator cbegin() const { return m_data; }
	const_iterator cend() const { return &m_data[m_size]; }

	reverse_iterator rbegin()
	{
		return std::reverse_iterator<iterator>(&m_data[m_size]);
	}

	reverse_iterator rend() { return std::reverse_iterator<iterator>(m_data); }

	const_reverse_iterator rbegin() const
	{
		return std::reverse_iterator<const_iterator>(&m_data[m_size]);
	}

	const_reverse_iterator rend() const
	{
		return std::reverse_iterator<iterator>(m_data);
	}

	const_reverse_iterator crbegin() const
	{
		return std::reverse_iterator<const_iterator>(&m_data[m_size]);
	}

	const_reverse_iterator crend() const
	{
		return std::reverse_iterator<const_iterator>(m_data);
	}

	allocator_type get_allocator() const { return m_allocator; }

private:
	size_t m_size;
	T* m_data;
	allocator_type m_allocator;
};
};  // namespace container
#endif
