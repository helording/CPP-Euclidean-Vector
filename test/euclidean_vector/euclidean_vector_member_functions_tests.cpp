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
TEST_CASE("euclidean_vector copy assignment overload tests") {
	auto v1 = comp6771::euclidean_vector{1.1, 2.2};
	auto v2 = comp6771::euclidean_vector(1, 5.6);
	auto v0 = comp6771::euclidean_vector(0, 0);

	SECTION("copy assignment test between two non-zero dimension, intialised euclidean_vector") {
		v1 = v2;

		CHECK(v1.dimensions() == 1);
		CHECK(v1.at(0) == 5.6);
		CHECK_THROWS(v1.at(1));

		CHECK(v2.dimensions() == 1);
		CHECK(v2.at(0) == 5.6);
		CHECK_THROWS(v2.at(1));

		CHECK(&v1[0] != &v2[0]);
	}

	SECTION("copy assignment test between unintialised variable and intialised non-zero "
	        "euclidean_vector") {
		auto vec = v1;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 1.1);
		CHECK(vec.at(1) == 2.2);
		CHECK_THROWS(vec.at(2));
		CHECK(&a1[0] != &vec[0]);
	}

	SECTION("copy assignment test between intialised non-zero euclidean_vector and constructor "
	        "called vector") {
		v1 = comp6771::euclidean_vector(1, 1.6);

		CHECK(v1.dimensions() == 1);
		CHECK(v1.at(0) == 1.6);
		CHECK_THROWS(v1.at(1));
	}

	SECTION("self-assignment copy operation test") {
		v1 = v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 1.1);
		CHECK(v1.at(1) == 2.2);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("copy assignment test for zero dimension vector onto intialised euclidean_vector") {
		v1 = v0;

		CHECK(v1.dimensions() == 0);
		CHECK_THROWS(v1.at(0));
		CHECK(&v1[0] != &v0[0]);
	}

	SECTION("copy assignment test for initialised dimension vector onto zero dimension vector") {
		v0 = v2;

		CHECK(v0.dimensions() == 1);
		CHECK(v0.at(0) == 5.6);
		CHECK_THROWS(v0.at(1));
	}
}

TEST_CASE("euclidean_vector move assignment tests") {
	auto vec = comp6771::euclidean_vector(3, 1.6);

	SECTION("move assignment test between two non-zero dimension intialised euclidean_vector") {
		auto from = comp6771::euclidean_vector(1, 5.6);
		vec = std::move(from);

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 5.6);
		CHECK_THROWS(vec.at(1));

		CHECK(from.dimensions() == 0);
		CHECK_THROWS(from.at(0));
	}

	SECTION("self assigned move operator test") {
		vec = std::move(vec);

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));
	}

	SECTION("move assignent test for zero dimension euclidean_vector") {
		auto v0 = comp6771::euclidean_vector(0, 0);
		vec = std::move(v0);

		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));

		CHECK(v0.dimensions() == 0);
		CHECK_THROWS(v0.at(0));
	}
}

TEST_CASE("euclidean_vector unary + overload tests") {
	auto v1 = comp6771::euclidean_vector{-6, 1};
	auto vec_0 = comp6771::euclidean_vector{0, 0};
	auto vec_00 = comp6771::euclidean_vector{0, 0};

	SECTION("unary + assignment unalters original object and returns copy test") {
		auto v2 = +v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -6);
		CHECK(v1.at(1) == 1);
		CHECK_THROWS(v1.at(2));
		CHECK_THROWS(v1.at(-1));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == -6);
		CHECK(v2.at(1) == 1);
		CHECK_THROWS(v2.at(2));
		CHECK_THROWS(v2.at(-1));

		CHECK(&v1[0] != &v2[0]);
	}

	SECTION("unary + assignment on zero deminsion vector tests") {
		CHECK(vec_0 == +vec_00);
	}
}

