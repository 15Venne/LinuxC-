#include <stdio.h>
#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

struct FilePointerDeleter
{
	void operator()(FILE * fp) const
	{
		if(fp) {
			fclose(fp);
			cout << ">>> fclose(fp)" << endl;
		}
	}
};

void test0()
{
	unique_ptr<FILE, FilePointerDeleter>  up(fopen("test.txt", "a+"));
	std::string msg("wangdao\n");
	fwrite(msg.c_str(), 1, msg.size(), up.get());
}

void test1()
{
	shared_ptr<FILE> sp(fopen("test.txt", "a+"), FilePointerDeleter());
	std::string msg("wangdao\n");
	fwrite(msg.c_str(), 1, msg.size(), sp.get());
}
 

int main(void)
{
	//test0();
	test1();
	return 0;
}
