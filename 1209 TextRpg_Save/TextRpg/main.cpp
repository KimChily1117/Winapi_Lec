#include <iostream>
#include <time.h>

using namespace std;

#define	NAME_SIZE	32
#define	ITEM_DESC_LENGTH	512
#define	INVENTORY_MAX		20
#define	STORE_WEAPON_MAX	3
#define	STORE_ARMOR_MAX		3
#define	LEVEL_MAX			10

// 레벨업에 필요한 경험치 목록을 만든다.
const int	g_iLevelUpExp[LEVEL_MAX] = { 4000, 10000, 20000, 35000, 50000, 70000, 100000, 150000, 200000, 400000 };

enum GAME_MODE
{
	GM_NONE,
	GM_NEW,
	GM_LOAD,
	GM_END
};

enum MAIN_MENU
{
	MM_NONE,
	MM_MAP,
	MM_STORE,
	MM_INVENTORY,
	MM_EXIT
};

enum MAP_TYPE
{
	MT_NONE,
	MT_EASY,
	MT_NORMAL,
	MT_HARD,
	MT_BACK
};

enum JOB
{
	JOB_NONE,
	JOB_KNIGHT,
	JOB_ARCHER,
	JOB_WIZARD,
	JOB_END
};

enum BATTLE
{
	BATTLE_NONE,
	BATTLE_ATTACK,
	BATTLE_BACK
};

enum ITEM_TYPE
{
	IT_NONE,
	IT_WEAPON,
	IT_ARMOR,
	IT_BACK
};

enum STORE_MENU
{
	SM_NONE,
	SM_WEAPON,
	SM_ARMOR,
	SM_BACK
};

enum EQUIP
{
	EQ_WEAPON,
	EQ_ARMOR,
	EQ_MAX
};

struct _tagItem
{
	char	strName[NAME_SIZE];
	char	strTypeName[NAME_SIZE];
	ITEM_TYPE	eType;
	int		iMin;
	int		iMax;
	int		iPrice;
	int		iSell;
	char	strDesc[ITEM_DESC_LENGTH];
};

struct _tagInventory
{
	_tagItem	tItem[INVENTORY_MAX];
	int			iItemCount;
	int			iGold;
};

struct _tagPlayer
{
	char	strName[NAME_SIZE];
	char	strJobName[NAME_SIZE];
	JOB		eJob;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iHP;
	int		iHPMax;
	int		iMP;
	int		iMPMax;
	int		iExp;
	int		iLevel;
	_tagItem	tEquip[EQ_MAX];
	bool		bEquip[EQ_MAX];
	_tagInventory	tInventory;
};


struct _tagMonster
{
	char	strName[NAME_SIZE];
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iHP;
	int		iHPMax;
	int		iMP;
	int		iMPMax;
	int		iLevel;
	int		iExp;
	int		iGoldMin;
	int		iGoldMax;
};


struct _tagLevelUpStatus
{
	int	iAttackMin;
	int	iAttackMax;
	int	iArmorMin;
	int	iArmorMax;
	int	iHPMin;
	int	iHPMax;
	int	iMPMin;
	int	iMPMax;
};

// JOB_END 는 4이다. 그런데 직업은 3개이므로 -1을 해주어서 배열을 각 직업별로
	// 생성하도록 한다.
_tagLevelUpStatus	g_tLvUpTable[JOB_END - 1] = {};


int InputInt() // 숫자 입력을 받고 오류처리
{
	int iInput;

	cin >> iInput;

	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1024, '\n');
		return INT_MAX; //INT_MAX가 반환되면 입력이 잘못됬다는거
	}
	return iInput;

}



int OutputMainMenu()
{

	cout << "*************************** 로비 ***************************" << endl;
	cout << "1. 맵" << endl;
	cout << "2. 상점" << endl;
	cout << "3. 가방" << endl;
	cout << "4. 종료" << endl;
	cout << "메뉴를 선택하세요 : ";

	int iMenu = InputInt();

	if (iMenu == INT_MAX || iMenu <= MM_NONE || iMenu > MM_EXIT)
	{
		return MM_NONE;
	}

	return iMenu;
}


void OutputBattleTag(int iMenu)
{
	system("cls");
	switch (iMenu)
	{
	case MT_EASY:
		cout << "*************************** 쉬움 ***************************" << endl;
		break;
	case MT_NORMAL:
		cout << "*************************** 보통 ***************************" << endl;
		break;
	case MT_HARD:
		cout << "*************************** 어려움 ***************************" << endl;
		break;
	}
}

