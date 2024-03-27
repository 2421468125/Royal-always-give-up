using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Undo : BaseCards
{
    public Undo() : base("undo")
    {
        helper = new string[1] { "liliang" };
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
        battle_manager.changeBuf("liliang", CardManager.hero, CardManager.hero, count);
    }
}
