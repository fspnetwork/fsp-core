#include "fsp.security.hpp"

namespace fsp {

    /// Create, Issue and Transfer Functionality

    void security::create(name issuer, eosio::asset max_supply) {
        if( security::validateCreation(issuer) ){
            require_auth( _self );

            auto sym = max_supply.symbol;
            eosio_assert( sym.is_valid(), "invalid symbol name" );
            eosio_assert( max_supply.is_valid(), "invalid supply");
            eosio_assert( max_supply.amount > 0, "max-supply must be positive");

            stats statstable( _self, sym.name() );
            auto existing = statstable.find( sym.name() );
            eosio_assert( existing == statstable.end(), "security with symbol already exists" );

            statstable.emplace( _self, [&]( auto& s ) {
                s.supply.symbol = max_supply.symbol;
                s.max_supply    = max_supply;
                s.issuer        = issuer;
            });
        } else {
            eosio_assert(true, "Invalid creation");
        }
    }

    void security::issue(name to, eosio::asset quantity, std::string memo) {
        if( security::validateIssue(to) ) {
            //eosio_assert(security::validateCreation(to), "Issue validation failed");
            auto sym = quantity.symbol;
            eosio_assert( sym.is_valid(), "invalid symbol name" );
            eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

            auto sym_name = sym.name();
            stats statstable( _self, sym_name );
            auto existing = statstable.find( sym_name );
            eosio_assert( existing != statstable.end(), "security with symbol does not exist, create token before issue" );
            const auto& st = *existing;

            require_auth( st.issuer );
            eosio_assert( quantity.is_valid(), "invalid quantity" );
            eosio_assert( quantity.amount > 0, "must issue positive quantity" );

            eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
            eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

            statstable.modify( st, 0, [&]( auto& s ) {
                s.supply += quantity;
            });

            add_balance( st.issuer, quantity, st.issuer );

            if( to != st.issuer ) {
                SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, to, quantity, memo} );
            }
        } else {
            eosio_assert(true, "Invalid issue.");
        }
    }

    void security::transfer(name from, name to, eosio::asset quantity, std::string memo){
        if( security::validateTransfer(quantity, from, to) ){
            eosio_assert( from != to, "cannot transfer to self" );
            require_auth( from );
            eosio_assert( is_account( to ), "to account does not exist");
            auto sym = quantity.symbol.name();
            stats statstable( _self, sym );
            //_self declared in contract.hpp suggests that this action is paid for by this contract
            const auto& st = statstable.get( sym );

            require_recipient( from );
            require_recipient( to );

            eosio_assert( quantity.is_valid(), "invalid quantity" );
            eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
            eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
            eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );


            sub_balance( from, quantity );
            add_balance( to, quantity, from );
        } else {
            eosio_assert(true, "Invalid transfer");
        }

    }


    void security::sub_balance( name owner, eosio::asset value ) {
        accounts from_acnts( _self, owner );

        const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
        eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


        if( from.balance.amount == value.amount ) {
            from_acnts.erase( from );
        } else {
            from_acnts.modify( from, owner, [&]( auto& a ) {
                a.balance -= value;
            });
        }
    }

    void security::add_balance( name owner, eosio::asset value, name ram_payer ) {
        accounts to_acnts( _self, owner );
        auto to = to_acnts.find( value.symbol.name() );
        if( to == to_acnts.end() ) {
            to_acnts.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            to_acnts.modify( to, 0, [&]( auto& a ) {
                a.balance += value;
            });
        }
    }

    bool security::validateCreation(name creator) {
        return true;
    }

    bool security::validateIssue(name to) {
        return true;
    }

    bool security::validateTransfer(eosio::asset sym, name from, name to) {
        return true;
    }
}

EOSIO_ABI( fsp::security, (create)(issue)(transfer) )
