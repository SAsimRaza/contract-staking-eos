
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include "events.hpp"

using namespace eosio;

/// triggered when an account initiates a cross chain transafer
#define EMIT_X_TRANSFER_EVENT(blockchain, target, quantity, id) \
    START_EVENT("xtransfer", "1.2")                             \
    EVENTKV("blockchain", blockchain)                           \
    EVENTKV("target", target)                                   \
    EVENTKV("quantity", quantity)                               \
    EVENTKVL("id", id)                                          \
    END_EVENT()

class [[eosio::contract("stake")]] stake : public contract
{
public:
    // Introduce base class members.
    using contract::contract;

    // Use the base class constructor.
    stake(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                   token_symbol("SYS", 4),
                                                                   settings_instance(receiver, receiver.value){};

    struct [[eosio::table]] balance
    {
        name staker;
        asset amount;

        auto primary_key() const { return staker.value; }
    };
    typedef eosio::multi_index<"balances"_n, balance> balances;

    struct [[eosio::table]] settings
    {
        double lockTime;
    } default_settings;

    typedef singleton<"settings"_n, settings> settings_singleton;
    settings_singleton settings_instance;

    // Declare class method.
    [[eosio::on_notify("eosio.token::transfer")]] void
    deposit(name staker, name to, asset quantity);

    [[eosio::action]] void withdraw(name caller);

    [[eosio::action]] void updatesetting(uint64_t _locktime);

private:
    const symbol token_symbol;
};