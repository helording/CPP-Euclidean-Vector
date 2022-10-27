#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
		friend bool operator==(euclidean_vector const&, euclidean_vector const&) noexcept;
		friend bool operator!=(euclidean_vector const&, euclidean_vector const&) noexcept;
		friend euclidean_vector operator+(euclidean_vector const&, euclidean_vector const&);
		friend euclidean_vector operator-(euclidean_vector const&, euclidean_vector const&);
		friend euclidean_vector operator*(euclidean_vector const&, double) noexcept;
		friend euclidean_vector operator/(euclidean_vector const&, double);
		friend std::ostream& operator<<(std::ostream&, euclidean_vector const&) noexcept;
		friend auto euclidean_norm(euclidean_vector const& v) noexcept -> double;
		friend auto unit(euclidean_vector const& v) -> euclidean_vector;
		friend auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

	public:
		euclidean_vector() noexcept;
		explicit euclidean_vector(int) noexcept;
		euclidean_vector(int, double) noexcept;
		euclidean_vector(std::vector<double>::const_iterator,
		                 std::vector<double>::const_iterator) noexcept;
		euclidean_vector(std::initializer_list<double>) noexcept;
		euclidean_vector(euclidean_vector const&) noexcept;
		euclidean_vector(euclidean_vector&&) noexcept;
		~euclidean_vector() = default;

		euclidean_vector& operator=(euclidean_vector const&) noexcept;
		euclidean_vector& operator=(euclidean_vector&&) noexcept;
		double& operator[](int index) noexcept;
		double operator[](int index) const noexcept;
		euclidean_vector operator+() const noexcept;
		euclidean_vector operator-() const noexcept;
		euclidean_vector& operator+=(euclidean_vector const&);
		euclidean_vector& operator-=(euclidean_vector const&);
		euclidean_vector& operator*=(double) noexcept;
		euclidean_vector& operator/=(double);
		double at(int) const;
		double& at(int);

		int dimensions() const noexcept {
			return dim_;
		}

		explicit operator std::vector<double>() const noexcept {
			auto vector = std::vector<double>(static_cast<size_t>(this->dimensions()));
			std::copy(this->begin(), this->end(), vector.begin());
			return vector;
		}

		explicit operator std::list<double>() const noexcept {
			auto list = std::list<double>(static_cast<size_t>(this->dimensions()));
			std::copy(this->begin(), this->end(), list.begin());
			return list;
		}

	private:
		std::unique_ptr<double[]> magnitude_;
		int dim_;
		mutable bool altered_ = true;
		mutable double cache_;

		void swap(euclidean_vector&) noexcept;

		// Helper function for norm cache
		void update_altered() noexcept {
			this->altered_ = true;
		}

		class Iterator {
		public:
			using iterator_category = std::input_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = double;
			using pointer_type = double*;
			using reference_type = double&;

			Iterator() = default;
			Iterator(pointer_type p)
			: ptr(p) {}

			reference_type operator*() const {
				return *ptr;
			}

			pointer_type operator->() {
				return ptr;
			}

			Iterator& operator++() {
				ptr++;
				return *this;
			}
			Iterator operator++(int) {
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}

			Iterator& operator--() {
				ptr--;
				return *this;
			}
			Iterator operator--(int) {
				Iterator tmp = *this;
				--(*this);
				return tmp;
			}

			Iterator& operator+=(const difference_type offset) {
				ptr += offset;
				return *this;
			}

			Iterator operator+(const difference_type offset) const {
				Iterator tmp = *this;
				return tmp += offset;
			}

			Iterator& operator-=(const difference_type offset) {
				return *this += -offset;
			}

			Iterator operator-(const difference_type offset) const {
				Iterator tmp = *this;
				return tmp -= offset;
			}

			difference_type operator-(const Iterator& right) const {
				return ptr - right.ptr;
			}

			reference_type operator[](const difference_type offset) const {
				return *(*this + offset);
			}

			bool operator<(const Iterator& right) const {
				return ptr < right.ptr;
			}

			bool operator>(const Iterator& right) const {
				return right < *this;
			}

			bool operator<=(const Iterator& right) const {
				return !(right < *this);
			}

			bool operator>=(const Iterator& right) const {
				return !(*this < right);
			}

			friend Iterator operator+(const difference_type offset, const Iterator& it) {
				Iterator tmp = it;
				return tmp += offset;
			}

			friend bool operator==(const Iterator& a, const Iterator& b) {
				return a.ptr == b.ptr;
			}
			friend bool operator!=(const Iterator& a, const Iterator& b) {
				return !(a == b);
			}

		private:
			pointer_type ptr;
		};

		Iterator begin() {
			return Iterator(&magnitude_[0]);
		}
		Iterator end() {
			return Iterator(&magnitude_[static_cast<size_t>(dim_)]);
		}
		Iterator begin() const {
			return Iterator(&magnitude_[0]);
		}
		Iterator end() const {
			return Iterator(&magnitude_[static_cast<size_t>(dim_)]);
		}
	};

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
