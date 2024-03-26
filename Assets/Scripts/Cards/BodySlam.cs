using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BodySlam : BaseCards
{
    public int damage;
    public BodySlam() : base("bodySlam")
    {
        damage = 0;
        helper = new string[1] { "fangyu" };
    }

    public override void Use(Character target)
    {
        damage = CardManager.hero.dynamicBuf["fangyu"];
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);

    }
}
