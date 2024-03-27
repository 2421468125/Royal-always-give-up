﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class PommelStrike : BaseCards {
    public int damage;
    public PommelStrike() : base("pommelStrike")
    {
        damage = 9;
    }

    public override void Use(Character target)
    {

        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
        int real_draw = battle_manager.power(3, CardManager.hero, CardManager.hero, 1);
        card_manager.Draw_card(real_draw);
    }
}

