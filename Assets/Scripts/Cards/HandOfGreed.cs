using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandOfGreed : BaseCards
{
    public int damage,soul;
    public HandOfGreed() : base("handOfGreed")
    {
        damage = 18;
        soul = 15;
        isArrogant = true;
        helper = new string[2] { "hun","zifu" };
    }

    public override void Use(Character target)
    {

        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        int real_soul = battle_manager.power(4, CardManager.hero, CardManager.hero, soul);
        CardManager.hero.AddSoul(real_soul);

    }
}
