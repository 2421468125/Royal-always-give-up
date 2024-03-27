using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hunduoshizhong : BaseCards
{
    public int buff;
    public Hunduoshizhong() : base("hunduoshizhong")
    {
        helper = new string[3] { "liliang", "minjie", "hun" };
    }

    public override void Use(Character target)
    {
        buff = CardManager.hero.soul / 5;
        battle_manager.changeBuf("liliang", CardManager.hero, target, buff);
        battle_manager.changeBuf("minjie", CardManager.hero, target, buff);
    }
}
