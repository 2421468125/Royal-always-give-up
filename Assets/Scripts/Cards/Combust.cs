using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Combust : BaseCards
{
    public Combust() : base("combust")
    {
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("tongguiyujin", CardManager.hero, CardManager.hero, 1);
    }
}
