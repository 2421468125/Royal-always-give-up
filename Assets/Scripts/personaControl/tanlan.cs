using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class tanlan  : MonoBehaviour
{
    // Start is called before the first frame update


    public float cd = 3f;

    private Text myText;

    private Image myImage;

    private bool beginCD = false;
    private void Awake()
    {
        myImage = GetComponent<Image>();
    }

    private void Update()
    {
        //如果按下鼠标左键
            //要将fillAmount,!!fillAmount的范围只能是0-1，所以还要除以个CD
            // myImage.fillAmount -= Time.deltaTime / cd;

            // myText.text = (myImage.fillAmount * cd).ToString("0.00");
            // if (myImage.fillAmount == 0)
            // {
            //     beginCD = false;
            // }
        if(battleManager.ifGreedyUsed == true) myImage.fillAmount = 0;
        myImage.fillAmount = battleManager.attackCount % 5f / 5f;
    }
}