void OutputPlayer(_tagPlayer* pPlayer)
{
	// 플레이어 정보를 출력한다.
	cout << "================== Player ==================" << endl;
	cout << "이름 : " << pPlayer->strName << "\t직업 : " <<
		pPlayer->strJobName << endl;
	cout << "레벨 : " << pPlayer->iLevel << "\t경험치 : " <<
		pPlayer->iExp << " / " << g_iLevelUpExp[pPlayer->iLevel - 1] << endl;

	// 무기를 장착하고 있을 경우 공격력에 무기공격력을 추가하여 출력한다.
	if (pPlayer->bEquip[EQ_WEAPON] == true)
	{
		cout << "공격력 : " << pPlayer->iAttackMin << " + " <<
			pPlayer->tEquip[EQ_WEAPON].iMin << " - " <<
			pPlayer->iAttackMax << " + " << pPlayer->tEquip[EQ_WEAPON].iMax;
	}

	else
	{
		cout << "공격력 : " << pPlayer->iAttackMin << " - " <<
			pPlayer->iAttackMax;
	}

	// 방어구를 장착하고 있을 경우 방어력에 방어구 방어력을 추가하여 출력한다.
	if (pPlayer->bEquip[EQ_ARMOR] == true)
	{
		cout << "\t방어력 : " << pPlayer->iArmorMin << " + " <<
			pPlayer->tEquip[EQ_ARMOR].iMin << " - " <<
			pPlayer->iArmorMax << " + " << pPlayer->tEquip[EQ_ARMOR].iMax << endl;
	}

	else
	{
		cout << "\t방어력 : " << pPlayer->iArmorMin << " - " <<
			pPlayer->iArmorMax << endl;
	}

	cout << "체력 : " << pPlayer->iHP << " / " << pPlayer->iHPMax <<
		"\t마나 : " << pPlayer->iMP << " / " << pPlayer->iMPMax << endl;

	if (pPlayer->bEquip[EQ_WEAPON])
		cout << "장착무기 : " << pPlayer->tEquip[EQ_WEAPON].strName;

	else
		cout << "장착무기 : 없음";

	if (pPlayer->bEquip[EQ_ARMOR])
		cout << "\t장착방어구 : " << pPlayer->tEquip[EQ_ARMOR].strName << endl;

	else
		cout << "\t장착방어구 : 없음" << endl;

	cout << "보유골드 : " << pPlayer->tInventory.iGold << " Gold" << endl << endl;



}

void OutputMonster(_tagMonster* pMonster)
{
	// 몬스터 정보 출력
	cout << "================== Monster ==================" << endl;
	cout << "이름 : " << pMonster->strName << "\t레벨 : " <<
		pMonster->iLevel << endl;
	cout << "공격력 : " << pMonster->iAttackMin << " - " <<
		pMonster->iAttackMax << "\t방어력 : " << pMonster->iArmorMin <<
		" - " << pMonster->iArmorMax << endl;
	cout << "체력 : " << pMonster->iHP << " / " << pMonster->iHPMax <<
		"\t마나 : " << pMonster->iMP << " / " << pMonster->iMPMax << endl;
	cout << "획득경험치 : " << pMonster->iExp << "\t획득골드 : " <<
		pMonster->iGoldMin << " - " << pMonster->iGoldMax << endl << endl;
}

int OutputBattleMenu()
{
	cout << "1. 공격" << endl;
	cout << "2. 도망가기" << endl;
	cout << "메뉴를 선택하세요 : ";
	int iMenu = InputInt();

	if (iMenu == INT_MAX || iMenu <= BATTLE_NONE || iMenu > BATTLE_BACK)
	{
		return BATTLE_NONE;
	}

	return iMenu;
}

