
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
using namespace eosio;

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