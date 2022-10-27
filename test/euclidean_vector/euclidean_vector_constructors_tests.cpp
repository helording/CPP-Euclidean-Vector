#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

/*
Testing rationale

The general testing rationale was to test constructors, setter and getters, member functions
and friends in the following order.

Constructors was chosen to be tested first due to all other aspects of the euclidean_vector
class being reliant on the object being constructed correctly. Getters and setters were chosen
next as the testing of the class's constructors and functions relied on getters and setters,
and so they needed to be tested early to ensure tests could be relied upon. Member functions
were tested next as those functions could be used within friend and non-member frunctions.
Lastly friend functions were tested last as they were likely to be the most reliant on functions
tested previously.

Each constructor, function or operator for the class received its own TEST_CASE (with the
exception of the equality and inequality operators). Each use-case for the function then
recieved it's own SECTION, so that test failure could be accuractely identified.

To check the outcome of a test the == operator was used inspect variables of the class.
Conversely the class equality operator could have been used for the same effect however in order
to reduce the overall amount of constructor calls, the former method was chosen.
*/
TEST_CASE("euclidean_vector default constructor test") {
	auto vec = comp6771::euclidean_vector();

	CHECK(vec.dimensions() == 1);
	CHECK(vec.at(0) == 0);
	CHECK_THROWS(vec.at(1));
}

TEST_CASE("euclidean_vector single-arugment constructor tests") {
	SECTION("single-argument constructor argument >1 test") {
		auto vec = comp6771::euclidean_vector(2);

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 0);
		CHECK(vec.at(1) == 0);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("single-argument constructor argument equals 1 test") {
		auto vec = comp6771::euclidean_vector(1);

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 0);
		CHECK_THROWS(vec.at(1));
	}

	SECTION("single-arugment constructor argument equals 0 test") {
		auto vec = comp6771::euclidean_vector(0);

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));
	}
}

TEST_CASE("euclidean_vector magnitude and dimension argument constructor tests") {
	SECTION("arguments dimension >1 magnitude >0 test") {
		auto vec = comp6771::euclidean_vector(3, 5.6);

		CHECK(vec.dimensions() == 3);
		CHECK(vec.at(0) == 5.6);
		CHECK(vec.at(1) == 5.6);
		CHECK(vec.at(2) == 5.6);
		CHECK_THROWS(vec.at(3));
	}

	SECTION("dimension as 1 euclidean_vector constructor test") {
		auto vec = comp6771::euclidean_vector(1, 0);

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 0);
		CHECK_THROWS(vec.at(1));

		auto vec2 = comp6771::euclidean_vector(1, 5.6);

		CHECK(vec2.dimensions() == 1);
		CHECK(vec2.at(0) == 5.6);
		CHECK_THROWS(vec2.at(1));
	}

	SECTION("0 dimension euclidean_vector constructor test") {
		auto vec = comp6771::euclidean_vector(0, 0);

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));

		auto vec2 = comp6771::euclidean_vector(0, 5.6);

		CHECK(vec2.dimensions() == 0);
		CHECK_THROWS(vec2.at(0));
	}
}

TEST_CASE("euclidean_vector vector-iterator constructor tests") {
	auto std_vec = std::vector<double>{1.1, 2.2, 3.3, 4.4};

	SECTION("constructed with unaltered begin and end vector iterators test") {
		auto e_vec = comp6771::euclidean_vector(std_vec.begin(), std_vec.end());

		CHECK(e_vec.dimensions() == 4);
		CHECK(e_vec.at(0) == 1.1);
		CHECK(e_vec.at(1) == 2.2);
		CHECK(e_vec.at(2) == 3.3);
		CHECK(e_vec.at(3) == 4.4);
		CHECK_THROWS(e_vec.at(4));

		CHECK(std_vec == std::vector<double>{1.1, 2.2, 3.3, 4.4});
	}

	SECTION("constructed with altered begin and end vector iterators test") {
		auto e_vec = comp6771::euclidean_vector(std_vec.begin() + 1, std_vec.end() - 1);

		CHECK(e_vec.dimensions() == 2);
		CHECK(e_vec.at(0) == 2.2);
		CHECK(e_vec.at(1) == 3.3);
		CHECK_THROWS(e_vec.at(2));

		CHECK(std_vec == std::vector<double>{1.1, 2.2, 3.3, 4.4});
	}

	SECTION("constructing with one altered vector iterator test") {
		auto e_vec = comp6771::euclidean_vector(std_vec.begin(), std_vec.begin() + 1);

		CHECK(e_vec.dimensions() == 1);
		CHECK(e_vec.at(0) == 1.1);
		CHECK_THROWS(e_vec.at(1));

		CHECK(std_vec == std::vector<double>{1.1, 2.2, 3.3, 4.4});
	}

	SECTION("constructing with equal vector iterator test") {
		auto e_vec = comp6771::euclidean_vector(std_vec.begin() + 1, std_vec.begin() + 1);

		CHECK(e_vec.dimensions() == 0);
		CHECK_THROWS(e_vec.at(0));

		CHECK(std_vec == std::vector<double>{1.1, 2.2, 3.3, 4.4});
	}

	SECTION("euclidean_vector constructed from empty vector") {
		auto std_vec_empty = std::vector<double>();
		auto e_vec = comp6771::euclidean_vector(std_vec_empty.begin(), std_vec_empty.end());

		CHECK(e_vec.dimensions() == 0);
		CHECK_THROWS(e_vec.at(0));

		CHECK(std_vec_empty == std::vector<double>());
	}
}

