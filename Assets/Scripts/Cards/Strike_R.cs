using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class Strike_R : BaseCards
{
    public int damage;
    public Strike_R(): base("strike_R")
    {
        damage = 6;
    }

    public override void Use(Character target)
    {
        
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        Debug.Log(target);
    }
}

