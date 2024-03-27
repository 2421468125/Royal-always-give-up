using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class Clothesline : BaseCards {
    public int damage, weak;
    public Clothesline() : base("clothesline")
    {
        damage = 12;
        weak = 2;
        helper = new string[1] { "xuruo" };
    }

    public override void Use(Character target)
    {

        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        battle_manager.changeBuf("xuruo", CardManager.hero, target, weak);
    }
}

