# VGPirp
Reader/writer for the VGP irp file format

# usage:

Convert a pair of synchronous FastQ to the IRP format:

```
VGP_fastqToIRP out.irp in1.fastq in2.fastq
```

Convert a single FastQ containing interleaved pair records to the IRP
format:

```
VGP_fastqToIRP out.irp in.fastq
```

Convert IRP to interval pair FastQ file:

```
VGP_IRPToFastQ < in.irp > out.fastq
```