void Battle(_tagPlayer* pPlayer, _tagMonster* pMonster)
{
	// 예를 들어 Min 5 Max 15 라고 가정할 경우 
				   // 15 - 5 + 1 을 하면 11이 된다. 11로 나눈 나머지는 0 ~ 10이
				   // 나오게 되고 여기에 Min값인 5를 더하게 되면
				   // 5 ~ 15 사이로 값이 나오게 되는것이다.
	int   iAttackMin = pPlayer->iAttackMin;
	int   iAttackMax = pPlayer->iAttackMax;

	// 무기를 장착하고 있을 경우 무기와 Min, Max를 더해준다.
	if (pPlayer->bEquip[EQ_WEAPON])
	{
		iAttackMin += pPlayer->tEquip[EQ_WEAPON].iMin;
		iAttackMax += pPlayer->tEquip[EQ_WEAPON].iMax;
	}

	int   iAttack = rand() % (iAttackMax - iAttackMin + 1) +
		iAttackMin;
	int   iArmor = rand() % (pMonster->iArmorMax - pMonster->iArmorMin + 1) +
		pMonster->iArmorMin;

	int   iDamage = iAttack - iArmor;
	// 삼항연산자 : 조건식 ? true일때값 : false일때값;
	//if (iDamage < 1)
	//   iDamage = 1;
	iDamage = iDamage < 1 ? 1 : iDamage;

	// 몬스터 HP를 감소시킨다.
	pMonster->iHP -= iDamage;

	cout << pPlayer->strName << " 가 " << pMonster->strName <<
		"에게 " << iDamage << " 피해를 입혔습니다." << endl;

	// 몬스터가 죽었을 경우를 처리한다.
	if (pMonster->iHP <= 0)
	{
		cout << pMonster->strName << " 몬스터가 사망하였습니다." << endl;

		pPlayer->iExp += pMonster->iExp;
		int   iGold = (rand() % (pMonster->iGoldMax - pMonster->iGoldMin + 1) +
			pMonster->iGoldMin);
		pPlayer->tInventory.iGold += iGold;

		cout << pMonster->iExp << " 경험치를 획득하였습니다." << endl;
		cout << iGold << " Gold를 획득하였습니다." << endl;

		pMonster->iHP = pMonster->iHPMax;
		pMonster->iMP = pMonster->iMPMax;

		// 레벨업을 했는지 체크해본다.
		if (pPlayer->iExp >= g_iLevelUpExp[pPlayer->iLevel - 1])
		{
			// 플레이어 경험치를 레벨업에 필요한 경험치만큼 차감한다.
			pMonster->iExp -= g_iLevelUpExp[pPlayer->iLevel - 1];

			// 레벨을 증가시킨다.
			++pPlayer->iLevel;

			cout << "레벨업 하였습니다." << endl;

			// 능력치를 상승시킨다.
			// 직업 인덱스를 구한다.
			int   iJobIndex = pPlayer->eJob - 1;
			int   iHPUp = rand() % (g_tLvUpTable[iJobIndex].iHPMax - g_tLvUpTable[iJobIndex].iHPMin + 1) +
				g_tLvUpTable[iJobIndex].iHPMin;
			int   iMPUp = rand() % (g_tLvUpTable[iJobIndex].iMPMax - g_tLvUpTable[iJobIndex].iMPMin + 1) +
				g_tLvUpTable[iJobIndex].iMPMin;

			pPlayer->iAttackMin += g_tLvUpTable[iJobIndex].iAttackMin;
			pPlayer->iAttackMax += g_tLvUpTable[iJobIndex].iAttackMax;
			pPlayer->iArmorMin += g_tLvUpTable[iJobIndex].iArmorMin;
			pPlayer->iArmorMax += g_tLvUpTable[iJobIndex].iArmorMax;
			pPlayer->iHPMax += iHPUp;
			pPlayer->iMPMax += iMPUp;

			// 체력과 마나를 회복시킨다.
			pPlayer->iHP = pPlayer->iHPMax;
			pPlayer->iMP = pPlayer->iMPMax;
		}
		return;
	}

	// 몬스터가 살아있다면 플레이어를 공격한다.
	iAttack = rand() % (pMonster->iAttackMax - pMonster->iAttackMin + 1) +
		pMonster->iAttackMin;

	int   iArmorMin = pPlayer->iArmorMin;
	int   iArmorMax = pPlayer->iArmorMax;

	if (pPlayer->bEquip[EQ_ARMOR])
	{
		iArmorMin += pPlayer->tEquip[EQ_ARMOR].iMin;
		iArmorMax += pPlayer->tEquip[EQ_ARMOR].iMax;
	}

	iArmor = rand() % (iArmorMax - iArmorMin + 1) +
		iArmorMin;

	iDamage = iAttack - iArmor;
	// 삼항연산자 : 조건식 ? true일때값 : false일때값;
	//if (iDamage < 1)
	//   iDamage = 1;
	iDamage = iDamage < 1 ? 1 : iDamage;

	// 플레이어의 HP를 감소시킨다.
	pPlayer->iHP -= iDamage;

	cout << pMonster->strName << " 가 " << pPlayer->strName <<
		"에게 " << iDamage << " 피해를 입혔습니다." << endl;

	// 플레이어가 죽었을 경우
	if (pPlayer->iHP <= 0)
	{
		cout << pPlayer->strName << " 플레이어가 사망하였습니다." << endl;

		int   iExp = pPlayer->iExp * 0.1f;
		int   iGold = pPlayer->tInventory.iGold * 0.1f;

		pPlayer->iExp -= iExp;
		pPlayer->tInventory.iGold -= iGold;

		cout << iExp << " 경험치를 잃었습니다." << endl;
		cout << iGold << " Gold를 잃었습니다." << endl;

		// 플레이어의 HP와 MP를 회복한다.
		pPlayer->iHP = pPlayer->iHPMax;
		pPlayer->iMP = pPlayer->iMPMax;
	}

	system("pause");
}

