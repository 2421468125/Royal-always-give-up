using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RecklessCharge : BaseCards
{
    public int damage;
    public RecklessCharge() : base("recklessCharge")
    {
        damage = 40;
        isArrogant = true;
        helper = new string[1] { "zifu"  };
    }

    public override void Use(Character target)
    {

        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        battle_manager.changeBuf("choupaijianyi", CardManager.hero, CardManager.hero, 1);
    }
}

