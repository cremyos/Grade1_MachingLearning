#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>

using namespace std;

#define MAXN 10000
#define MAX_ITEM 26 

/**************** ���²����Ϊ��ü��ϵ��Ӽ� ******************/

struct mychar{			// �Զ���Ľṹ�壬����ַ����Լ����ִ��� 
	char ch[MAXN];		// �ַ���		
	int num;			// ���ִ��� 
};

mychar tmp;
vector<mychar> vset;	// ����Ӽ������� 
int ListLength;			// �ַ��ĳ��� 

void saveSubset(char *Buffer, int flag) // ���Ӽ������vector�� 
{    
     for(int i=0; i<=flag; i++)
		tmp.ch[i] = Buffer[i];          
     tmp.num = flag;
     vset.push_back(tmp);
}

int Index(char *List, char c) //�ҵ�Ԫ��c�ڼ���List�е�λ�� 
{
     for(int i=0; i<=ListLength-1; i++)
     {
              if(c == List[i])
              {
                    return i;             
                    break;
              }                  
     }
     return -1;     
}

void SubSet(char *List, int m, char *Buffer, int flag)
{     
     if(m < ListLength-1)
     {
          for(int i=(flag==0) ? 0 : Index(List,Buffer[flag-1])+1; i<=ListLength-1; i++)
          //��flag==0ʱ��Buffer��û���κ�Ԫ�أ���ʱi=[0...ListLength-1]
          //��flag>0ʱ���ҵ�Buffer�е����һ��Ԫ���ڼ���List�е�λ��i����[i....ListLength-1]
          //����Ԫ�أ��ӵ�BufferԪ�ص������ 
          {
                Buffer[flag]=List[i];                
                saveSubset(Buffer,flag);
                SubSet(List, m+1, Buffer,flag+1);
          }          
     }
     return;
}

mychar getComplementarySet(mychar List, mychar SubSet)		// �󲹼��ĺ��� 
{
	mychar result;
	int len = 0;
	int point1, point2;
	point1 = point2 = 0;
	bool flag;
	
	for(; point1 < List.num; point1++)
	{
		flag = true;
		for(point2 = 0; point2 < SubSet.num; point2++)
		{
			if(List.ch[point1] == SubSet.ch[point2])	
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			result.ch[len] = List.ch[point1];
			len	++;		
		}
	}
	result.num = len;
	
	return result;
}

/************************* ���˽��� **************************/


/**************** ���²����Ϊ������ﹹ�� ******************/

struct Transaction      //����ṹ��  
{  
    int TID;  
    int num;  
    char item[MAXN];  
}*trans;

struct ItemSet	// ָ��ÿ������е�С� 
{
	char item[MAXN];
	int itemLen;
	int appeartime;
};

struct Apriori         // ���ýṹ��ָ��ָ������  
{  
    ItemSet Apriori_item[MAXN];  
};

vector<ItemSet> v;	 // ������ɢ�б� 

int trans_num;       // ������  
int min_sup;         // ����֧�ֶȣ���С֧�ֶȼ���  
int time_tmp;		 // ÿ���ж�ΪƵ������ݴ���� 
double min_conf;	 // ��С����ϵ�� 

bool aprioriGrow(char c1[],char c2[],char tk[],int k) // ��k��Ƶ��������k+1��  
{  
	int i, tk_num;
	i = tk_num = 0;
	bool f;
	
	while(i < k)
	{
		if(c1[i] == c2[i])
		{
			tk[tk_num] = c1[i];
			i++; tk_num ++;
		}
		else
		{
			if(i == k - 1)	// ��������������k���ǰk-1��Ҫ���
			{
				tk[tk_num ++] =  c1[k - 1];
				tk[tk_num ++] =  c2[k - 1];
				f = true;
			} 
			else f = false;
			break;
		}
	}
	return f; 
} 