void RunBattle(_tagPlayer* pPlayer, _tagMonster* pMonsterArr, int iMenu)
{
	_tagMonster tMonster = pMonsterArr[iMenu - 1];

	while (true)
	{
		system("cls");
		OutputBattleTag(iMenu);

		// 플레이어 출력 

		OutputPlayer(pPlayer); // 얘는 위에 매개변수로 포인터를 받아왔고
		OutputMonster(&tMonster); // 얘는 매개변수로 배열을 받았다가 다시 구조체로 선언만햇으니
								  // 다시 주소값을 넘겨줘야함. 애초에 에러가 나서 되지도않는다. 

		switch (OutputBattleMenu()) // fsm 스테이트패턴과 매우 유사 
		{
		case BATTLE_ATTACK:
			Battle(pPlayer,&tMonster);
			system("pause");
			break;
		case BATTLE_BACK:
			return;
		}
	}
}

int OutPutMapMenu()
{
	system("cls");
	cout << "*************************** 맵 ***************************" << endl;
	cout << "1. 쉬움" << endl;
	cout << "2. 보통" << endl;
	cout << "3. 어려움" << endl;
	cout << "4. 뒤로가기" << endl;
	cout << "맵을 선택하세요 : ";
	int iMenu = InputInt();

	if (iMenu == INT_MAX || iMenu <= MT_NONE || iMenu > MT_BACK)
	{
		return MT_NONE;
	}

	return iMenu;

	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1024, '\n');
		return INT_MAX;
	}

}

//맵에 관련된 동작 
void RunMap(_tagPlayer* pPlayer, _tagMonster* pMonsterArr)
{
	bool bLoop = true;

	while (bLoop)
	{
		int iMenu = OutPutMapMenu();

		if (MT_BACK == iMenu)
		{
			return;
		}


		//전투를 구성한다 
		RunBattle(pPlayer, pMonsterArr, iMenu);
	}

}



int SelectJob()
{
	int	iJob = JOB_NONE;
	while (iJob == JOB_NONE)
	{
		system("cls");
		cout << "1. 기사" << endl;
		cout << "2. 궁수" << endl;
		cout << "3. 마법사" << endl;
		cout << "직업을 선택하세요 : ";
		iJob = InputInt();


		if (iJob <= JOB_NONE || iJob >= JOB_END)
			iJob = JOB_NONE;
	}

	return iJob;
}

void SetPlayer(_tagPlayer* pPlayer) // 매개변수를 포인터로 받아야 값에 지장을 줄수있음
//call by address
{
	/* 그대로 개행문자를 이름으로 받아들여서 이름 입력받는부분이 넘어감
	그래서 cin.clear함수로 청소해주고 다시 입력을 받아준다. 

	그리고 화면도 system("cls")로 한번 없애줌 
	*/

	system("cls");
	cin.clear();
	cin.ignore(1024,'\n');

	cout << "이름 : " << endl;

	cin.getline(pPlayer->strName, NAME_SIZE - 1);

	pPlayer->eJob = (JOB)SelectJob();
	pPlayer->iLevel = 1;
	pPlayer->iExp = 0;
	pPlayer->tInventory.iGold = 10000;

	switch (pPlayer->eJob)
	{
	case JOB_KNIGHT:
		strcpy_s(pPlayer->strJobName, "기사");
		pPlayer->iAttackMin = 5;
		pPlayer->iAttackMax = 10;
		pPlayer->iArmorMin = 15;
		pPlayer->iArmorMax = 20;
		pPlayer->iHPMax = 500;
		pPlayer->iHP = 500;
		pPlayer->iMP = 100;
		pPlayer->iMPMax = 100;
		break;
	case JOB_ARCHER:
		strcpy_s(pPlayer->strJobName, "궁수");
		pPlayer->iAttackMin = 10;
		pPlayer->iAttackMax = 15;
		pPlayer->iArmorMin = 10;
		pPlayer->iArmorMax = 15;
		pPlayer->iHPMax = 400;
		pPlayer->iHP = 400;
		pPlayer->iMP = 200;
		pPlayer->iMPMax = 200;
		break;
	case JOB_WIZARD:
		strcpy_s(pPlayer->strJobName, "마법사");
		pPlayer->iAttackMin = 15;
		pPlayer->iAttackMax = 20;
		pPlayer->iArmorMin = 5;
		pPlayer->iArmorMax = 10;
		pPlayer->iHPMax = 300;
		pPlayer->iHP = 300;
		pPlayer->iMP = 300;
		pPlayer->iMPMax = 300;
		break;
	}

}

