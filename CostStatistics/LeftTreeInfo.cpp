#include "stdafx.h"
#include "LeftTreeInfo.h"


LeftTreeInfo::LeftTreeInfo()
{

}


LeftTreeInfo::~LeftTreeInfo()
{

}

void LeftTreeInfo::loadLeftTree(CTreeListCtrl * pCtrl)
{
	CTreeListItem* pRoot = NULL;
	CTreeListItem* pL2 = NULL;
	CTreeListItem* pL3 = NULL;

	pCtrl->InsertColumn("");

	pRoot = pCtrl->InsertItem("������Ϣ");
	pL2 = pCtrl->InsertItem("���Ϸ�");
	pL2 = pCtrl->InsertItem("�����");
	pL2 = pCtrl->InsertItem("�����");


	pRoot = pCtrl->InsertItem("ְ��н��ͳ��");
	pL2 = pCtrl->InsertItem("�˹��Ѻϼ�",pRoot);
	pCtrl->InsertItem("ְ��н��",pL2);
	pL3 = pCtrl->InsertItem("ֱ���˹�����",pL2);
	pCtrl->InsertItem("�������Գ���",pL3);
	pCtrl->InsertItem("��װ����",pL3);
	pCtrl->InsertItem("í������",pL3);
	pCtrl->InsertItem("Һѹ����",pL3);
	pCtrl->InsertItem("���峵��",pL3);
	pCtrl->InsertItem("���ӹ�����",pL3);
	pCtrl->InsertItem("�ۺϰ�",pL3);
	pCtrl->InsertItem("��������",pL3);
	pCtrl->InsertItem("��ع���������",pL3);

	pCtrl->InsertItem("�����˹���̯",pL2);
	pCtrl->InsertItem("�����˹���̯",pL2);
	pCtrl->InsertItem("��Ŀ�˹��ѷ�̯",pL2);


    pL2 = pCtrl->InsertItem("�������ͳ�Ʊ�",pRoot);
	pCtrl->InsertItem("ֱ���������",pL2);
	pCtrl->InsertItem("����������",pL2);
	pCtrl->InsertItem("��Ŀ������",pL2);

	pL2 = pCtrl->InsertItem("�������ͳ�Ʊ�",pRoot);
	pCtrl->InsertItem("���",pL2);
	pCtrl->InsertItem("˳��",pL2);
	pCtrl->InsertItem("�ɶ�",pL2);
	pCtrl->InsertItem("��ɳ",pL2);
	pCtrl->InsertItem("֣��",pL2);
	pCtrl->Invalidate(TRUE);
}