bool isFrequent(char c[],int n)  //�ж�һ��n��Ƿ�ΪƵ����  
{
	int i, j, pos;
	int time = 0;	// ����Ƶ��
	for(i = 0; i < trans_num; i++)
	{
		if(n > trans[i].num)continue;	// ����е������Ѿ�����ĳ�������е��������Թ�
		j = pos = 0;
		while(j < n && pos < trans[i].num)
		{
			if(c[j] == trans[i].item[pos])
			{
				j++; 
				pos++;
			}
			else if(c[j] < trans[i].item[pos])	break;	// Ԫ���Ѿ��������� 
			else pos++; 
		} 
		if(j == n) time++; 
	} 
	time_tmp = time; 
	 
	return (time >= min_sup); 
}  

bool hasChecked(Apriori *p,int n,char tk[],int curK)  //�ж��Ƿ�����Ƶ������  
{  
    int i;  
    for(i = 0;i < n; i++)  
    {   
		if(strcmp(p -> Apriori_item[i].item, tk) == 0)
			return true;  
	}  
    return false;  
}  

int returnNum(char* List, int k)		// ��ɢ�б����ҵ��Ӽ����ֵĴ��� 
{
	int i, res = 0;
	bool flag = 0;
	for(i = 0; i < v.size(); i++)
	{
		flag = 1;
		if(v[i].itemLen != k) continue;
		else
		{
			for(int j =0; j < k; j++)
			{
				if(v[i].item[j] != List[j])
				{
					flag = 0;
					break;
				}
			}
			if(flag)
			{
				res = v[i].appeartime;
				break;
			}
		}
	}

	return res;
}


/************************* ���˽��� **************************/