bool LoadPlayer(_tagPlayer * pPlayer)
{
	FILE* pFile = NULL;

	/*char	strName[NAME_SIZE];
	char	strJobName[NAME_SIZE];
	JOB		eJob;
	int		iAttackMin;
	int		iAttackMax;
	int		iArmorMin;
	int		iArmorMax;
	int		iHP;
	int		iHPMax;
	int		iMP;
	int		iMPMax;
	int		iExp;
	int		iLevel;
	_tagItem	tEquip[EQ_MAX];
	bool		bEquip[EQ_MAX];
	_tagInventory	tInventory;*/

	

	fopen_s(&pFile, "player.ply","rb");

	if (pFile)
	{
		// 이름 읽어오기 
		fread(pPlayer->strName, 1, NAME_SIZE, pFile);

		//직업 정보,이름 가져오기 
		fread(&pPlayer->eJob, sizeof(JOB), 1, pFile);
		fread(pPlayer->strJobName, 1, NAME_SIZE, pFile);

		//공격력 읽어오기 
		fread(&pPlayer->iAttackMin, 4, 1, pFile);
		fread(&pPlayer->iAttackMax, 4, 1, pFile);

		//방어력 읽어오기 
		fread(&pPlayer->iArmorMin, 4, 1, pFile);
		fread(&pPlayer->iArmorMax, 4, 1, pFile);

		//체력 읽어오기
		fread(&pPlayer->iHP, 4, 1, pFile);
		fread(&pPlayer->iHPMax, 4, 1, pFile);

		//마나 읽어오기
		fread(&pPlayer->iMP, 4, 1, pFile);
		fread(&pPlayer->iMPMax, 4, 1, pFile);

		//경험치와 레벨을 읽어온다.

		fread(&pPlayer->iExp, sizeof(int), 1, pFile); // 요렇게 해도 똑같음 int는 4바이트니깐
		fread(&pPlayer->iLevel,sizeof(int), 1, pFile);

		//무기아이템 착용 여부를 읽어온다. 
		fread(&pPlayer->bEquip[EQ_WEAPON], sizeof(bool), 1, pFile);

		// 
		if (pPlayer->bEquip[EQ_WEAPON])
		{
			fread(&pPlayer->tEquip[EQ_WEAPON], sizeof(_tagItem), 1, pFile);
		}

		//방어구
		fread(&pPlayer->bEquip[EQ_ARMOR], sizeof(bool), 1, pFile);

		if (pPlayer->bEquip[EQ_ARMOR])
		{
			fread(&pPlayer->bEquip[EQ_ARMOR], sizeof(_tagItem), 1, pFile);
		}

		/*
		인벤토리 정보를 얻어옴 

		골드 -> 아이템 숫자(슬롯) -> 그 슬롯안에 있는 아이템 정보들
		
		*/

		fread(&pPlayer->tInventory.iGold, 4, 1, pFile);

		fread(&pPlayer->tInventory.iItemCount, 4, 1, pFile);

		fread(&pPlayer->tInventory.tItem, sizeof(_tagItem), 1, pFile);


		//파일 다적었으면 닫아줍니다.
		fclose(pFile);
		return true;
	}
	return false;
}

// bool을 이용해서 파일을 제대로 만들어졌는지 true false로 리턴해준다. 
bool SavePlayer(_tagPlayer* pPlayer)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, "Player.ply", "wb");

	if (pFile)
	{
		// 이름 저장 
		fwrite(pPlayer->strName, 1, NAME_SIZE, pFile);

		//직업 정보,이름 저장 
		fwrite(&pPlayer->eJob, sizeof(JOB), 1, pFile);
		fwrite(pPlayer->strJobName, 1, NAME_SIZE, pFile);

		//공격력 저장 
		fwrite(&pPlayer->iAttackMin, 4, 1, pFile);
		fwrite(&pPlayer->iAttackMax, 4, 1, pFile);

		//방어력 저장 
		fwrite(&pPlayer->iArmorMin, 4, 1, pFile);
		fwrite(&pPlayer->iArmorMax, 4, 1, pFile);

		//체력 저장
		fwrite(&pPlayer->iHP, 4, 1, pFile);
		fwrite(&pPlayer->iHPMax, 4, 1, pFile);

		//마나 저장
		fwrite(&pPlayer->iMP, 4, 1, pFile);
		fwrite(&pPlayer->iMPMax, 4, 1, pFile);

		//경험치와 레벨을 저장.

		fwrite(&pPlayer->iExp, sizeof(int), 1, pFile); // 요렇게 해도 똑같음 int는 4바이트니깐
		fwrite(&pPlayer->iLevel, sizeof(int), 1, pFile);

		//무기아이템 착용 여부를 저장. 
		fwrite(&pPlayer->bEquip[EQ_WEAPON], sizeof(bool), 1, pFile);

		// 
		if (pPlayer->bEquip[EQ_WEAPON])
		{
			fwrite(&pPlayer->tEquip[EQ_WEAPON], sizeof(_tagItem), 1, pFile);
		}

		//방어구
		fwrite(&pPlayer->bEquip[EQ_ARMOR], sizeof(bool), 1, pFile);

		if (pPlayer->bEquip[EQ_ARMOR])
		{
			fwrite(&pPlayer->bEquip[EQ_ARMOR], sizeof(_tagItem), 1, pFile);
		}

		/*
		인벤토리 정보를 저장

		골드 -> 아이템 숫자(슬롯) -> 그 슬롯안에 있는 아이템 정보들

		*/

		fwrite(&pPlayer->tInventory.iGold, 4, 1, pFile);

		fwrite(&pPlayer->tInventory.iItemCount, 4, 1, pFile);

		fwrite(pPlayer->tInventory.tItem, sizeof(_tagItem),
			pPlayer->tInventory.iItemCount, pFile);


		fclose(pFile);
		return true;
	}

	return false;
}


