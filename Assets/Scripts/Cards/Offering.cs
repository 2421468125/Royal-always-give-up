using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Offering : BaseCards
{
    public int zuzhou;
    public Offering() : base("offering")
    {
        zuzhou = 3;
        helper = new string[1] { "zuzhou" };
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("zuzhou", CardManager.hero, CardManager.hero, zuzhou);
        CardManager.hero.energy += 2;
        card_manager.Draw_card(3);
    }
}