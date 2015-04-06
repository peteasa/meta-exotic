##################################################################
# The intention of this layer is to provide an easy to use
# way of building exotic (ie non-native) toolchains.
#
# For example an arm target may have an avr processor attached.
# This layer enables the toolchain to be built for the avr processor
# so that avr code can be built within the yocto sdk and on target
# and that avr code can then be included in the distribution.
##################################################################

## Used by exotic-binutils.inc
### exotic-binutils_2.23.bb
### exotic-binuitls-cross_2.23.bb
### exotic-binutils-cross-canadian_2.23.bb
## Used by exotic-gcc-common.inc
### exotic-gcc-4.8.inc
#### exotic-gcc_4.8.bb
#### exotic-gcc-cross_4.8.bb
#### exotic-gcc-cross-canadian_4.8.bb
#### exotic-gcc-runtime_4.8.bb
#### exotic-libgcc_4.8.bb
## Used by exotic-newlib-common.inc
### exotic-newlib-1.20.inc
#### exotic-libgloss_1.20.bb
#### exotic-newlib_1.20.bb

##################################################################
# Architecture-dependent build variables.
##################################################################

TARGET_ARCH_GVARIABLE := "${TARGET_ARCH}"
TARGET_OS_GVARIABLE := "${TARGET_OS}"
TARGET_VENDOR_GVARIABLE := "${TARGET_VENDOR}"
TARGET_SYS_GVARIABLE := "${TARGET_SYS}"
TARGET_PREFIX_GVARIABLE := "${TARGET_SYS}-"
TARGET_CC_ARCH_GVARIABLE := "${TARGET_CC_ARCH}"
TARGET_LD_ARCH_GVARIABLE := "${TARGET_LD_ARCH}"
TARGET_AS_ARCH_GVARIABLE := "${TARGET_AS_ARCH}"

#
# Now update TARGET variables
#
TARGET_ARCH = "${EXOTIC_TARGET_ARCH}"
TARGET_OS = "${EXOTIC_TARGET_OS}"
TARGET_VENDOR = "${EXOTIC_TARGET_VENDOR}"
TARGET_SYS = "${EXOTIC_TARGET_SYS}"
TARGET_PREFIX = "${TARGET_SYS}-"
TARGET_CC_ARCH = "${EXOTIC_TARGET_CC_ARCH}"
TARGET_LD_ARCH = "${EXOTIC_TARGET_LD_ARCH}"
TARGET_AS_ARCH = "${EXOTIC_TARGET_AS_ARCH}"

##################################################################
# Build flags and options.
##################################################################
TARGET_CFLAGS_GVARIABLE := "${TARGET_CFLAGS}"
TARGET_CPPFLAGS_GVARIABLE := "${TARGET_CPPFLAGS}"
TARGET_CXXFLAGS_GVARIABLE := "${TARGET_CXXFLAGS}"
TARGET_LDFLAGS_GVARIABLE := "${TARGET_LDFLAGS}"

#
# Now update TARGET variables
#
TARGET_CPPFLAGS = "${EXOTIC_TARGET_CPPFLAGS}"
TARGET_CFLAGS = "${EXOTIC_TARGET_CFLAGS}"
TARGET_CXXFLAGS = "${EXOTIC_TARGET_CXXFLAGS}"
TARGET_LDFLAGS = "${EXOTIC_TARGET_LDFLAGS}"

# Now use these updated variables with autotools
inherit autotools