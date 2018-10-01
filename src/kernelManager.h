#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <map>
#include <iostream>

#include <fstream>
#include<vector>

class kernelManager {
public:
	static int deviceIndex;
	static cl_context context;
	static cl_device_id device_id;
	static cl_command_queue command_queue;
	static std::map<std::string, cl_program> programTable;
	static std::map<std::string, cl_kernel> kernelTable;
	static bool ready;
	static char* kernelFile;
public:
	//Get the platform info
	static void getPlatformsInfo();
	//Print out all device name
	static void getAllDeviceName();
	//Print out the current device compatibility
	static void showDeviceInfo();
	static void getDeviceFullInfo(int device);

	static void setKernelDirectory(char*);
	static void setDevice(int device);
	static cl_kernel createKernel(const char* filename, const char* kernel);
	static cl_kernel createKernel(const char* kernel);

private:
	static void loadProgram(const char* filename);
	static void initializeManager();
	static cl_device_id getDeviceID(int k);
};