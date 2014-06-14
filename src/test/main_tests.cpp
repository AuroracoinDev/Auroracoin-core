// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "core.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    uint64_t nSum = 0;
    uint64_t nSubsidy = GetBlockValue(0, 0);
    nSum += nSubsidy;
    nSubsidy = GetBlockValue(1, 0);
    BOOST_CHECK(nSubsidy == 10500000 * COIN);
    nSum += nSubsidy;

    for (int nHeight = 2; nHeight <= 5450; nHeight++) {
        nSubsidy = GetBlockValue(nHeight, 0);
        BOOST_CHECK(nSubsidy <= 25 * COIN);
        nSum += nSubsidy;
    }
BOOST_CHECK(MoneyRange(nSum));
    for (int nHeight = 5451; nHeight < 6000; nHeight++) {
        nSubsidy = GetBlockValue(nHeight, 0);
        BOOST_CHECK(nSubsidy <= 12.5 * COIN);
        nSum += nSubsidy;
    }
    BOOST_CHECK(MoneyRange(nSum));

    for (int nHeight = 6000; nHeight < 28000000; nHeight += 1000) {
        nSubsidy = GetBlockValue(nHeight, 0);
        BOOST_CHECK(nSubsidy <= 12.5 * COIN);
        nSum += nSubsidy * 1000;
        BOOST_CHECK(MoneyRange(nSum));
    }
    BOOST_CHECK(nSum == 2099311245413000ULL);
}

BOOST_AUTO_TEST_SUITE_END()
