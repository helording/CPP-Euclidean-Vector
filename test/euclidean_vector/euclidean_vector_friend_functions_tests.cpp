#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <istream>
#include <sstream>
#include <string>

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
TEST_CASE("stream output (<<) operator overload tests") {
	SECTION("<< operator printing zero dimensional euclidean_vector test") {
		auto vec = comp6771::euclidean_vector(0, 0);

		std::stringstream test("");
		test << vec;
		CHECK(test.str() == "[]");
	}

	SECTION("<< operator printing one dimensional, magnitude to one-decimal place euclidean_vector "
	        "test") {
		auto vec = comp6771::euclidean_vector(1, 1.6);

		std::stringstream test("");
		test << vec;
		CHECK(test.str() == "[1.600000]");
	}

	SECTION("<< operator printing three dimensional, magnitudes of differing degrees "
	        "euclidean_vector test") {
		auto vec = comp6771::euclidean_vector{1.1, 2.234, 3.3003};

		std::stringstream test("");
		test << vec;
		CHECK(test.str() == "[1.100000 2.234000 3.300300]");
	}
}

TEST_CASE("euclidean_vector equality (= and !=) operators overload tests") {
	auto vec = comp6771::euclidean_vector(3, 1.6);

	SECTION("self assigned == operator test") {
		CHECK(vec == vec);
	}

	SECTION("== operator between two equal, non-zero dimension euclidean_vectors test") {
		CHECK(vec == comp6771::euclidean_vector(3, 1.6));
		CHECK(vec == comp6771::euclidean_vector{1.6, 1.6, 1.6});
		CHECK(comp6771::euclidean_vector() == comp6771::euclidean_vector{});
		CHECK(comp6771::euclidean_vector{1.6, 2.6} == comp6771::euclidean_vector{1.6, 2.6});
	}

	SECTION("== operator between non-zero dimension euclidean_vectors test") {
		CHECK(comp6771::euclidean_vector(0, 0.0) == comp6771::euclidean_vector(0, 0));
	}

	SECTION("!= operator between euclidean_vectors of differing dimensions test") {
		CHECK(vec != comp6771::euclidean_vector(2, 1.6));
		CHECK(vec != comp6771::euclidean_vector(4, 1.6));
	}

	SECTION("!= operator between euclidean_vectors of differing magnitudes test") {
		CHECK(vec != comp6771::euclidean_vector(3, 2.6));
		CHECK(vec != comp6771::euclidean_vector{1.6, 1.6, 1.7});
		CHECK(vec != comp6771::euclidean_vector{1.7, 1.6, 1.6});
		CHECK(vec != comp6771::euclidean_vector{2, 2, 2});
		CHECK(vec != comp6771::euclidean_vector{1, 1, 1});
	}

	SECTION("!= operator between euclidean_vectors of differing magnitudes and dimensions test") {
		CHECK(vec != comp6771::euclidean_vector(4, 2.6));
		CHECK(vec != comp6771::euclidean_vector());
	}

	SECTION("!= operator between zero and non-zero dimensional euclidean_vectors test") {
		auto v0 = comp6771::euclidean_vector(0, 0);

		CHECK(vec != v0);
		CHECK(v0 != comp6771::euclidean_vector());
	}
}

