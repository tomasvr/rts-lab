#!/bin/bash
make clean
make
wsim-iclbsn2 --ui --trace=sched.trc --mode=time --modearg=5s --monitor=Pending:w SchedTest.elf 
wtracer --in=sched.trc --out=wsim.vcd-NPBASIC --format=vcd
gtkwave wsim.vcd-NPBASIC -S signals.tcl