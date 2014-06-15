// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpcclient.h"

#include "rpcprotocol.h"
#include "util.h"
#include "ui_interface.h"
#include "chainparams.h" // for Params().RPCPort()

#include <stdint.h>

using namespace std;
using namespace json_spirit;

template<typename T>
void ConvertTo(Value& value, bool fAllowNull=false)
{
    if (fAllowNull && value.type() == null_type)
        return;
    if (value.type() == str_type)
    {
        // reinterpret string as unquoted json value
        Value value2;
        string strJSON = value.get_str();
        if (!read_string(strJSON, value2))
            throw runtime_error(string("Error parsing JSON:")+strJSON);
        ConvertTo<T>(value2, fAllowNull);
        value = value2;
    }
    else
    {
        value = value.get_value<T>();
    }
}

// Convert strings to command-specific RPC representation
Array RPCConvertValues(const std::string &strMethod, const std::vector<std::string> &strParams)
{
    Array params;
    BOOST_FOREACH(const std::string &param, strParams)
        params.push_back(param);

    int n = params.size();

    //
    // Special case non-string parameter types
    //
    if (strMethod == "stop"                   && n > 0) ConvertTo<bool>(params[0]);
    if (strMethod == "getaddednodeinfo"       && n > 0) ConvertTo<bool>(params[0]);
    if (strMethod == "setgenerate"            && n > 0) ConvertTo<bool>(params[0]);
    if (strMethod == "setgenerate"            && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "getnetworkhashps"       && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "getnetworkhashps"       && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "sendtoaddress"          && n > 1) ConvertTo<double>(params[1]);
    if (strMethod == "settxfee"               && n > 0) ConvertTo<double>(params[0]);
    if (strMethod == "getreceivedbyaddress"   && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "getreceivedbyaccount"   && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "listreceivedbyaddress"  && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "listreceivedbyaddress"  && n > 1) ConvertTo<bool>(params[1]);
    if (strMethod == "listreceivedbyaccount"  && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "listreceivedbyaccount"  && n > 1) ConvertTo<bool>(params[1]);
    if (strMethod == "getbalance"             && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "getblockhash"           && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "move"                   && n > 2) ConvertTo<double>(params[2]);
    if (strMethod == "move"                   && n > 3) ConvertTo<int64_t>(params[3]);
    if (strMethod == "sendfrom"               && n > 2) ConvertTo<double>(params[2]);
    if (strMethod == "sendfrom"               && n > 3) ConvertTo<int64_t>(params[3]);
    if (strMethod == "listtransactions"       && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "listtransactions"       && n > 2) ConvertTo<int64_t>(params[2]);
    if (strMethod == "listaccounts"           && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "walletpassphrase"       && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "getblocktemplate"       && n > 0) ConvertTo<Object>(params[0]);
    if (strMethod == "listsinceblock"         && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "sendmany"               && n > 1) ConvertTo<Object>(params[1]);
    if (strMethod == "sendmany"               && n > 2) ConvertTo<int64_t>(params[2]);
    if (strMethod == "addmultisigaddress"     && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "addmultisigaddress"     && n > 1) ConvertTo<Array>(params[1]);
    if (strMethod == "createmultisig"         && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "createmultisig"         && n > 1) ConvertTo<Array>(params[1]);
    if (strMethod == "listunspent"            && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "listunspent"            && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "listunspent"            && n > 2) ConvertTo<Array>(params[2]);
    if (strMethod == "getblock"               && n > 1) ConvertTo<bool>(params[1]);
    if (strMethod == "getrawtransaction"      && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "createrawtransaction"   && n > 0) ConvertTo<Array>(params[0]);
    if (strMethod == "createrawtransaction"   && n > 1) ConvertTo<Object>(params[1]);
    if (strMethod == "signrawtransaction"     && n > 1) ConvertTo<Array>(params[1], true);
    if (strMethod == "signrawtransaction"     && n > 2) ConvertTo<Array>(params[2], true);
    if (strMethod == "sendrawtransaction"     && n > 1) ConvertTo<bool>(params[1], true);
    if (strMethod == "gettxout"               && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "gettxout"               && n > 2) ConvertTo<bool>(params[2]);
    if (strMethod == "lockunspent"            && n > 0) ConvertTo<bool>(params[0]);
    if (strMethod == "lockunspent"            && n > 1) ConvertTo<Array>(params[1]);
    if (strMethod == "importprivkey"          && n > 2) ConvertTo<bool>(params[2]);
    if (strMethod == "verifychain"            && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "verifychain"            && n > 1) ConvertTo<int64_t>(params[1]);
    if (strMethod == "keypoolrefill"          && n > 0) ConvertTo<int64_t>(params[0]);
    if (strMethod == "getrawmempool"          && n > 0) ConvertTo<bool>(params[0]);
    if (strMethod == "estimatefee"            && n > 0) ConvertTo<boost::int64_t>(params[0]);
    if (strMethod == "estimatepriority"       && n > 0) ConvertTo<boost::int64_t>(params[0]);

    return params;
}

