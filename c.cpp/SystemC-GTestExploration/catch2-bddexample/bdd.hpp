#pragma once

#if 0

// These macros are already defined in the Catch2 Headers
// https://mariusbancila.ro/blog/2018/03/29/writing-cpp-unit-tests-with-catch2/
// "BDD-style" convenience wrappers
#define SCENARIO( ... ) TEST_CASE( "Scenario: " __VA_ARGS__ )
#define SCENARIO_METHOD( className, ... ) INTERNAL_CATCH_TEST_CASE_METHOD( className, "Scenario: " __VA_ARGS__ )

#define GIVEN( desc )    SECTION( std::string("   Given: ") + desc )
#define WHEN( desc )     SECTION( std::string("    When: ") + desc )
#define AND_WHEN( desc ) SECTION( std::string("And when: ") + desc )
#define THEN( desc )     SECTION( std::string("    Then: ") + desc )
#define AND_THEN( desc ) SECTION( std::string("     And: ") + desc )
#endif

SCENARIO("BDD test with zero", "[bdd]")
{
   WHEN("The number is 0") {
      THEN("The result is 0") {
         REQUIRE(fizzbuzz(0) == "0");
      }
   }
}

SCENARIO("BDD test any number", "[bdd]")
{
   GIVEN("Any positive number")
   {
      WHEN("The number is 1") {
         THEN("The result is 1") {
            REQUIRE(fizzbuzz(1) == "1");
         }
      }
      WHEN("The number is 2") {
         THEN("The result is 2") {
            REQUIRE(fizzbuzz(2) == "2");
         }
      }
      WHEN("The number is 3") {
         THEN("The result is fizz") {
            REQUIRE(fizzbuzz(3) == "fizz");
         }
      }
      WHEN("The number is 4") {
         THEN("The result is 4") {
            REQUIRE(fizzbuzz(4) == "4");
         }
      }
      WHEN("The number is 5") {
         THEN("The result is buzz") {
            REQUIRE(fizzbuzz(5) == "buzz");
         }
      }
      WHEN("The number is a multiple of 3 only") {
         THEN("The result is buzz") {
            for (int i = 3; i <= 100; i += 3) {
               if (i % 5) REQUIRE(fizzbuzz(i) == "fizz");
            }
         }
      }
      WHEN("The number is a multiple of 5 only") {
         THEN("The result is buzz") {
            for (int i = 5; i <= 100; i += 5) {
               if (i % 3) REQUIRE(fizzbuzz(i) == "buzz");
            }
         }
      }
      WHEN("The number is a multiple of 3 and 5") {
         THEN("The result is fizzbuzz") {
            for (int i = 15; i <= 100; i += 15) {
               REQUIRE(fizzbuzz(i) == "fizzbuzz");
            }
         }
      }
   }
   GIVEN("Any negative number") {
      WHEN("The number is -1") {
         THEN("The result is -1") {
            REQUIRE(fizzbuzz(-1) == "-1");
         }
      }
      WHEN("The number is -2") {
         THEN("The result is -2") {
            REQUIRE(fizzbuzz(-2) == "-2");
         }
      }
      WHEN("The number is -3") {
         THEN("The result is fizz") {
            REQUIRE(fizzbuzz(-3) == "fizz");
         }
      }
      WHEN("The number is -4") {
         THEN("The result is -4") {
            REQUIRE(fizzbuzz(-4) == "-4");
         }
      }
      WHEN("The number is -5") {
         THEN("The result is buzz") {
            REQUIRE(fizzbuzz(-5) == "buzz");
         }
      }
   }
}
