#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>


namespace fsp {

    class registry : eosio::contract {
        struct entity {
           name account_name;
           entity_type type;
           eosio::bytes approval_hash;
        }

        bool approve(name account_name, entity_type type);
        
        bool is_registred(name account_name);

        bool is_approved(name account_name);

        status get_status(name account_name);

        entity_type get_entity_type(name account_name);

        eosio::bytes get_approval_hash(name account_name);

    }

    enum status{
        PENDING_APPROVAL, APPROVED, APPROVAL_REJECTED
    }

    enum entity_type{
        INVESTOR, FIRM
    }
}