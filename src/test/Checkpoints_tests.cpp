// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p5810 = uint256("0x71517f8219449fd56ade24c888bbfd7d228c898d2aac8a078fd655be4182e813");
    uint256 p6350 = uint256("0x76afd9f23e61b513e0c5224754943a1b1a6ddbed73586416363095808ac12eb1");
    BOOST_CHECK(Checkpoints::CheckBlock(5810, p5810));
    BOOST_CHECK(Checkpoints::CheckBlock(6350, p6350));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(5810, p6350));
    BOOST_CHECK(!Checkpoints::CheckBlock(6350, p5810));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(5810+1, p5810));
    BOOST_CHECK(Checkpoints::CheckBlock(350+1, p6350));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 6350);
}    

BOOST_AUTO_TEST_SUITE_END()
