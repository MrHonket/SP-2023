//
// Created by Daniel Nykjaer on 28/05/2023.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Simulator/STable.h"

//Unit tests for STable
//Requirement 9

auto testTable = STable<double>{};

TEST_CASE("store function & contains") {

    //initialize test value
    auto returnBool = false;

    //store test objected expected to be found
    testTable.store("testkey",5.0);

    //call contains on the stored testkey
    returnBool = testTable.contains("testkey");

    CHECK(returnBool == true);
}

TEST_CASE("Lookupfunction key is found"){

    //store test object
    testTable.store("testkey",5.0);

    //lookup test object expected to be found
    auto returnValue = testTable.lookup("testkey");

    //check if expected and found values are equal
    CHECK(returnValue == 5.0);
}

TEST_CASE("Lookupfunction key is not found") {

    //call lookup on a key not expected to be found in the table
    auto returnValue = testTable.lookup("badkey");

    //check if the lookup returns a nullopt on bad keys
    CHECK(returnValue == std::nullopt);
}

TEST_CASE("update where key is found") {

    //store key expected to be found and later updated
    testTable.store("testkey",5.0);

    //try to update the key stored before
    testTable.update("testkey",1.0);

    //lookup key expected to be updated
    auto returnValue = testTable.lookup("testkey");

    CHECK(returnValue == 1.0);
}