int main()  
{  
    freopen("tt.txt", "r", stdin);  
    cin >> trans_num;  
    trans = (Transaction *)malloc(sizeof(Transaction)*trans_num);  
    min_sup = 100;    // ������С֧�ֶȼ���
    min_conf = 0.4;
  
    int allItem[MAX_ITEM] = {0};  
    int i, j;  
    char tmp;  
    
    for(i = 0;i < trans_num; i++)  
    {  
        cin >> trans[i].TID >> trans[i].num;  
        for(j = 0;j < trans[i].num; j++)  
        {  
            cin >> tmp;  
            trans[i].item[j] = tmp-'A';  
            allItem[trans[i].item[j]]++;  
        }  
        sort(trans[i].item, trans[i].item + trans[i].num); 	// �������������� 
    }  
  
    Apriori* Apriori_Point1 = new Apriori; 	// ����ָ��k�� 
	Apriori* Apriori_Point2 = new Apriori;  // ����ָ��k+1�� 
	Apriori* Apriori_tmp = new Apriori;
    memset(Apriori_Point1, 0, sizeof(Apriori_Point1));  
    memset(Apriori_Point2, 0, sizeof(Apriori_Point2));  
    
    ItemSet ItemSettmp;
  
  	fstream finout;
	finout.open("MyResult.txt",ios::out);
	
    finout << "level 1��" << endl; 
	cout <<  "level 1��" << endl;  
    j = 0;  
    for(i = 0;i < MAX_ITEM; i++)  
    {  
        if(allItem[i] >= min_sup)  
        {  
            Apriori_Point1->Apriori_item[j].item[0] = i;  
            char ch = char(Apriori_Point1 -> Apriori_item[j].item[0] + 'A');
            finout << ch << "  " << "    time is��" << allItem[i] << endl;  
            ItemSettmp.item[0] = ch;
            ItemSettmp.appeartime = allItem[i];
            ItemSettmp.itemLen = 1;
            v.push_back(ItemSettmp);		// ������ɢ�б��� 
            j++;  
        }  
    }  
    finout << endl << endl;  
    int KN_num = j;  
    
    if(KN_num == 0)return 0;  	// ��������Ϊ��ʱ�㷨ֹͣ 
    char tk[MAXN];  
    int curK = 1;  
    int curNum;  
  
    while(1)  
    {  
        curNum = 0;  
        for(i = 0; i < KN_num - 1; i++)  
        {  
            for(j = i + 1; j < KN_num; j++)  
            {            
				if(!aprioriGrow(Apriori_Point1 -> Apriori_item[i].item, 
							Apriori_Point1->Apriori_item[j].item, tk, curK))
					continue;  
                if(hasChecked(Apriori_Point2, curNum, tk, curK + 1))
					continue;  
                if(isFrequent(tk, curK + 1))  
                {  
                    strcpy(Apriori_Point2->Apriori_item[curNum].item, tk); 
					Apriori_Point2->Apriori_item[curNum].appeartime = time_tmp;
                    curNum++;  
                }  
            }  
        }  
        KN_num = curNum;  
        curK ++;  
        if(KN_num > 0)
        {
			finout << "level " << curK << ":" << endl << endl; 
			cout << "level " << curK << ":" << endl;  
        }
        for(i = 1;i <KN_num; i++)  
        {   
        	
        	ListLength = curK;
        	char *Buffer = new char[ListLength];
        	char *List = new char[ListLength];
            for(j = 0; j < curK - 1; j++)  
            {  
                finout << char(Apriori_Point2->Apriori_item[i].item[j] + 'A' ) << ",";
	  			Buffer[j] = ' ';		// ��ʼ��
				List[j] = char(Apriori_Point2->Apriori_item[i].item[j] + 'A' );
				ItemSettmp.item[j] = List[j];   
            }  
            finout << char(Apriori_Point2->Apriori_item[i].item[j] + 'A' ); 
            Buffer[j] = ' ';
			List[j] = char(Apriori_Point2->Apriori_item[i].item[j] + 'A' );
			ItemSettmp.item[j] = List[j];
            finout << "    time is��" << Apriori_Point2->Apriori_item[i].appeartime << endl;
            
            mychar mychar_List;
            strcpy(mychar_List.ch, ItemSettmp.item);
            mychar_List.num = ListLength;
            
			ItemSettmp.appeartime = Apriori_Point2->Apriori_item[i].appeartime;
			ItemSettmp.itemLen = curK ;
			v.push_back(ItemSettmp);
            
            
            finout << "������Ӽ�Ϊ��" << endl;
			
		    SubSet(List,0,Buffer,0);		// �����Ӽ� 
		    
			int p;
			for(p = 0; p < vset.size(); p++)
			{
				char* subset_tmp = new char[vset[p].num];
				strcpy(subset_tmp, vset[p].ch);
				mychar mychar_Subset;
				strcpy(mychar_Subset.ch, subset_tmp);
				mychar_Subset.num = vset[p].num + 1;
				int apear = returnNum(subset_tmp, vset[p].num + 1); 
				if(apear != 0)
				{ 
					for(int q = 0; q <= vset[p].num; q++)
					{
						finout << subset_tmp[q];
					}
					finout << "   time��" << apear << endl;
				} 
				
				finout << "------- �������� -------" << endl;
				
			//	if(apear * min_conf >= Apriori_Point2->Apriori_item[i].appeartime) 
				if(Apriori_Point2->Apriori_item[i].appeartime * 1.0 / apear >= min_conf)
				{
					mychar res = getComplementarySet(mychar_List, mychar_Subset);
					finout << "{";
					for(int q = 0; q <= vset[p].num; q++)
					{
						finout << subset_tmp[q];
					} 
					finout << "} --> {";
					for(int l = 0; l < res.num ; l++)
						finout << res.ch[l];
					finout <<"}";
					finout << "     " <<  Apriori_Point2->Apriori_item[i].appeartime * 1.0 / apear;
					finout << endl;
				}
				else
				{
					finout << "û�дﵽ��С����" << endl;
				}
				finout << "-------- ���� ---------" << endl;
				finout << endl; 
    		 
				
				delete[] subset_tmp;
			}

		    delete[] Buffer;
		    delete[] List;
		    
		    vset.clear(); 
		    finout << endl; 
            
        } 
		 
        Apriori_tmp = Apriori_Point1;
		Apriori_Point1 = Apriori_Point2;
		Apriori_Point2 = Apriori_tmp;
        finout << endl << endl;  
        if(KN_num == 0)break;  
    }
//	cout << v.size() << endl; 
    return 0;  
} 
