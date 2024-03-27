using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bufType
{
    static public bufType ins;
    static public Dictionary<string,int> dic = new Dictionary<string, int>();
    // Start is called before the first frame update
    public bufType(){
        if (ins != null)
        {
            return;
        }
        else
            ins = this;
        dic.Add("yishang",1);//strange数值代表易伤回合数
        dic.Add("liliang",0);//strange数值代表力量
        dic.Add("xuruo",1); //虚弱回合数量
        dic.Add("minjie",0); //敏捷
        dic.Add("cuiruo",1);
        dic.Add("jingji", 0);
        dic.Add("jingji+", 0);
        dic.Add("fangyu", 0);
        dic.Add("rengongzhipin", 0);
        dic.Add("duoluo",0);
        dic.Add("zuzhou", 1);
        dic.Add("choupaijianyi", 1);
        dic.Add("hexinhudun", 0);
        dic.Add("tongguiyujin", 0);
        dic.Add("chaonengli", 0);
        dic.Add("emoqiyue", 0);
        dic.Add("hunpozaisheng", 0);

    }
}