_tagMonster CreateMonster(const char* pName, int iAttackMin,
	int iAttackMax, int iArmorMin, int iArmorMax, int iHP,
	int iMP, int iLevel, int iExp, int iGoldMin, int iGoldMax)
{
	_tagMonster tMonster = {};


	strcpy_s(tMonster.strName, pName);
	tMonster.iAttackMin = iAttackMin;
	tMonster.iAttackMax = iAttackMax;
	tMonster.iArmorMin = iArmorMin;
	tMonster.iArmorMax = iArmorMax;
	tMonster.iHP = iHP;
	tMonster.iHPMax = iHP;
	tMonster.iMP = iMP;
	tMonster.iMPMax = iMP;
	tMonster.iLevel = iLevel;
	tMonster.iExp = iExp;
	tMonster.iGoldMin = iGoldMin;
	tMonster.iGoldMax = iGoldMax;

	return tMonster;
}

void SetMonster(_tagMonster* pMonsterArr) // 배열명은 포인터다. 외워라 그냥 명시적으로라도 외워라 
{
	pMonsterArr[0] = CreateMonster("고블린", 20, 30, 2, 5,
		100, 10, 1, 1000, 500, 1500);
	pMonsterArr[1] = CreateMonster("트롤", 80, 130, 60, 90,
		2000, 100, 5, 7000, 6000, 8000);
	pMonsterArr[2] = CreateMonster("드래곤", 250, 500, 200, 400,
		30000, 20000, 10, 30000, 20000, 50000);
}


_tagLevelUpStatus CreateLvUpStatus(int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, int iHPMin, int iHPMax,
	int iMPMin, int iMPMax)
{
	_tagLevelUpStatus tStatus = {};

	tStatus.iAttackMin = 4;
	tStatus.iAttackMax = 10;
	tStatus.iArmorMin = 8;
	tStatus.iArmorMax = 16;
	tStatus.iHPMin = 50;
	tStatus.iHPMax = 100;
	tStatus.iMPMin = 10;
	tStatus.iMPMax = 20;

	return tStatus;
}
int OutPutAStoreMenu()
{
	system("cls");
	cout << "*************************** 상점 ***************************" << endl;
	cout << "1. 무기상점" << endl;
	cout << "2. 방어구상점" << endl;
	cout << "3. 뒤로가기" << endl;
	cout << "상점을 선택하세요 : ";
	int iMenu = InputInt();

	if (iMenu < SM_NONE || iMenu > SM_BACK)
	{
		return SM_NONE;
	}
	return iMenu;
}

int OutputStoreItemList(_tagInventory* pInventory,_tagItem* pStore,int iItemCount)
{
	system("cls");

	
	// 판매 목록을 보여준다.
	for (int i = 0; i < iItemCount; ++i)
	{
		cout << i + 1 << ". 이름 : " << pStore[i].strName <<
			"\t종류 : " << pStore[i].strTypeName << endl;
		cout << "공격력 : " << pStore[i].iMin << " - " <<
			pStore[i].iMax << endl;
		cout << "판매가격 : " << pStore[i].iPrice <<
			"\t구매가격 : " << pStore[i].iSell << endl;
		cout << "설명 : " << pStore[i].strDesc << endl << endl;
	}

	cout << iItemCount + 1 << ". 뒤로가기" << endl;
	cout << "보유금액 : " << pInventory->iGold << " Gold" << endl;
	cout << "남은공간 : " << INVENTORY_MAX - pInventory->iItemCount << endl;
	cout << "구입할 아이템을 선택하세요 : ";
	int iMenu = InputInt();

	return iMenu;

	if (iMenu < 1 || iMenu > iItemCount + 1 )
	{
		return INT_MAX;
	}

}