TEST_CASE("euclidean_vector + overload tests") {
	auto v1 = comp6771::euclidean_vector{2, 1.5};
	auto v2 = comp6771::euclidean_vector{2, 1.75};
	auto v1_add = &v1;
	auto v2_add = &v2;

	auto v0 = comp6771::euclidean_vector(0, 0);
	auto v00 = comp6771::euclidean_vector(0, 0);

	SECTION("+ operator out of bounds exception tests") {
		auto not_2d_vec = comp6771::euclidean_vector{3};

		REQUIRE_THROWS_AS(v1 + not_2d_vec, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 + not_2d_vec, "Dimensions of LHS(2) and RHS(1) do not match");
		REQUIRE_THROWS_AS(v1 + v0, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 + v0, "Dimensions of LHS(2) and RHS(0) do not match");
	}

	SECTION("+ operator between two non-zero euclidean_vectors of equal dimension tests") {
		auto copy = v1 + v2;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 4);
		CHECK(copy.at(1) == 3.25);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 2);
		CHECK(v1.at(1) == 1.5);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 2);
		CHECK(v2.at(1) == 1.75);
		CHECK_THROWS(v2.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&v2 == v2_add);
	}

	SECTION("+ operator compound assignment tests") {
		auto copy = v1 + v1 + v2;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 6);
		CHECK(copy.at(1) == 4.75);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 2);
		CHECK(v1.at(1) == 1.5);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 2);
		CHECK(v2.at(1) == 1.75);
		CHECK_THROWS(v2.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&v2 == v2_add);
	}

	SECTION("+ operator operation on self tests") {
		v1 = v1 + v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 4);
		CHECK(v1.at(1) == 3);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("+ operator between two zero dimensional euclidean_vectors test") {
		auto copy = v0 + v00;
		CHECK(copy.dimensions() == 0);
		CHECK_THROWS(copy.at(0));
	}
}

TEST_CASE("euclidean_vector - overload tests") {
	auto v1 = comp6771::euclidean_vector{2, 1.5};
	auto v2 = comp6771::euclidean_vector{1, 1.75};
	auto v1_add = &v1;
	auto v2_add = &v2;

	auto v0 = comp6771::euclidean_vector(0, 0);
	auto v00 = comp6771::euclidean_vector(0, 0);

	SECTION("- operator out of bounds exception tests") {
		auto not_2d_vec = comp6771::euclidean_vector{3};

		REQUIRE_THROWS_AS(v1 - not_2d_vec, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 - not_2d_vec, "Dimensions of LHS(2) and RHS(1) do not match");
		REQUIRE_THROWS_AS(v1 - v0, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 - v0, "Dimensions of LHS(2) and RHS(0) do not match");
	}

	SECTION("- operator between two non-zero euclidean_vectors of equal dimension tests") {
		auto copy = v1 - v2;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 1);
		CHECK(copy.at(1) == -0.25);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 2);
		CHECK(v1.at(1) == 1.5);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 1);
		CHECK(v2.at(1) == 1.75);
		CHECK_THROWS(v2.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&v2 == v2_add);
	}

	SECTION("- operator compound assignment tests") {
		auto v3 = comp6771::euclidean_vector{1, 1};

		auto copy = v1 - v2 - a3;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 0);
		CHECK(copy.at(1) == -1.25);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 2);
		CHECK(v1.at(1) == 1.5);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 1);
		CHECK(v2.at(1) == 1.75);
		CHECK_THROWS(v2.at(2));

		CHECK(v3.dimensions() == 2);
		CHECK(v3.at(0) == 1);
		CHECK(v3.at(1) == 1);
		CHECK_THROWS(v3.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&v2 == v2_add);
	}

	SECTION("- operator operation on self tests") {
		v1 = v1 - v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 0);
		CHECK(v1.at(1) == 0);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("- operator between two zero dimensional euclidean_vectors test") {
		auto copy = v0 - v00;
		CHECK(copy.dimensions() == 0);
		CHECK_THROWS(copy.at(0));
	}
}

TEST_CASE("euclidean_vector * (multiplicaton) overload tests") {
	auto v1 = comp6771::euclidean_vector{-1.5, 2};
	auto v1_add = &v1;

	SECTION("* operator by positive integer") {
		auto copy = v1 * 2;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == -3);
		CHECK(copy.at(1) == 4);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 2);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("* operator by negative integer") {
		auto copy = v1 * -3;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 4.5);
		CHECK(copy.at(1) == -6);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 2);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("* operator by 1") {
		auto copy = v1 * 1;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == -1.5);
		CHECK(copy.at(1) == 2);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 2);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("* operator by 0") {
		auto copy = v1 * 0;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 0);
		CHECK(copy.at(1) == 0);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 2);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("* operator assignment on self") {
		v1 = v1 * 2;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -3);
		CHECK(v1.at(1) == 4);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("* operator on zero dimension euclidean_vector") {
		auto v0 = comp6771::euclidean_vector(0, 0);

		auto copy = v0 * 4;
		CHECK(copy.dimensions() == 0);
		CHECK_THROWS(copy.at(0));
	}
}