TEST_CASE("euclidean_vector list-initialiser constructor tests") {
	SECTION("constructed with list-initialiser with multiple elements test") {
		auto vec = comp6771::euclidean_vector{1.1, 2.2, 3.3};

		CHECK(vec.dimensions() == 3);
		CHECK(vec.at(0) == 1.1);
		CHECK(vec.at(1) == 2.2);
		CHECK(vec.at(2) == 3.3);
		CHECK_THROWS(vec.at(3));
	}

	SECTION("constructed with single element list-initialiser test") {
		auto vec = comp6771::euclidean_vector{1.1};

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 1.1);
		CHECK_THROWS(vec.at(1));
	}

	SECTION("constructed with empty list-initialiser test") {
		auto vec = comp6771::euclidean_vector({});

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));
	}

	SECTION("empty curly braces default constructor test") {
		auto vec = comp6771::euclidean_vector{};

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 0);
		CHECK_THROWS(vec.at(1));
	}
}

TEST_CASE("euclidean_vector copy constructor tests") {
	SECTION("vector copy constructed with multiple elements and differing magnitudes test") {
		auto vec = comp6771::euclidean_vector{1.1, 2.2, 3.3};
		auto copy = comp6771::euclidean_vector(vec);

		CHECK(copy.dimensions() == 3);
		CHECK(copy.at(0) == 1.1);
		CHECK(copy.at(1) == 2.2);
		CHECK(copy.at(2) == 3.3);
		CHECK_THROWS(copy.at(3));

		CHECK(vec.dimensions() == 3);
		CHECK(vec.at(0) == 1.1);
		CHECK(vec.at(1) == 2.2);
		CHECK(vec.at(2) == 3.3);
		CHECK_THROWS(vec.at(3));

		CHECK(&vec[0] != &copy[0]);
	}

	SECTION("vector copy constructed through default (single element) euclidean_vector test") {
		auto vec = comp6771::euclidean_vector();
		auto copy = comp6771::euclidean_vector(vec);

		CHECK(copy.dimensions() == 1);
		CHECK(copy.at(0) == 0);
		CHECK_THROWS(copy.at(1));

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 0);
		CHECK_THROWS(vec.at(1));

		CHECK(&vec[0] != &copy[0]);
	}

	SECTION("vector copy constructed through 0 dimension euclidean_vector test") {
		auto vec = comp6771::euclidean_vector(0, 0);
		auto copy = comp6771::euclidean_vector(vec);

		CHECK(copy.dimensions() == 0);
		CHECK_THROWS(copy.at(0));

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));

		CHECK(&vec[0] != &copy[0]);
	}

	SECTION("vector copy constructed and then assigned to self") {
		auto vec = comp6771::euclidean_vector{1.1, 2.2, 3.3};
		vec = comp6771::euclidean_vector(vec);

		CHECK(vec.dimensions() == 3);
		CHECK(vec.at(0) == 1.1);
		CHECK(vec.at(1) == 2.2);
		CHECK(vec.at(2) == 3.3);
		CHECK_THROWS(vec.at(3));
	}
}

TEST_CASE("euclidean_vector move constructor tests") {
	SECTION("vector move constructed with multiple elements and differing magnitudes test") {
		auto from = comp6771::euclidean_vector{1.1, 2.2, 3.3};
		auto to = comp6771::euclidean_vector(std::move(from));

		CHECK(from.dimensions() == 0);
		CHECK_THROWS(from.at(0));

		CHECK(to.dimensions() == 3);
		CHECK(to.at(0) == 1.1);
		CHECK(to.at(1) == 2.2);
		CHECK(to.at(2) == 3.3);
		CHECK_THROWS(to.at(3));
	}

	SECTION("vector move constructed through default (single element) euclidean_vector test") {
		auto from = comp6771::euclidean_vector();
		auto to = comp6771::euclidean_vector(std::move(from));

		CHECK(from.dimensions() == 0);
		CHECK_THROWS(from.at(0));

		CHECK(to.dimensions() == 1);
		CHECK(to.at(0) == 0.0);
		CHECK_THROWS(to.at(1));
	}

	SECTION("vector move constructed through 0 dimension euclidean_vector test") {
		auto from = comp6771::euclidean_vector(0, 0);
		auto to = comp6771::euclidean_vector(std::move(from));

		CHECK(from.dimensions() == 0);
		CHECK_THROWS(from.at(0));

		CHECK(to.dimensions() == 0);
		CHECK_THROWS(to.at(0));
	}
}