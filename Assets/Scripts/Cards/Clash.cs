using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class Clash : BaseCards {
    public int damage;
    public Clash() : base("clash")
    {
        damage = 14;
    }

    public override void Use(Character target)
    {
        int real_damage = battle_manager.power(1, CardManager.hero, target, damage);
        target.Hurt(real_damage);
    }

    public override void update_card_use_state()
    {
        can_be_used = false;
        foreach (BaseCards card in card_manager.card_in_hand)
        {
            if (card._type != CardManager.Ctype.ATTACK)
            {
                this.can_be_used = false;
                return;
            }
        }
        can_be_used = true;
    }
}