TEST_CASE("euclidean_vector / (multiplicaton) overload tests") {
	auto v1 = comp6771::euclidean_vector{-1.5, 3};
	auto v1_add = &v1;

	SECTION("/ operator throws exception at division by 0 case ") {
		REQUIRE_THROWS_AS(v1 / 0, std::logic_error);
		REQUIRE_THROWS_WITH(v1 / 0, "Invalid vector division by 0");
	}

	SECTION("/ operator by positive integer") {
		auto copy = v1 / 3;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == -0.5);
		CHECK(copy.at(1) == 1);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 3);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("/ operator by negative integer") {
		auto copy = v1 / -3;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == 0.5);
		CHECK(copy.at(1) == -1);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 3);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("/ operator by double") {
		auto copy = v1 / 0.5;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == -3);
		CHECK(copy.at(1) == 6);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 3);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("/ operator by 1") {
		auto copy = v1 / 1;

		CHECK(copy.dimensions() == 2);
		CHECK(copy.at(0) == -1.5);
		CHECK(copy.at(1) == 3);
		CHECK_THROWS(copy.at(2));

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 3);
		CHECK_THROWS(v1.at(2));

		CHECK(&v1 == v1_add);
		CHECK(&copy != &v1);
	}

	SECTION("/ operator assignment onto self") {
		v1 = v1 / 0.1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -15);
		CHECK(v1.at(1) == 30);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("/ operator on zero dimension euclidean_vector") {
		auto v0 = comp6771::euclidean_vector(0, 0);

		auto copy = v0 / 4;
		CHECK(copy.dimensions() == 0);
		CHECK_THROWS(copy.at(0));
	}
}

TEST_CASE("euclidean_vector euclidean norm function tests") {
	auto vec = comp6771::euclidean_vector{3, 4};
	auto vec1 = comp6771::euclidean_vector{1, 1};

	SECTION("calculating euclidean norm from non-zero euclidean_vector tests") {
		REQUIRE(5 == euclidean_norm(vec));
	}

	SECTION("euclidean correctly calculated after += operation alters magnitudes") {
		auto vec2 = comp6771::euclidean_vector{5, 7};
		euclidean_norm(vec2);
		vec2 += vec1;
		REQUIRE(10 == euclidean_norm(vec2));
	}

	SECTION("euclidean correctly calculated after -= operation alters magnitudes") {
		auto vec2 = comp6771::euclidean_vector{7, 9};
		euclidean_norm(vec2);
		vec2 -= vec1;
		REQUIRE(10 == euclidean_norm(vec2));
	}

	SECTION("euclidean correctly calculated after *= operation alters magnitudes") {
		euclidean_norm(vec);
		vec *= 2.0;
		REQUIRE(10 == euclidean_norm(vec));
	}

	SECTION("euclidean correctly calculated after /= operation alters magnitudes") {
		euclidean_norm(vec);
		vec /= 0.5;
		REQUIRE(10 == euclidean_norm(vec));
	}

	SECTION("euclidean correctly calculated [] setter operation alters magnitudes") {
		euclidean_norm(vec1);
		vec1[0] = 3;
		vec1[1] = 4;
		REQUIRE(5 == euclidean_norm(vec1));
	}

	SECTION("euclidean correctly calculated .at() setter operation alters magnitudes") {
		euclidean_norm(vec1);
		vec1.at(0) = 6;
		vec1.at(1) = 8;
		REQUIRE(10 == euclidean_norm(vec1));
	}

	SECTION("calculating euclidean norm after copying vector, cache and function related data "
	        "member copied correctly") {
		euclidean_norm(vec1);
		vec1[0] = 3;
		vec1[1] = 4;

		auto copy1 = vec1;
		auto copy2 = comp6771::euclidean_vector(vec1);

		REQUIRE(5 == euclidean_norm(copy1));
		REQUIRE(5 == euclidean_norm(copy2));
	}

	SECTION("calculating euclidean norm after mvoing vector, cache and function related data member "
	        "copied correctly") {
		euclidean_norm(vec);

		auto copy = vec;

		auto move1 = comp6771::euclidean_vector(std::move(vec));
		auto move2 = std::move(copy);

		REQUIRE(5 == euclidean_norm(move1));
		REQUIRE(5 == euclidean_norm(move2));
	}
}