void BuyItem(_tagInventory* pInventory, _tagItem* pStore,int iItemCount,int iStoreType)
{
	while (true)
	{
		system("cls");
		switch (iStoreType)
		{
		case SM_WEAPON:
			cout << "**************************** 무기 상점 *******************************" << endl;
			break;

		case SM_ARMOR:
			cout << "**************************** 방어구 상점 *******************************" << endl;
			break;
		
		}
		int iInput = OutputStoreItemList(pInventory, pStore, iItemCount);

		if (iInput == INT_MAX)
		{
			cout << "잘못 입력하였습니다" << endl;
			system("pause");
			continue;
		}

		else if (iInput == iItemCount + 1) // 4번 나가기 
		{
			break;
		}
				
		// 상점판매목록 배열의 인덱스를 구해준다.
		int	iIndex = iInput - 1;

		// 인벤토리가 꽉 찼는지 검사한다.
		if (pInventory->iItemCount == INVENTORY_MAX)
		{
			cout << "가방이 꽉 찼습니다." << endl;
			system("pause");
			continue;
		}

		// 돈이 부족할 경우
		else if (pInventory->iGold < pStore[iIndex].iPrice)
		{
			cout << "보유금액이 부족합니다." << endl;
			system("pause");
			continue;
		}

		// 처음에 iItemCount는 하나도 추가되어있지 않기 때문에 0으로 초기화
		// 되어 있으므로 0번 인덱스에 구매한 아이템을 추가하게 된다. 그리고
		// 카운트가 1이 된다. 다음번에 추가할때는 1번 인덱스에 추가하게된다.
		pInventory->tItem[pInventory->iItemCount] =
			pStore[iIndex];
		++pInventory->iItemCount;

		// 골드를 차감한다.
		pInventory->iGold -= pStore[iIndex].iPrice;

		cout << pStore[iIndex].strName << " 아이템을 구매하였습니다." << endl;
		system("pause");
	}
}

void RunStore(_tagInventory* pInventory,_tagItem* pWeapon,_tagItem* pArmor)
{

	while (true)
	{
		switch (OutPutAStoreMenu())
		{
		case SM_WEAPON:
			BuyItem(pInventory, pWeapon, STORE_WEAPON_MAX,SM_WEAPON);
			break;
		case SM_ARMOR:
			BuyItem(pInventory, pArmor, STORE_ARMOR_MAX,SM_ARMOR);
			break;
		case SM_BACK:
			return;
		}
	}
}

_tagItem CreateItem(const char* pName,ITEM_TYPE eType,int iMin,int iMax,int iPrice,int iSell,
	const char* pDesc)
{
	_tagItem tItem = {};

	strcpy_s(tItem.strName, pName);
	strcpy_s(tItem.strDesc, pDesc);

	tItem.eType = eType;

	switch (eType)
	{	
	
	case IT_WEAPON:
		strcpy_s(tItem.strTypeName,"무기");
		break;
	case IT_ARMOR:
		strcpy_s(tItem.strTypeName,"방어구");
		break;
	}

	tItem.iMin = iMin;
	tItem.iMax = iMax;
	tItem.iPrice = iPrice;
	tItem.iSell = iSell;


	return tItem;
}

int OutputInventory(_tagPlayer* pPlayer)
{
	system("cls");
	cout << "*************************** 가방 ***************************" << endl;

	OutputPlayer(pPlayer);

	for (int i = 0; i < pPlayer->tInventory.iItemCount; ++i)
	{
		cout << i + 1 << ". 이름 : " << pPlayer->tInventory.tItem[i].strName <<
			"\t종류 : " << pPlayer->tInventory.tItem[i].strTypeName << endl;
		cout << "공격력 : " << pPlayer->tInventory.tItem[i].iMin << " - " <<
			pPlayer->tInventory.tItem[i].iMax << endl;
		cout << "판매가격 : " << pPlayer->tInventory.tItem[i].iPrice <<
			"\t구매가격 : " << pPlayer->tInventory.tItem[i].iSell << endl;
		cout << "설명 : " << pPlayer->tInventory.tItem[i].strDesc << endl << endl;
	}

	cout << pPlayer->tInventory.iItemCount + 1 << ". 뒤로가기" << endl;
	cout << "장착할 아이템을 선택하세요 : ";
	int iMenu = InputInt();

	if (iMenu < 1 || iMenu > pPlayer->tInventory.iItemCount + 1)
	{
		return INT_MAX;
	}
	return iMenu;
}

EQUIP ComputeEquipType(ITEM_TYPE eType)
// 제대로 선택했을 경우 해당 아이템의 종류에 따라 장착 부위를
// 결정하게 한다.
{
	EQUIP	eq;
	switch (eType)
	{
		//장착 부위 셋팅 
	case IT_WEAPON:
		eq = EQ_WEAPON;
		break;
	case IT_ARMOR:
		eq = EQ_ARMOR;
		break;
	}
	return eq;
}

