using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class mainControl : MonoBehaviour
{
    // Start is called before the first frame update
    public void changebaozao(){
        Hero.persona = 1;
        SceneManager.LoadScene("MapScene");
    }
    public void changefuhua(){
        Hero.persona = 2;
        SceneManager.LoadScene("MapScene");
    }
    public void changetanlan(){
        Hero.persona = 3;
        SceneManager.LoadScene("MapScene");
    }
}