TEST_CASE("euclidean_vector unary - overload tests") {
	auto v1 = comp6771::euclidean_vector{-6, 1};
	auto vec_0 = comp6771::euclidean_vector{0, 0};
	auto vec_00 = comp6771::euclidean_vector{0, 0};

	SECTION("unary - assignment unalters original object and returns reversed copy test") {
		auto v2 = -v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -6);
		CHECK(v1.at(1) == 1);
		CHECK_THROWS(v1.at(2));
		CHECK_THROWS(v1.at(-1));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 6);
		CHECK(v2.at(1) == -1);
		CHECK_THROWS(v2.at(2));
		CHECK_THROWS(v2.at(-1));
		CHECK(&v1[0] != &v2[0]);
	}

	SECTION("unary - assignment on zero deminsion vector tests") {
		CHECK(vec_0 == -vec_00);
	}
}

TEST_CASE("euclidean_vector += operator overload tests") {
	auto v1 = comp6771::euclidean_vector{4, 3};
	auto v2 = comp6771::euclidean_vector{2, 1.6};
	auto v3 = comp6771::euclidean_vector{1, 1};
	auto v4 = comp6771::euclidean_vector{1.1, 1};
	auto v0 = comp6771::euclidean_vector(0, 0);

	SECTION("+= operator out of bounds exception tests") {
		auto not_2d_vec = comp6771::euclidean_vector{3};

		REQUIRE_THROWS_AS(v1 += not_2d_vec, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 += not_2d_vec, "Dimensions of LHS(2) and RHS(1) do not match");
		REQUIRE_THROWS_AS(v0 += v1, std::invalid_argument);
		REQUIRE_THROWS_WITH(v0 += v1, "Dimensions of LHS(0) and RHS(2) do not match");
	}

	SECTION("+= operator on equally sized euclidean_vectors") {
		v1 += v2;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 6);
		CHECK(v1.at(1) == 4.6);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 2);
		CHECK(v2.at(1) == 1.6);
		CHECK_THROWS(v2.at(2));
	}

	SECTION("+= operator compound assignments") {
		v1 += v2 += v3 += v4;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 8.1);
		CHECK(v1.at(1) == 6.6);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 4.1);
		CHECK(v2.at(1) == 3.6);
		CHECK_THROWS(v2.at(2));

		CHECK(v3.dimensions() == 2);
		CHECK(v3.at(0) == 2.1);
		CHECK(v3.at(1) == 2);
		CHECK_THROWS(v3.at(2));

		CHECK(v4.dimensions() == 2);
		CHECK(v4.at(0) == 1.1);
		CHECK(v4.at(1) == 1);
		CHECK_THROWS(v4.at(2));
	}

	SECTION("+= operator for self assignment") {
		v1 += v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 8);
		CHECK(v1.at(1) == 6);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("+= operator on zero dimension euclidean_vector") {
		auto v00 = comp6771::euclidean_vector(0, 0);
		v0 += v00;
		CHECK(v0.dimensions() == 0);
		CHECK_THROWS(v0.at(0));
	}
}

TEST_CASE("euclidean_vector -= operator overload tests") {
	auto v1 = comp6771::euclidean_vector{-1, 0.25};
	auto v2 = comp6771::euclidean_vector{2, 1.5};
	auto v3 = comp6771::euclidean_vector{1.5, 2};
	auto v0 = comp6771::euclidean_vector(0, 0);

	SECTION("-= operator out of bounds exception tests") {
		auto not_2d_vec = comp6771::euclidean_vector{3};

		REQUIRE_THROWS_AS(v1 -= not_2d_vec, std::invalid_argument);
		REQUIRE_THROWS_WITH(v1 -= not_2d_vec, "Dimensions of LHS(2) and RHS(1) do not match");
		REQUIRE_THROWS_AS(v0 -= v1, std::invalid_argument);
		REQUIRE_THROWS_WITH(v0 -= v1, "Dimensions of LHS(0) and RHS(2) do not match");
	}

	SECTION("-= operator on equally sized euclidean_vectors") {
		v1 -= v2;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -3);
		CHECK(v1.at(1) == -1.25);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 2);
		CHECK(v2.at(1) == 1.5);
		CHECK_THROWS(v2.at(2));
	}

	SECTION("-= operator compound assigmnents") {
		v1 -= v2 -= v3;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == -1.5);
		CHECK(v1.at(1) == 0.75);
		CHECK_THROWS(v1.at(2));

		CHECK(v2.dimensions() == 2);
		CHECK(v2.at(0) == 0.5);
		CHECK(v2.at(1) == -0.5);
		CHECK_THROWS(v2.at(2));

		CHECK(v3.dimensions() == 2);
		CHECK(v3.at(0) == 1.5);
		CHECK(v3.at(1) == 2);
		CHECK_THROWS(v3.at(2));
	}

	SECTION("-= operator for self assignment") {
		v1 -= v1;

		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 0);
		CHECK(v1.at(1) == 0);
		CHECK_THROWS(v1.at(2));
	}

	SECTION("-= operator on zero dimension euclidean_vector") {
		auto v00 = comp6771::euclidean_vector(0, 0);
		v0 -= v00;

		CHECK(v0.dimensions() == 0);
		CHECK_THROWS(v0.at(0));
	}
}

