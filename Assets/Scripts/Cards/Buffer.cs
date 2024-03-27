using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Buffer : BaseCards
{
    public Buffer() : base("buffer")
    {
        isExhaust = true;
        helper = new string[2] { "hun" ,"fangyu"};
    }

    public override void Use(Character target)
    {
        battle_manager.changeBuf("hexinhudun", CardManager.hero, CardManager.hero, 1);
    }
}
