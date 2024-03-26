using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShangGong : BaseCards
{
    public int soul;
    public ShangGong() : base("whiteNoise")
    {
        soul = CardManager.hero.money >= 10 ? 10 : CardManager.hero.money;
        helper = new string[1] { "hun" };
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        CardManager.hero.money -= soul;
        int real_soul = battle_manager.power(4, CardManager.hero, CardManager.hero, soul);
        CardManager.hero.AddSoul(real_soul);
    }
}

