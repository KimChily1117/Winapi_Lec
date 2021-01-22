using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    /*
     처음에는 싱글톤 패턴을 사용해서. Object와 Object를 이어주기 위함이었는데. 

    어짜피 UI를 연결 시켜주려면 만들었어야했음    
     
     */

    public Boss boss;
    public BossMissile bossmissile;
    public MeleeArea bossmelee;

    public PlayerController player;



    static GameManager Instance;
    public static GameManager GetInstance()
    { return Instance; }


    private void Awake()
    {
        Instance = this;
    }

    // Update is called once per frame


    private void Update()
    {
        ProcTarget();
    }

    void ProcTarget()
    {

        bossmissile.player = player.GetComponent<PlayerController>();
        bossmelee.target = player.GetComponent<PlayerController>() ;
        bossmissile.target = player.transform;
    }
}