TEST_CASE("euclidean_vector *= operator overload tests") {
	auto vec = comp6771::euclidean_vector{2, -1.6};

	SECTION("*= operator by positive integer") {
		vec *= 2;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 4);
		CHECK(vec.at(1) == -3.2);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator by negative integer") {
		vec *= -2;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == -4);
		CHECK(vec.at(1) == 3.2);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator by double integer") {
		vec *= 0.25;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 0.5);
		CHECK(vec.at(1) == -0.4);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator by 1") {
		vec *= 1;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 2);
		CHECK(vec.at(1) == -1.6);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator by 0") {
		vec *= 0;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 0);
		CHECK(vec.at(1) == 0);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator on one dimension euclidean_vector") {
		auto vec = comp6771::euclidean_vector(1, 5);
		vec *= 2;

		CHECK(vec.dimensions() == 1);
		CHECK(vec.at(0) == 10);
		CHECK_THROWS(vec.at(1));
	}

	SECTION("*= operator on zero dimension euclidean_vector") {
		auto vec = comp6771::euclidean_vector(0, 0);
		vec *= 222;
		CHECK(vec.dimensions() == 0);
		CHECK_THROWS(vec.at(0));
	}
}

TEST_CASE("euclidean_vector /= operator overload tests") {
	auto vec = comp6771::euclidean_vector{2, -1.6};

	SECTION("/= operator by interger") {
		vec /= 2;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 1);
		CHECK(vec.at(1) == -0.8);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("/= operator by negative integer") {
		vec /= -2;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == -1);
		CHECK(vec.at(1) == 0.8);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("/= operator by double") {
		vec /= 0.25;

		CHECK(vec.dimensions() == 2);
		CHECK(vec.at(0) == 8);
		CHECK(vec.at(1) == -6.4);
		CHECK_THROWS(vec.at(2));
	}

	SECTION("*= operator by 0 exception test") {
		REQUIRE_THROWS_AS(vec /= 0, std::logic_error);
		REQUIRE_THROWS_WITH(vec /= 0, "Invalid vector division by 0");
	}
}

TEST_CASE("euclidean_vector casting to std::vector tests") {
	SECTION("Non-zero dimension euclidean_vector cast to std::vector") {
		auto e_vec = comp6771::euclidean_vector{-6, 1};
		auto std_vec = std::vector<double>{-6, 1};
		CHECK(static_cast<std::vector<double>>(e_vec) == std_vec);
	}

	SECTION("Zero dimension euclidean_vector cast to std::vector") {
		auto e_vec = comp6771::euclidean_vector(0, 0);
		auto empty_vec = std::vector<double>();
		CHECK(static_cast<std::vector<double>>(e_vec) == empty_vec);
	}
}

TEST_CASE("euclidean_vector casting to std::list tests") {
	SECTION("Non-zero dimension euclidean_vector cast to std::list") {
		auto e_vec = comp6771::euclidean_vector{-6, 1};
		auto list = std::list<double>{-6, 1};
		CHECK(static_cast<std::list<double>>(e_vec) == list);
	}

	SECTION("Zero dimension euclidean_vector cast to std::list") {
		auto e_vec = comp6771::euclidean_vector(0, 0);
		auto empty_list = std::list<double>();
		CHECK(static_cast<std::list<double>>(e_vec) == empty_list);
	}
}
