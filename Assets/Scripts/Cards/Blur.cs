using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Blur : BaseCards
{
    public int defense,soul;
    public Blur() : base("blur")
    {
        defense = 6;
        soul = -6;
        helper = new string[2] { "fangyu","hun" };
    }

    public override void Use(Character target)
    {
        int real_defend = battle_manager.power(2, CardManager.hero, target, defense);
        target.Defend(real_defend);
        int real_soul = battle_manager.power(4, CardManager.hero, CardManager.hero, soul); 
        if (CardManager.hero.soul + real_soul >= 0)
        {
            CardManager.hero.AddSoul(real_soul);
            target.Defend(real_defend);
        }

    }
}

