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

        Enemy[] enemies = new Enemy[CardManager.character_manager.EnemyList.Count];
        for (int i = 0; i < enemies.Length; i++)
            enemies[i] = CardManager.character_manager.EnemyList[i];


        for (int i = 0; i < enemies.Length; i++)
        {
            battle_manager.changeBuf("yishang", CardManager.hero, enemies[i], yishang);
            int real_damage = battle_manager.power(1, CardManager.hero, enemies[i], damage);
            enemies[i].Hurt(real_damage);
        }
    }
}
