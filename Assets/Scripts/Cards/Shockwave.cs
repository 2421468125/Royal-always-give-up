using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class Shockwave : BaseCards {
    public int yishang, weak;
    public Shockwave() : base("shockwave")
    {
        yishang = 3;
        weak = 3;
        helper = new string[3] { "yishang","xuruo" ,"xiaohao"};
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        for (int i = CardManager.character_manager.EnemyList.Count - 1; i > -1; i--)
        {
            battle_manager.changeBuf("yishang", CardManager.hero, CardManager.character_manager.EnemyList[i], yishang);
            battle_manager.changeBuf("xuruo", CardManager.hero, CardManager.character_manager.EnemyList[i], weak);
        }

    }
}



