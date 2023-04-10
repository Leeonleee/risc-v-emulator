#!/bin/bash
./vm_riskxvii >> tests/out/no_argument.out
./vm_riskxvii tests/printing_s.mi >> tests/out/printing_s.out
(echo "10" | ./vm_riskxvii tests/fibonacci.mi) >> tests/out/fibonacci.out
