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
        helper = new string[2] { "yishang","xuruo" };
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        foreach(Character enemy in character_manager.EnemyList)
        {
            enemy.AddState("xuruo", weak);
            enemy.AddState("yishang", yishang);
        }
        
    }
}



