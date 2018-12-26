#ifndef IMPORTEXCEL
#define IMPORTEXCEL


class ImportExcel
{

public:
	ImportExcel();
	~ImportExcel();

public:
	void ImportStaffSalary();//导入职工薪酬
	void ImportWorkShopTotal();//导入车间合计
	void ImportCostTotal();//导入制造费用统计
	void ImportOverSeaProduce();//导入驻外基地生产情况
};
#endif