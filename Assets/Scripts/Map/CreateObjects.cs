using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateObjects : MonoBehaviour
{
    // Start is called before the first frame update
    Vector3 p ;
    void Start()
    {
        MapCreate mapCreate = FindObjectOfType<MapCreate>();
        p.z = 0;
       
        for (int laye = 0; laye < 15;laye++)
          for(int point = 0 ; point < MapCreate.cnt_layer0[laye];point++)
            if(MapCreate.position_per_layer0[laye,point].x != -25)
              {  
                if(MapCreate.type_per0[laye,point] == 1)
                {
                    GameObject e = Instantiate(Resources.Load("rest") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    p.x = MapCreate.position_per_layer0[laye,point].x;
                    p.y = MapCreate.position_per_layer0[laye,point].y;
                    e.transform.localPosition = p;
                }
                if(MapCreate.type_per0[laye,point] == 2)
                {
                    GameObject e = Instantiate(Resources.Load("chest") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    p.x = MapCreate.position_per_layer0[laye,point].x;
                    p.y = MapCreate.position_per_layer0[laye,point].y;
                    e.transform.localPosition = p;
                }
                if(MapCreate.type_per0[laye,point] == 3)
                {
                    GameObject e = Instantiate(Resources.Load("monster") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    p.x = MapCreate.position_per_layer0[laye,point].x;
                    p.y = MapCreate.position_per_layer0[laye,point].y;
                    e.transform.localPosition = p;
                   
                }
                if(MapCreate.type_per0[laye,point] == 4)
                {
                    GameObject e = Instantiate(Resources.Load("boss") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    p.x = MapCreate.position_per_layer0[laye,point].x;
                    p.y = MapCreate.position_per_layer0[laye,point].y;
                    e.transform.localPosition = p;
                }
                if(MapCreate.type_per0[laye,point] == 5)
                {
                    GameObject e = Instantiate(Resources.Load("elite") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    p.x = MapCreate.position_per_layer0[laye,point].x;
                    p.y = MapCreate.position_per_layer0[laye,point].y;
                    e.transform.localPosition = p;
                }
                if (MapCreate.type_per0[laye, point] == 6)
                {
                        GameObject e = Instantiate(Resources.Load("shop") as GameObject);
                        SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                        sceneSwitchMaprelated.laye = laye;
                        sceneSwitchMaprelated.point_num = point;
                        p.x = MapCreate.position_per_layer0[laye, point].x;
                        p.y = MapCreate.position_per_layer0[laye, point].y;
                        e.transform.localPosition = p;
                }
            }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