int CommandLineRPC(int argc, char *argv[])
{
    string strPrint;
    int nRet = 0;
    try
    {
        // Skip switches
        while (argc > 1 && IsSwitchChar(argv[1][0]))
        {
            argc--;
            argv++;
        }

        // Method
        if (argc < 2)
            throw runtime_error("too few parameters");
        string strMethod = argv[1];

        // Parameters default to strings
        std::vector<std::string> strParams(&argv[2], &argv[argc]);
        Array params = RPCConvertValues(strMethod, strParams);

        // Execute
        Object reply = CallRPC(strMethod, params);

        // Parse reply
        const Value& result = find_value(reply, "result");
        const Value& error  = find_value(reply, "error");

        if (error.type() != null_type)
        {
            // Error
            strPrint = "error: " + write_string(error, false);
            int code = find_value(error.get_obj(), "code").get_int();
            nRet = abs(code);
        }
        else
        {
            // Result
            if (result.type() == null_type)
                strPrint = "";
            else if (result.type() == str_type)
                strPrint = result.get_str();
            else
                strPrint = write_string(result, true);
        }
    }
    catch (boost::thread_interrupted) {
        throw;
    }
    catch (std::exception& e) {
        strPrint = string("error: ") + e.what();
        nRet = abs(RPC_MISC_ERROR);
    }
    catch (...) {
        PrintExceptionContinue(NULL, "CommandLineRPC()");
        throw;
    }

    if (strPrint != "")
    {
        fprintf((nRet == 0 ? stdout : stderr), "%s\n", strPrint.c_str());
    }
    return nRet;
}

std::string HelpMessageCli(bool mainProgram)
{
    string strUsage;
    if(mainProgram)
    {
        strUsage += _("Options:") + "\n";
        strUsage += "  -?                     " + _("This help message") + "\n";
        strUsage += "  -conf=<file>           " + _("Specify configuration file (default: auroracoin.conf)") + "\n";
        strUsage += "  -datadir=<dir>         " + _("Specify data directory") + "\n";
        strUsage += "  -testnet               " + _("Use the test network") + "\n";
        strUsage += "  -regtest               " + _("Enter regression test mode, which uses a special chain in which blocks can be "
                                                    "solved instantly. This is intended for regression testing tools and app development.") + "\n";
    } else {
        strUsage += _("RPC client options:") + "\n";
    }

    strUsage += "  -rpcconnect=<ip>       " + _("Send commands to node running on <ip> (default: 127.0.0.1)") + "\n";
    strUsage += "  -rpcport=<port>        " + _("Connect to JSON-RPC on <port> (default: 12341 or testnet: 19118)") + "\n";
    strUsage += "  -rpcwait               " + _("Wait for RPC server to start") + "\n";

    if(mainProgram)
    {
        strUsage += "  -rpcuser=<user>        " + _("Username for JSON-RPC connections") + "\n";
        strUsage += "  -rpcpassword=<pw>      " + _("Password for JSON-RPC connections") + "\n";

        strUsage += "\n" + _("SSL options: (see the Bitcoin Wiki for SSL setup instructions)") + "\n";
        strUsage += "  -rpcssl                " + _("Use OpenSSL (https) for JSON-RPC connections") + "\n";
    }

    return strUsage;
}

