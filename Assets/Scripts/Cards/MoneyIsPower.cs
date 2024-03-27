using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoneyIsPower : BaseCards
{
    public MoneyIsPower() : base("moneyIsPower")
    {
        isExhaust = true;
        helper = new string[2]{ "hun","liliang"};
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("chaonengli", CardManager.hero, CardManager.hero, 1);
    }
}

