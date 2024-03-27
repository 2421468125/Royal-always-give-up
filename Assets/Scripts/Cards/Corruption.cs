using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Corruption : BaseCards
{
    public Corruption() : base("corruption")
    {
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("duoluo", CardManager.hero, CardManager.hero, 1);
    }
}
