using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class SteamPower : BaseCards
{
    public int draw;
    public SteamPower() : base("steamPower")
    {
        draw = 3;
        helper = new string[1] { "hun"};
    }

    public override void Use(Character target)
    {
        if (CardManager.hero.soul >= 5)
        {
            int real_soul = battle_manager.power(4, CardManager.hero, CardManager.hero, -5);
            CardManager.hero.AddSoul(real_soul);
        }
        int real_draw = battle_manager.power(3, CardManager.hero, CardManager.hero, draw);
        card_manager.Draw_card(real_draw);

    }
}
