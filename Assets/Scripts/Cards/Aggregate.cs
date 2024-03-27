using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Aggregate : BaseCards
{
    public Aggregate() : base("aggregate")
    {
    }

    public override void Use(Character target)
    {
        int count = 1;
        foreach (KeyValuePair<string, int> buff in CardManager.hero.dynamicBuf)
        {
            if (buff.Value == 0)
                continue;
            if (bufType.dic[buff.Key] == 1)
                count++;
        }
        int real_draw = battle_manager.power(3, CardManager.hero, CardManager.hero, count);
        card_manager.Draw_card(real_draw);
    }
}