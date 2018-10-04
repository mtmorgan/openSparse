#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "R_ext/libextern.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include"Tools.h"
#include "kernelManager.h"
#include "arrayfire.h"
#include <af/opencl.h>
#include "sparseMatrix.h"


//using namespace std;
using namespace af;

extern "C" LibExport
void upload(
	double* dataFrame, int * rowInd, double* colInd, double* size,
	double* offset, void** address)
{
	sparseMatrix<double>* data =
		new sparseMatrix<double>(dataFrame, rowInd, colInd, size, offset);
	*address = (void*)data;
}
extern "C" LibExport
void download(double* dataFrame, int * rowInd, double* colInd, void** address)
{
	sparseMatrix<double>* data = *(sparseMatrix<double>**)address;
	data->getHostData(dataFrame);
	data->getHostRow(rowInd);
	data->getHostCol(colInd);
}

extern "C" LibExport
void clear(void** address) {
	delete *(sparseMatrix<double>**)address;
}


extern "C" LibExport
void rowSum(double* rowResult,void** address)
{
	sparseMatrix<double>* data = *(sparseMatrix<double>**)address;
	size_t length = data->rowNum;
	array result = constant(0, length, dtype::f32);
	data->lock();
	result.lock();
	cl_kernel kernel = kernelManager::createKernel("rowSum");
	static cl_command_queue af_queue = afcl::getQueue();
	// Set arguments and launch your kernels
	cl_int error = 0;
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), data->getDevData());
	error += clSetKernelArg(kernel, 1, sizeof(cl_mem), data->getDevRow());
	error += clSetKernelArg(kernel, 2, sizeof(cl_mem), data->getDevCol());
	error += clSetKernelArg(kernel, 3, sizeof(cl_mem), result.device<cl_mem>());
	//	 data->getLength(1);
	size_t workNum = data->getLength(2) - 1;
	size_t localNum = 1;
	clEnqueueNDRangeKernel(
		af_queue, kernel, 1, NULL, &workNum, &localNum, 0,
		NULL, NULL);

	data->unlock();
	result.unlock();
	cpyData(rowResult, result.host<float>(), length);

	//af_print(result);
}

extern "C" LibExport
void colSum(double* colResult, void** address)
{
	sparseMatrix<double>* data = *(sparseMatrix<double>**)address;
	size_t length = data->colNum;
	array result= constant(0,length, dtype::f32);
	data->lock();
	result.lock();
	cl_kernel kernel = kernelManager::createKernel("colSum");
	static cl_command_queue af_queue = afcl::getQueue();
	// Set arguments and launch your kernels
	cl_int error = 0;
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), data->getDevData());
	error += clSetKernelArg(kernel, 1, sizeof(cl_mem), data->getDevRow());
	error += clSetKernelArg(kernel, 2, sizeof(cl_mem), data->getDevCol());
	error += clSetKernelArg(kernel, 3, sizeof(cl_mem), result.device<cl_mem>());
	//	 data->getLength(1);
	size_t workNum = data->getLength(2) - 1;
	size_t localNum = 1;
	clEnqueueNDRangeKernel(
		af_queue, kernel, 1, NULL, &workNum, &localNum, 0,
		NULL, NULL);

	data->unlock();
	result.unlock();
	cpyData(colResult, result.host<float>(), length);
	//af_print(result);
}

extern "C" LibExport
void diagnostics(void)
{
	  kernelManager::getAllDeviceName();
	  kernelManager::getPlatformsInfo();
	  kernelManager::showDeviceInfo();
}
