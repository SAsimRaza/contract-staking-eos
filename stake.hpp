
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
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
    stake(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds), token_symbol("ASA", 4)
    {
    }
    struct [[eosio::table]] balance
    {
        name staker;
        asset amount;

        auto primary_key() const { return staker.value; }
    };
    typedef eosio::multi_index<"balances"_n, balance> balances;

    // Declare class method.
    [[eosio::on_notify("eosio.token::transfer")]] void
    deposit(name staker, name to, asset quantity);

    [[eosio::action]] void withdraw(name caller);

private:
    const symbol token_symbol;
};