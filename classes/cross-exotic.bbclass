##################################################################
# The intention of this layer is to provide an easy to use
# way of building exotic (ie non-native) toolchains.
#
# For example an arm target may have an avr processor attached.
# This layer enables the toolchain to be built for the avr processor
# so that avr code can be built within the yocto sdk and on target
# and that avr code can then be included in the distribution.
##################################################################

#
# Now update TARGET variables
#
#TARGET_ARCH = "${EXOTIC_TARGET_ARCH}"
#TARGET_OS = "${EXOTIC_TARGET_OS}"
#TARGET_VENDOR = "${EXOTIC_TARGET_VENDOR}"
#TARGET_SYS = "${EXOTIC_TARGET_SYS}"
# TARGET_PREFIX = "${TARGET_SYS}-"
#TARGET_CC_ARCH = "${EXOTIC_TARGET_CC_ARCH}"
#TARGET_LD_ARCH = "${EXOTIC_TARGET_LD_ARCH}"
#TARGET_AS_ARCH = "${EXOTIC_TARGET_AS_ARCH}"

# Replace the bitbake.conf exports with the EXOTIC equivalent
#export CPPFLAGS = "${EXOTIC_TARGET_CPPFLAGS}"
#export CFLAGS = "${EXOTIC_TARGET_CFLAGS}"
#export CXXFLAGS = "${EXOTIC_TARGET_CXXFLAGS}"
#export LDFLAGS = "${EXOTIC_TARGET_LDFLAGS}"
#TARGET_CPPFLAGS = "${EXOTIC_TARGET_CPPFLAGS}"
#TARGET_CFLAGS = "${EXOTIC_TARGET_CFLAGS}"
#TARGET_CXXFLAGS = "${EXOTIC_TARGET_CXXFLAGS}"
#TARGET_LDFLAGS = "${EXOTIC_TARGET_LDFLAGS}"

# Now use these updated variables with cross-canadian
inherit cross

# Now override things that we need changed
