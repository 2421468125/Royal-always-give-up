using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LastStrike : BaseCards
{
    public int damage,weak;
    public LastStrike() : base("wildStrike")
    {
        damage = 12;
        weak = 1;
        helper = new string[1] { "xuruo" };
    }

    public override void Use(Character target)
    {

        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        battle_manager.changeBuf("xuruo", CardManager.hero, CardManager.hero, weak);
    }
}
