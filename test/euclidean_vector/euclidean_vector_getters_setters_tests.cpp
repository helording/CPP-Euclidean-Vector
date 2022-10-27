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
TEST_CASE("euclidean_vector getter tests") {
	auto vec = comp6771::euclidean_vector{1.6, 2.6, 3.6};
	auto vec0 = comp6771::euclidean_vector(0, 0);

	SECTION("euclidean_vector::at getter tests on non-zero dimension euclidean_vector") {
		CHECK(vec.at(0) == 1.6);
		CHECK(vec.at(1) == 2.6);
		CHECK(vec.at(2) == 3.6);
	}

	SECTION("euclidean_vector::at getter exception handling on non-zero dimension "
	        "euclidean_vector") {
		REQUIRE_THROWS_AS(vec.at(3), std::out_of_range);
		REQUIRE_THROWS_WITH(vec.at(3), "Index 3 is not valid for this euclidean_vector object");
		REQUIRE_THROWS_AS(vec.at(-1), std::out_of_range);
		REQUIRE_THROWS_WITH(vec.at(-1), "Index -1 is not valid for this euclidean_vector object");
	}

	SECTION("euclidean_vector::at getter test on 0 dimension euclidean_vector") {
		REQUIRE_THROWS_AS(vec0.at(0), std::out_of_range);
		REQUIRE_THROWS_WITH(vec0.at(0), "Index 0 is not valid for this euclidean_vector object");
	}

	SECTION("euclidean_vector::[] getter tests on non-zero dimension euclidean_vector") {
		CHECK(vec[0] == 1.6);
		CHECK(vec[1] == 2.6);
		CHECK(vec[2] == 3.6);
	}
}

TEST_CASE("euclidean_vector setter tests") {
	auto vec = comp6771::euclidean_vector{1.6, 2.6, 3.6};
	auto vec0 = comp6771::euclidean_vector(0, 0);

	SECTION("euclidean_vector::at setter tests on non-zero dimension euclidean_vector") {
		vec.at(0) = 1.1;
		vec.at(1) = 2.1;
		vec.at(2) = 3.1;

		CHECK(vec.at(0) == 1.1);
		CHECK(vec.at(1) == 2.1);
		CHECK(vec.at(2) == 3.1);
	}

	SECTION("euclidean_vector::at setter exception tests on non-zero dimension euclidean_vector") {
		REQUIRE_THROWS_AS(vec.at(3) = 3.6, std::out_of_range);
		REQUIRE_THROWS_WITH(vec.at(3) = 3.6, "Index 3 is not valid for this euclidean_vector object");
		REQUIRE_THROWS_AS(vec.at(-1) = 3.6, std::out_of_range);
		REQUIRE_THROWS_WITH(vec.at(-1) = 3.6, "Index -1 is not valid for this euclidean_vector object");
	}

	SECTION("euclidean_vector::at setter test on 0 dimension euclidean_vector") {
		REQUIRE_THROWS_AS(vec0.at(0) = 1, std::out_of_range);
		REQUIRE_THROWS_WITH(vec0.at(0) = 1, "Index 0 is not valid for this euclidean_vector object");
	}

	SECTION("euclidean_vector::[] setter tests on 3 dimension, non-zero euclidean_vector") {
		vec[0] = 1.6;
		vec[1] = 2.6;
		vec[2] = 3.6;

		CHECK(vec[0] == 1.6);
		CHECK(vec[1] == 2.6);
		CHECK(vec[2] == 3.6);
	}
}
