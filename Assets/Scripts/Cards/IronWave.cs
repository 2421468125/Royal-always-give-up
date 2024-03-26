using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IronWave : BaseCards
{
    public int damage;
    public int defense;
    public IronWave() : base("ironWave")
    {
        damage = 5;
        defense = 5;
        helper = new string[1] { "fangyu" };
    }

    public override void Use(Character target)
    {
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);

        int real_defend = battle_manager.power(2, CardManager.hero, CardManager.hero, defense);
        CardManager.hero.Defend(real_defend);

    }
}