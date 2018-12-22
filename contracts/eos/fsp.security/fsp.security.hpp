#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>


namespace fsp {

    using std::string;
    class security : eosio::contract {

    private:

        struct account {
            eosio::asset    balance;

            uint64_t primary_key()const { return balance.symbol.name(); }
        };

        struct security_stats {
            eosio::asset  supply;
            eosio::asset  max_supply;
            name  issuer;

            uint64_t primary_key()const { return supply.symbol.name(); }
        };

        typedef eosio::multi_index<N(accounts), account> accounts;
        typedef eosio::multi_index<N(stat), security_stats> stats;

        void sub_balance( name owner, eosio::asset value );
        void add_balance( name owner, eosio::asset value, name ram_payer );

    public:

        void create(
                name issuer,
                eosio::asset max_supply
        );

        void issue(
                name to,
                eosio::asset quantity,
                string memo
        );

        void transfer(
                name from,
                name to,
                eosio::asset quantity,
                string memo
        );

        bool validateCreation(name creator);

        bool validateIssue(name to);

        bool validateTransfer(
                eosio::asset quantity,
                name from,
                name to
        );

        eosio::bytes getCorrespondance(
                eosio::symbol_name sym,
                string key
                );

        bool setCorrespondance(
                eosio::symbol_name sym,
                string uri,
                eosio::bytes doc_hash
                );

        inline eosio::asset get_supply(eosio::symbol_name sym) const;

    };
}