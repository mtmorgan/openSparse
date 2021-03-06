
#dyn.load(.parms$getLibPath())
#dyn.unload(.parms$getLibPath())
#detach("package:openSparse",unload = T)



mydata=sparseData()

myclass=openiSpase(mydata$dataframe,mydata$rowind,mydata$colind,mydata$rowNum,mydata$colNum)
myclass=upload(myclass)
sum((colSums(myclass)-colSums(mydata$dataMatrix))^2)
sum((rowSums(myclass)-rowSums(mydata$dataMatrix))^2)
myclass=download(myclass)

library(rhdf5)
library(ExperimentHub)
library(HDF5Array)
library("tictoc")
hub=ExperimentHub::ExperimentHub()
fl=hub[["EH1039"]]
mydata=openSparse(fl)
tic()
res=rowSums(mydata)
toc()
setDevice(1)
getCurDevice()
.resourceManager$getGPUusage()
tic()
res1=rowSums(mydata)
toc()

mydata1=TENxMatrix(fl, group="mm10")



tic()
res1=colSums(mydata1)
toc()


