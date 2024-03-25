using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Anger : BaseCards
{
    public int damage;
    public Anger() : base("anger")
    {
        damage = 6;
    }

    public override void Use(Character target)
    {
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        card_manager.discard_list.Add(new Anger());

    }
}

