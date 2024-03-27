using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Linghunchongji : BaseCards
{
    public int damage;
    public Linghunchongji() : base("linghunchongji")
    {
        damage = CardManager.hero.soul;
        helper = new string[1] { "hun" };
    }

    public override void Use(Character target)
    {
        damage = CardManager.hero.soul;
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
    }
}

