===============================================================
 
Makefile for building MSP Code Examples in command line
environement using the GCC Open Source Compiler for MSP430

Refer to MSP430 GCC User's Guide

    http://www.ti.com/lit/ug/slau646/slau646.pdf

Download GCC standalone package:

    http://www.ti.com/tool/msp430-gcc-opensource

===============================================================

Makefile usage:

    make Device=<deviceName> Example=<exampleName>
    e.g. Device=MSP430F5529 Example=MSP430F55xx_adc_01


Debug with GDB:

    make debug Device=<deviceName> Example=<exampleName>

    * Before connecting to target, GDB Agent needs to be running.
    * Refer to MSP430 GCC User's Guide for how to use GDB:
      http://www.ti.com/lit/ug/slau646/slau646.pdf

===============================================================

This Makefile assumes GCC standalong package is installed at the
default directory. To specify a different GCC install path, use:

    make Device=<deviceName> Example=<exampleName> \
         GCC_DIR=<path_To_Standalone_GCC_Root>

If using GCC installed inside Code Composer Studio, specify both
the GCC root and the MSP430 include path,

    make Device=<deviceName> Example=<exampleName> \
         GCC_DIR=<path_To_CCS/ccsv6/tools/compiler/gcc_msp430_version> \
         GCC_MSP_INC_DIR=<path_To_CCS/ccsv6/ccs_base/msp430/include_gcc>

===============================================================