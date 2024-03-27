using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreativeAi : BaseCards
{
    public CreativeAi() : base("creativeAi")
    {
        isExhaust = true;
        isEthereal = true;
        helper = new string[2] { "hun", "xuwu" };
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("hunpozaisheng", CardManager.hero, CardManager.hero, 1);
    }
}