void Runinventory(_tagPlayer* pPlayer)
{
	while (true)
	{
		int iInput = OutputInventory(pPlayer);
		
		if (iInput == INT_MAX)
		{
			continue;
		}

		else if(iInput == pPlayer->tInventory.iItemCount + 1)
		{
			break;
		}

		// 아이템 인덱스를 구해준다.
		int	idx = iInput - 1;

		
		
		EQUIP eq = ComputeEquipType(pPlayer->tInventory.tItem[idx].eType);
				
		// 아이템이 장착되어 있을 경우 장착되어있는 아이템과 장착할 아이템을
		// 교체해 주어야 한다. Swap 알고리즘을 활용한다.
		if (pPlayer->bEquip[eq] == true)
		{
			_tagItem	tSwap = pPlayer->tEquip[eq];
			pPlayer->tEquip[eq] = pPlayer->tInventory.tItem[idx];
			pPlayer->tInventory.tItem[idx] = tSwap;
		}

		// 장착되어있지 않을 경우 인벤토리 아이템을 장착창으로 옮기고
		// 인벤토리는 1칸 비워지게 된다.
		else
		{
			pPlayer->tEquip[eq] = pPlayer->tInventory.tItem[idx];

			for (int i = idx; i < pPlayer->tInventory.iItemCount - 1; ++i)
			{
				pPlayer->tInventory.tItem[i] = pPlayer->tInventory.tItem[i + 1];
			}

			--pPlayer->tInventory.iItemCount;

			// 장착을 했기 때문에 true로 만들어준다.
			pPlayer->bEquip[eq] = true;
		}

		cout << pPlayer->tEquip[eq].strName << " 아이템을 장착하였습니다." << endl;

		system("pause");

	}


}

int main()
{
	srand((unsigned int)time(0));
		
	_tagPlayer tPlayer = {};
	
	int iGameMode = 0;

	while (iGameMode <= GM_NONE || iGameMode > GM_END)
	{
		system("cls");
		cout << "1. new game " << endl;
		cout << "2. load game" << endl;
		cout << "3. end" << endl;
		
		iGameMode = InputInt();
	}


	if (iGameMode == GM_END)
	{
		return 0;
	}

	switch (iGameMode)
	{
	case GM_NEW:
		SetPlayer(&tPlayer);
		break;
	case GM_LOAD:
		//false에 !을 붙이면 true가 된다.
		if (!LoadPlayer(&tPlayer))
		{
			cout << "Player Load Error" << endl;
			return 0;
		}
		break;
	}
	
	

	// 몬스터를 생성한다.
	_tagMonster	tMonsterArr[MT_BACK - 1] = {};

	SetMonster(tMonsterArr); // 배열은 포인터 그자체니깐 당연히 되지

	g_tLvUpTable[JOB_KNIGHT - 1] = 
		CreateLvUpStatus(4, 10, 8, 16, 50, 100, 10, 20);
	g_tLvUpTable[JOB_ARCHER - 1] =
		CreateLvUpStatus(10, 15, 5, 10, 30, 60, 30, 50);
	g_tLvUpTable[JOB_WIZARD - 1] =
		CreateLvUpStatus(15, 20, 3, 7, 20, 40, 50, 100);

	// 상점에서 판매할 아이템 목록을 생성한다.
	_tagItem	tStoreWeapon[STORE_WEAPON_MAX] = {};
	_tagItem	tStoreArmor[STORE_ARMOR_MAX] = {};

	tStoreWeapon[0] = CreateItem("목검",IT_WEAPON, 5, 10, 1000, 500,"나무로 만든 칼");
	tStoreWeapon[1] = CreateItem("장궁", IT_WEAPON, 20, 10, 7000, 3500, "짱짱한 활");
	tStoreWeapon[2] = CreateItem("지팡이", IT_WEAPON, 90, 150, 30000, 15000, "나무로 만든 지팡이");

	
	tStoreArmor[0] = CreateItem("천 갑옷", IT_ARMOR, 10, 20, 1000, 500, "천으로 만든 갑옷");
	tStoreArmor[1] = CreateItem("가죽 갑옷", IT_ARMOR, 20, 10, 7000, 3500, "가죽으로 만든 갑옷");
	tStoreArmor[2] = CreateItem("풀 플레이트 아머", IT_ARMOR, 90, 150, 30000, 15000, "풀플레이트로 만든 갑옷 ");


	bool	bLoop = true;
	while (bLoop)
	{
		switch (OutputMainMenu())
		{
		case MM_MAP:
			RunMap(&tPlayer, tMonsterArr);
			break;
		case MM_STORE:
			RunStore(&tPlayer.tInventory, tStoreWeapon, tStoreArmor);
			break;
		case MM_INVENTORY:
			Runinventory(&tPlayer);
			break;
		case MM_EXIT:
			bLoop = false;
			break;
		}
	}

	SavePlayer(&tPlayer);


	return 0;
}