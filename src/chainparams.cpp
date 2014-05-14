// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
    0x3a1f555f, 0xfa9ff802, 0xe404c755, 0xa17ad23e, 0x5a0a3d6c, 0x9f2c63c0, 0xb3d0d162, 0x969d0905,
    0x582463c0, 0xeb70dc55, 0x516b4c90, 0x78c85a4e, 0xf8e25153, 0xcb453fb2, 0xab83a8d9, 0x2aefbf53,
    0x066adc55, 0x36e9ff9f, 0x2490bb25, 0xdef5bb25, 0x9f3edc55, 0x91f1ef2e, 0xf21f9532, 0xebee8052,
    0xf9dba059, 0x6ab7c936, 0xee45ba36, 0xcca3f463, 0xa7df1ed4, 0xb78dd11f, 0x4684a059, 0xc52dcd45,
    0x542f1e32, 0x2029d01f, 0x6ec2d11f, 0xa43e9556, 0x158caa6c, 0x31ce67b0, 0xb812b549, 0xf60adc55,
    0xe7b29d9d, 0x7736dc55, 0x071fdc55, 0x44aea756, 0x4e3e3e2a, 0xa159dc55, 0x421215b9, 0xa788a059,
    0xf2626647, 0x3d116947, 0x97ead536, 0x90dda059, 0x534eee4d, 0x41a99458, 0x3c0bdc55, 0xc6ccd11f,
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xa4;
        pchMessageStart[2] = 0xdc;
        pchMessageStart[3] = 0x6c;
        vAlertPubKey = ParseHex("4104a5814813115273a109cff99907ba4a05d951873dae7acb6c973d0c9e7c88911a3dbc9aa600deac241b91707e7b4ffb30ad91c8e56e695a1ddf318592988afe0aac");
        nDefaultPort = 12340;
        nRPCPort = 12341;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nSubsidyHalvingInterval = 420000;

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        // CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
        //   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
        //     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
        //   vMerkleTree: 4a5e1e
        const char* pszTimestamp = "Visir 10. oktober 2008 Gjaldeyrishoft sett a Islendinga";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04a5814813115273a109cff99907ba4a05d951873dae7acb6c973d0c9e7c88911a3dbc9aa600deac241b91707e7b4ffb30ad91c8e56e695a1ddf318592988afe0a") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1390598806;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();;
        genesis.nNonce   = 538548;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x2a8e100939494904af825b488596ddd536b3a96226ad02e0f7ab7ae472b27a8e"));
        assert(genesis.hashMerkleRoot == uint256("0x8957e5e8d2f0e90c42e739ec62fcc5dd21064852da64b6528ebd46567f222169"));

        vSeeds.push_back(CDNSSeedData("aur.co.vu", "seed.aur.co.vu"));
        vSeeds.push_back(CDNSSeedData("auroracoin.co.vu", "auroracoin.co.vu"));


        base58Prefixes[PUBKEY_ADDRESS] = list_of(23);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(151);

        // Version bytes for HD-wallets. Same as bitcoin's "xprv", "xpub". See: BIP43 and BIP44
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xeb;
        pchMessageStart[1] = 0xb0;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0xcb;
        vAlertPubKey = ParseHex("04302390343f91cc401d56d68b123028bf52e5fca1939df127f63c6467cdf9c8e2c14b61104cf817d0b780da337893ecc4aaff1309e536162dabbdb45200ca2b0a");
        nDefaultPort = 19117;
        nRPCPort = 19118;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1396273344;
        genesis.nNonce = 2543986232;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xf423fe8eb935851f4baa40cfeb7120d84be32adf7876abf0fd293358a90f27a0"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("aur.co.vu", "testnet-seed.aur.co.vu"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(65);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(193);

        // Version bytes for HD-wallets. Same as bitcoin's "tprv", "tpub". See: BIP43 and BIP44
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        assert(hashGenesisBlock == uint256("0x190fc995db81aec386c25616376a0495b5aca80de1cc876e6d59ffe46d11e06a"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
