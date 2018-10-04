devtools::load_all()

## mydata = sparseData()

## myclass = openiSpase(
##     mydata$dataframe, mydata$rowind, mydata$colind,
##     mydata$size[4], mydata$size[5]
## )
## myclass = upload(myclass)
## sum((colSums(myclass)-colSums(mydata$dataMatrix))^2)
## sum((rowSums(myclass)-rowSums(mydata$dataMatrix))^2)
## myclass = download(myclass)

## sumResult = as.double(rep(0, 100))
## .C("rowSum", sumResult, myclass@GPUaddress)

library(rhdf5)
library(ExperimentHub)
library(HDF5Array)

hub = ExperimentHub::ExperimentHub()
fl = hub[["EH1039"]]
mydata = openSparse(fl)

system.time({
    res <- colSums(mydata)
})

mydata1 = TENxMatrix(fl,  group="mm10")
system.time({
    res1 <- colSums(mydata1)
})
