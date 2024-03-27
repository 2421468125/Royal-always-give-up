using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DemonForm : BaseCards
{
    public DemonForm() : base("demonForm")
    {
        isExhaust = true;
        helper = new string[1] { "hun" };
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("emoqiyue", CardManager.hero, CardManager.hero, 1);
    }
}
