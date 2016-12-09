#include <QtGui/QApplication>
#include "Dlg_TestModule.h"
#include <iostream>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

int main(int argc, char** argv)
{
	if (argc<2)
	{
		std::cout << "Inadequate Arguments." << std::endl;
		std::cout << "Name of shared memory needs to be given." << std::endl;
		return -1;
	}

	using namespace boost::interprocess;
	char* nameSharedMemory = argv[1];
	unsigned char *mem;
	size_t len;
	windows_shared_memory shm;
	mapped_region region;
	try
	{
		//Open already created shared memory object.
		shm = windows_shared_memory(open_only, nameSharedMemory, read_write);

		//Map the whole shared memory in this process
		region = mapped_region(shm, read_write);

		//Check that memory was initialized to 1
		mem = static_cast<unsigned char*>(region.get_address());
		len = region.get_size();
	}
	catch (interprocess_exception &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}	

	// add up reference count
	mem[0]+=1;

	QApplication qapp(argc, argv);
	Dlg_TestModule dtm(mem,len);
	dtm.setModal(false);
	dtm.show();
	qapp.exec();
	return 0;
}