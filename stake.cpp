#include "stake.hpp"

void stake::deposit(name staker, name to, asset quantity)
{

    print("Amount Deposited");
    check(quantity.amount > 0, "Should greater than 0");

    if (staker == get_self() || to != get_self())
    {
        return;
    }
    balances balance(get_self(), staker.value);
    auto itr = balance.find(staker.value);

    if (itr != balance.end())
        balance.modify(itr, get_self(), [&](auto &row)
                       { row.amount += quantity; });
    else
        balance.emplace(get_self(), [&](auto &row)
                        { row.amount = quantity;
                        row.staker = staker; });

    EMIT_X_TRANSFER_EVENT("EOS", to, quantity, "1");
}

void stake::withdraw(name caller)
{
    require_auth(caller);

    uint64_t value;

    if (settings_instance.exists())
    {
        value = settings_instance.get().lockTime;
    }
    else
    {
        value = 0;
    }

    check(value < current_time_point().sec_since_epoch(), "time not passed");

    balances balance(get_self(), caller.value);
    auto itr = balance.find(caller.value);

    check(itr != balance.end(), "You're not allowed to party");

    action{
        permission_level{get_self(), "active"_n},
        "eosio.token"_n,
        "transfer"_n,
        std::make_tuple(get_self(), caller, itr->amount, std::string("Withdraw Successfully"))}
        .send();

    balance.erase(itr);
}

void stake::updatesetting(uint64_t _locktime)
{
    require_auth(get_self());

    auto current_setting = settings_instance.get_or_create(get_self(), default_settings);

    current_setting.lockTime = _locktime;

    settings_instance.set(current_setting, get_self());
}
