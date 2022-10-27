// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <comp6771/euclidean_vector.hpp>

namespace comp6771 {

	// Constructors
	euclidean_vector::euclidean_vector() noexcept
	: euclidean_vector(1, 0) {}

	euclidean_vector::euclidean_vector(int dim) noexcept
	: euclidean_vector(dim, 0) {}

	euclidean_vector::euclidean_vector(int dim, double mag) noexcept
	: magnitude_(std::make_unique<double[]>(static_cast<size_t>(dim)))
	, dim_(dim) {
		for (double& mag_ : *this)
			mag_ = mag;
	}
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator start,
	                                   std::vector<double>::const_iterator end) noexcept
	: euclidean_vector(static_cast<int>(end - start), 0) {
		std::copy(start, end, this->begin());
	}

	euclidean_vector::euclidean_vector(std::initializer_list<double> list_param) noexcept
	: euclidean_vector(static_cast<int>(list_param.size()), 0) {
		std::copy(list_param.begin(), list_param.end(), this->begin());
	}

	euclidean_vector::euclidean_vector(euclidean_vector const& copy) noexcept
	: euclidean_vector(copy.dim_) {
		if (this == &copy)
			return;

		std::swap(this->altered_, copy.altered_);
		std::swap(this->cache_, copy.cache_);
		std::copy(copy.begin(), copy.end(), this->begin());
	}

	euclidean_vector::euclidean_vector(euclidean_vector&& right) noexcept
	: magnitude_(std::exchange(right.magnitude_, std::make_unique<double[]>(static_cast<size_t>(0))))
	, dim_(std::exchange(right.dim_, 0))
	, altered_(std::exchange(right.altered_, true))
	, cache_(std::exchange(right.cache_, 0.0)) {}

	// Swap function for copy and move assignments
	void euclidean_vector::swap(euclidean_vector& other) noexcept {
		std::swap(this->dim_, other.dim_);
		std::swap(this->magnitude_, other.magnitude_);
		std::swap(this->altered_, other.altered_);
		std::swap(this->cache_, other.cache_);
	}

	// Member functions
	euclidean_vector& euclidean_vector::operator=(euclidean_vector const& right) noexcept {
		euclidean_vector(right).swap(*this);
		return *this;
	}

	euclidean_vector& euclidean_vector::operator=(euclidean_vector&& right) noexcept {
		euclidean_vector(right).swap(*this);

		right.dim_ = 0;
		right.altered_ = true;
		right.cache_ = 0.0;
		right.magnitude_ = std::make_unique<double[]>(static_cast<size_t>(0));
		return *this;
	}

	euclidean_vector euclidean_vector::operator+() const noexcept {
		return comp6771::euclidean_vector(*this);
	}

	euclidean_vector euclidean_vector::operator-() const noexcept {
		return comp6771::euclidean_vector(*this) *= -1;
	}

	euclidean_vector& euclidean_vector::operator+=(euclidean_vector const& right) {
		if (this->dimensions() != right.dimensions()) {
			const std::string message = "Dimensions of LHS(" + std::to_string(this->dimensions())
			                            + ") and RHS(" + std::to_string(right.dimensions())
			                            + ") do not match";
			throw std::invalid_argument(message);
		}

		for (auto right_it{right.begin()}; auto& mag : *this)
			mag += *right_it++;
		this->update_altered();
		return *this;
	}

	euclidean_vector& euclidean_vector::operator-=(euclidean_vector const& right) {
		if (this->dimensions() != right.dimensions()) {
			const std::string message = "Dimensions of LHS(" + std::to_string(this->dimensions())
			                            + ") and RHS(" + std::to_string(right.dimensions())
			                            + ") do not match";
			throw std::invalid_argument(message);
		}

		for (auto right_it{right.begin()}; auto& mag : *this)
			mag -= *right_it++;
		this->update_altered();
		return *this;
	}

	euclidean_vector& euclidean_vector::operator*=(double multiple) noexcept {
		std::for_each (this->begin(), this->end(), [&](auto& mag) { mag = mag * multiple; });
		this->update_altered();
		return *this;
	}
	euclidean_vector& euclidean_vector::operator/=(double multiple) {
		if (multiple == 0)
			throw std::logic_error("Invalid vector division by 0");
		return *this *= 1.0 / multiple;
	}

	double& euclidean_vector::operator[](int index) noexcept {
		this->update_altered();
		return this->magnitude_[static_cast<size_t>(index)];
	}

	double euclidean_vector::operator[](int index) const noexcept {
		return this->magnitude_[static_cast<size_t>(index)];
	}

	double euclidean_vector::at(int index) const {
		const std::string message =
		   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
		if (index < 0 || index >= static_cast<int>(this->dim_))
			throw std::out_of_range(message);

		return this->magnitude_[static_cast<size_t>(index)];
	}

	double& euclidean_vector::at(int index) {
		const std::string message =
		   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
		if (index < 0 || index >= static_cast<int>(this->dim_))
			throw std::out_of_range(message);
		this->update_altered();

		return this->magnitude_[static_cast<size_t>(index)];
	}

	// Friend functions
	bool operator==(euclidean_vector const& left, euclidean_vector const& right) noexcept {
		if (left.dimensions() != right.dimensions())
			return false;

		for (auto r{right.begin()}; auto& l : left)
			if (l != *r++)
				return false;
		return true;
	}

	bool operator!=(euclidean_vector const& left, euclidean_vector const& right) noexcept {
		return not(left == right);
	}
	euclidean_vector operator+(euclidean_vector const& left, euclidean_vector const& right) {
		auto vec = comp6771::euclidean_vector(left);
		return vec += right;
	}
	euclidean_vector operator-(euclidean_vector const& left, euclidean_vector const& right) {
		auto vec = comp6771::euclidean_vector(left);
		return vec -= right;
	}

	euclidean_vector operator*(euclidean_vector const& vec, double num) noexcept {
		auto copy = comp6771::euclidean_vector(vec);
		return copy *= num;
	}

	euclidean_vector operator/(euclidean_vector const& vec, double num) {
		auto copy = comp6771::euclidean_vector(vec);
		return copy /= num;
	}

	std::ostream& operator<<(std::ostream& out, euclidean_vector const& vec) noexcept {
		out << std::string("[");

		for (auto it{vec.begin()}; it != vec.end(); ++it) {
			out << std::to_string(*it);
			if (it != vec.end() - 1)
				out << std::string(" ");
		}

		out << std::string("]");
		return out;
	}

	auto euclidean_norm(euclidean_vector const& v) noexcept -> double {
		if (!v.altered_) {
			return v.cache_;
		}
		else {
			double sum{0};
			for (double i : v)
				sum += i * i;
			v.cache_ = std::sqrt(sum);
			v.altered_ = false;
			return v.cache_;
		}
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			const std::string message = "euclidean_vector with no dimensions does not have a unit "
			                            "vector";
			throw std::invalid_argument(message);
		}

		double norm = euclidean_norm(v);
		if (norm == 0) {
			const std::string message = "euclidean_vector with zero euclidean normal does not have a "
			                            "unit vector";
			throw std::invalid_argument(message);
		}

		auto unit_vec = comp6771::euclidean_vector(v);
		for (double& mag : unit_vec)
			mag /= norm;
		return unit_vec;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			const std::string message = "Dimensions of LHS(" + std::to_string(x.dimensions())
			                            + ") and RHS(" + std::to_string(y.dimensions())
			                            + ") do not match";
			throw std::invalid_argument(message);
		}

		auto result{0.0};
		for (auto y_mag{y.begin()}; auto& x_mag : x)
			result += (*y_mag++) * x_mag;
		return result;
	}

} // namespace comp6771
