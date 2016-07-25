##################################################################
# Extensions to bitbake.conf
#   Temp location.  These should go into poky/meta/conf/bitbake.conf
##################################################################
#
##################################################################
# Architecture-dependent build variables.
##################################################################

# For these recipes we may need to override TARGET and HOST whilst keeping
# variables derived from the original TARGET and HOST unchanged.
# To do this create new variables to store the original TARGET and HOST
HOST_ARCH_GVARIABLE := "${HOST_ARCH}"
HOST_OS_GVARIABLE := "${HOST_OS}"
HOST_VENDOR_GVARIABLE := "${HOST_VENDOR}"
HOST_SYS_GVARIABLE := "${HOST_SYS}"
HOST_PREFIX_GVARIABLE := "${HOST_PREFIX}"
HOST_CC_ARCH_GVARIABLE := "${HOST_CC_ARCH}"
HOST_LD_ARCH_GVARIABLE := "${HOST_LD_ARCH}"
HOST_AS_ARCH_GVARIABLE := "${HOST_AS}"
HOST_EXEEXT_GVARIABLE := "${HOST_EXEEXT}"

##################################################################
# Build flags and options.
##################################################################
#TARGET_CFLAGS_GVARIABLE := "${TARGET_CFLAGS}"
#TARGET_CPPFLAGS_GVARIABLE := "${TARGET_CPPFLAGS}"
#TARGET_CXXFLAGS_GVARIABLE := "${TARGET_CXXFLAGS}"
#TARGET_LDFLAGS_GVARIABLE := "${TARGET_LDFLAGS}"

#
# Now update HOST variables
#
HOST_ARCH = "${EXOTIC_TARGET_ARCH}"
HOST_OS = "${EXOTIC_TARGET_OS}"
HOST_VENDOR = "${EXOTIC_TARGET_VENDOR}"
HOST_SYS = "${EXOTIC_TARGET_SYS}"
HOST_PREFIX = "${EXOTIC_TARGET_PREFIX}"
HOST_CC_ARCH = "${EXOTIC_TARGET_CC_ARCH}"
HOST_LD_ARCH = "${EXOTIC_TARGET_LD_ARCH}"
HOST_AS_ARCH = "${EXOTIC_TARGET_AS_ARCH}"
HOST_EXEEXT = ""

#
# Now all the scripts in this recipe can use TARGET_??
# or HOST_??
# or can use the original settings of TARGET_??_GVARIABLE and HOST_??_GVARIABLE
#
# For example move the Staging bin dir to a better location
STAGING_BINDIR_TOOLCHAIN = "${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}"
