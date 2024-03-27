using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Purity : BaseCards
{
    public Purity() : base("purity")
    {
        isExhaust = true;
        helper = new string[1] { "xiaohao" };
    }

    public override void Use(Character target)
    {
        int count = 0;
        List<string> tem = new List<string>();
        foreach (KeyValuePair<string,int> buff in  CardManager.hero.dynamicBuf)
        {
            if (buff.Value == 0)
                continue;
            if (bufType.dic[buff.Key] == 1)
            {
                tem.Add(buff.Key);
                count++;
            }
        }
        foreach(string key in tem)
        {
            CardManager.hero.dynamicBuf[key] = 0;
        }
        CardManager.hero.energy += count;
        CardManager.hero.UpdateState();
    }
}
