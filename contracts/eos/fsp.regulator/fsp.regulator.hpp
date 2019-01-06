#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>

namespace fsp {
    using std::string;
    class regulator : eosio::contract {
        bool checkTransfer(name sender, name receiver);
        bool checkIssue(name issuer);
        bool checkListing(fsp::security, fsp::venue) 
    }
}