#include <iostream>

using namespace std;

#define	NAME_SIZE	32

enum MAP_TYPE
{
	MT_NONE,
	MT_EASY,
	MT_NORMAL,
	MT_HARD,
	MT_BACK
};

enum MAIN_MENU
{
	MM_NONE,
	MM_ADD,
	MM_LOAD,
	MM_SAVE,
	MM_OUTPUT,
	MM_EXIT
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

void AddMonster(_tagMonster* pMonsterArr)
{
	system("cls");
	cin.clear();
	cin.ignore(1024, '\n');


	int		iMap = 0;

	while (true)
	{
		cout << "1. easy" << endl; 
		cout << "2. normal" << endl;
		cout << "3. hard" << endl;
		cout << "4. exit" << endl;
		cout << "추가할 난이도를 선택하세요 " << endl;
		iMap = InputInt();

		if (iMap == 4)	return;

		else if (iMap < 1 || iMap > 4)
			continue;

		--iMap;
		break;		
	}
		
		system("cls");
		cin.clear();
		cin.ignore(1024, '\n');

		cout << "이름 : " << endl;

		cin.getline(pMonsterArr[iMap].strName, NAME_SIZE - 1);			
		
		cout << "최소 공격력 : ";
		pMonsterArr[iMap].iAttackMin = InputInt();

		cout << "최대 공격력 : ";
		pMonsterArr[iMap].iAttackMax = InputInt();
		
		cout << "최소 방어력 : ";
		pMonsterArr[iMap].iArmorMin = InputInt();

		cout << "최대 방어력 : ";
		pMonsterArr[iMap].iArmorMax = InputInt();

		cout << "체력 : ";
		pMonsterArr[iMap].iHP = InputInt();
		pMonsterArr[iMap].iHPMax = pMonsterArr[iMap].iHP;

		cout << "마나 : ";
		pMonsterArr[iMap].iMP = InputInt();
		pMonsterArr[iMap].iMPMax = pMonsterArr[iMap].iMPMax;

		cout << "레벨 : ";
		pMonsterArr[iMap].iLevel = InputInt();

		cout << "획득 경험치 : ";
		pMonsterArr[iMap].iExp = InputInt();

		cout << "획득 최소 골드 :"; 
		pMonsterArr[iMap].iGoldMin = InputInt();

		cout << "획득 최대 골드 :";
		pMonsterArr[iMap].iGoldMax = InputInt();

		cout << " 몬스터 추가 완료 " << endl;

		system("pause");
}

bool SaveMonster(_tagMonster* pMonsterArr)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, "Monster.mst", "wb");

	if (pFile)
	{
		fwrite(pMonsterArr, sizeof(_tagMonster), MT_BACK, pFile);
		fclose(pFile);
		return true;
	}

	return false;
}



bool LoadMonster(_tagMonster* pMonsterArr)
{		

	FILE* pFile = NULL;

	fopen_s(&pFile, "Monster.mst", "rb");

	if (pFile)
	{
		cout << "Load Complete" << endl;
		system("pause");
		fread(pMonsterArr, sizeof(_tagMonster), MT_BACK, pFile);
		fclose(pFile);
		return true;
	}

	return false;
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

void Output(_tagMonster* pMonsterArr)
{
	system("cls");
	for (int i = 0; i < MT_BACK; i++)
	{
		OutputMonster(&pMonsterArr[i]);
	}

	system("pause");
}

int main()
{
	_tagMonster tMonster[MT_BACK] = {};
	//배열로 몬스터 숫자 초기화
	while (true)
	{
		system("cls");
		cout << "1. 몬스터 추가" << endl;
		cout << "2. 읽어오기" << endl;
		cout << "3. 저장" << endl;
		cout << "4. 몬스터 출력" << endl;
		cout << "5. 종료" << endl;
		cout << "Select Menu" << endl;

		int iMenu = InputInt();

		

		if (iMenu == MM_EXIT)
		{
			break;
		}

		switch (iMenu)
		{

		case MM_ADD:
			AddMonster(tMonster);
			break;
		case MM_LOAD:
			LoadMonster(tMonster);			
			break;
		case MM_SAVE:
			SaveMonster(tMonster);
			break;			
		case MM_OUTPUT:
			Output(tMonster);
			break;

		}// switch 종료 


	}//while문 종료 


	SaveMonster(tMonster);
	// 프로그램에서 따로 저장을 하지않더라도 자동으로 저장되게 적어놓음


	return 0;
}