TEST_CASE("euclidean_vector unit vector function tests") {
	SECTION("unit vector function calculates correctly for 2 dimensional vector") {
		auto vec = comp6771::euclidean_vector{3, 4};
		auto vec_unit = unit(vec);

		CHECK(vec_unit.dimensions() == 2);
		CHECK(vec_unit.at(0) == 3.0 / 5.0);
		CHECK(vec_unit.at(1) == 4.0 / 5.0);
		CHECK_THROWS(vec_unit.at(2));
	}

	SECTION("unit vector function calculates correctly for 3 dimensional vector") {
		auto vec = comp6771::euclidean_vector{3, 4, 12};
		auto vec_unit = unit(vec);

		CHECK(vec_unit.dimensions() == 3);
		CHECK(vec_unit.at(0) == 3.0 / 13.0);
		CHECK(vec_unit.at(1) == 4.0 / 13.0);
		CHECK(vec_unit.at(2) == 12.0 / 13.0);
		CHECK_THROWS(vec_unit.at(3));
	}

	SECTION("unit vector function calculates correctly for 5 dimensional vector") {
		auto vec = comp6771::euclidean_vector{1, 2, 2, 2, 6};
		auto vec_unit = unit(vec);

		CHECK(vec_unit.dimensions() == 5);
		CHECK(vec_unit.at(0) == 1.0 / 7.0);
		CHECK(vec_unit.at(1) == 2.0 / 7.0);
		CHECK(vec_unit.at(2) == 2.0 / 7.0);
		CHECK(vec_unit.at(3) == 2.0 / 7.0);
		CHECK(vec_unit.at(4) == 6.0 / 7.0);
		CHECK_THROWS(vec_unit.at(5));
	}

	SECTION("unit vector functions throws exception when dimension is 0 test") {
		auto vec = comp6771::euclidean_vector(0, 0);
		REQUIRE_THROWS_AS(unit(vec), std::invalid_argument);
		REQUIRE_THROWS_WITH(unit(vec),
		                    "euclidean_vector with no dimensions does not have a unit vector");
	}

	SECTION("unit vector functions throws exception when euclidean norm is 0 test") {
		auto vec = comp6771::euclidean_vector();
		REQUIRE_THROWS_AS(unit(vec), std::invalid_argument);
		REQUIRE_THROWS_WITH(unit(vec),
		                    "euclidean_vector with zero euclidean normal does not have a unit "
		                    "vector");

		auto vec2 = comp6771::euclidean_vector(10, 0);
		REQUIRE_THROWS_AS(unit(vec2), std::invalid_argument);
		REQUIRE_THROWS_WITH(unit(vec2),
		                    "euclidean_vector with zero euclidean normal does not have a unit "
		                    "vector");
	}
}

TEST_CASE("euclidean_vector dot product function tests") {
	SECTION("dot product calculates correctly for two euclidean_vector of same dimension test") {
		auto v1 = comp6771::euclidean_vector{3, 4};
		auto v2 = comp6771::euclidean_vector{2, 2};
		REQUIRE(dot(v1, v2) == 14);
	}

	SECTION("dot product calculates correctly for default euclidean_vector test") {
		auto v1 = comp6771::euclidean_vector();
		auto v2 = comp6771::euclidean_vector(1, 5);
		REQUIRE(dot(v1, v2) == 0);
	}

	SECTION("dot product calculates correctly for zero dimension euclidean_vector test") {
		auto v1 = comp6771::euclidean_vector(0, 0);
		auto v00 = comp6771::euclidean_vector(0, 0);
		REQUIRE(dot(v1, v00) == 0);
	}

	SECTION("dot product throws exception for euclidean_vectors of different dimensions test") {
		auto v1 = comp6771::euclidean_vector(3, 3);
		auto v2 = comp6771::euclidean_vector(4, 4);
		REQUIRE_THROWS_AS(dot(v1, v2), std::invalid_argument);
		REQUIRE_THROWS_WITH(dot(v1, v2), "Dimensions of LHS(3) and RHS(4) do not match");
	}
}