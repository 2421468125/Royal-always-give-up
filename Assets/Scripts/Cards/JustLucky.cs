using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JustLucky : BaseCards
{
    public int damage,defend,soul;
    public JustLucky() : base("justLucky")
    {
        damage = 3;
        defend = 2;
        soul = 3;
        helper = new string[2] { "fangyu","hun" };
    }

    public override void Use(Character target)
    {
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        int real_defend = battle_manager.power(2, CardManager.hero, target, defend);
        CardManager.hero.Defend(real_defend);
        int real_soul = battle_manager.power(4, CardManager.hero, CardManager.hero, soul);
        CardManager.hero.AddSoul(real_soul);
    }
}
