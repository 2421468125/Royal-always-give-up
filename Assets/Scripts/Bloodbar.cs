using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
public class Bloodbar: MonoBehaviour
{
    public Slider HP;  //实例化一个Slider
    public Text tx;
    private void Start()
    {
        HP.value=1.0f;   //Value的值介于0-1之间，且为浮点数
    }
    // Update is called once per frame
    void Update()
    {

    }
    public void updateslidervalue(Slider slider,float p)
    {
        slider.value=p;
    }
    public void SetBarPosition(Vector3 position)
    {
        if (HP != null)
        {
            HP.transform.localPosition = position;
        }
        else
        {
            Debug.LogError("Bloodbar component not found.");
        }
    }
}
