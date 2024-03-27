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

        Enemy[] enemies = new Enemy[CardManager.character_manager.EnemyList.Count];
        for (int i = 0; i < enemies.Length; i++)
            enemies[i] = CardManager.character_manager.EnemyList[i];


        for (int i = 0; i < enemies.Length; i++)
        {
            battle_manager.changeBuf("yishang", CardManager.hero, enemies[i], yishang);
            battle_manager.changeBuf("xuruo", CardManager.hero, enemies[i], weak);
        }
        
    }
}



