using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bane : BaseCards
{
    public int yishang, damage;
    public Bane() : base("bane")
    {
        yishang = 3;
        damage = 10; 
        helper = new string[1] { "yishang" };
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("yishang", CardManager.hero, CardManager.hero, yishang);


        for (int i = CardManager.character_manager.EnemyList.Count-1; i >-1 ; i--)
        {
            battle_manager.changeBuf("yishang", CardManager.hero, CardManager.character_manager.EnemyList[i], yishang);
            int real_damage = battle_manager.power(1, CardManager.hero, CardManager.character_manager.EnemyList[i], damage);
            CardManager.character_manager.EnemyList[i].Hurt(real_damage);
        }
    }
}
