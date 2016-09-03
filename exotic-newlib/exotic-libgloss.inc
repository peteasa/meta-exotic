##
## modelled libgcc.inc
##

require exotic-newlib/exotic-libgloss-common.inc

DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}binutils virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${EXOTIC_TARGET_PREFIX}g++ "

# Ensure that static libraries are built
DISABLE_STATIC = ""

PACKAGES = "\
    ${PN} \
    ${PN}-dev \
    ${PN}-staticdev \
    ${PN}-dbg \
"

FILES_${PN} = " \
    ${prefix}/lib/${EXOTIC_TARGET_SYS}/*.o \
    ${prefix}/lib/${EXOTIC_TARGET_SYS}/nosys.specs \
"
FILES_${PN}-dev = "\
"
FILES_${PN}-dbg += "${prefix}/lib/${EXOTIC_TARGET_SYS}/.debug/"
FILES_${PN}-staticdev = "	\
    ${prefix}/lib/${EXOTIC_TARGET_SYS}/*.a \
	"

# Not good to strip exotic binaries
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

NEWLIBBUILDTREENAME = "newlib-build-internal-"

do_package[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_ipk[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_deb[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_rpm[depends] += "virtual/${MLPREFIX}libc:do_packagedata"

INSANE_SKIP_${PN}-staticdev = "arch"
INSANE_SKIP_${PN}-dbg += "arch"

## Following causes a problem with the SDK creation!  HOST_OS is here fixed
## so SDK build does not fully work
## look at package_rpm.bbclass -
## need to override pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-poky-linux-gnueabi')
## at the moment it looks like pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-elf')
HOST_OS="${TARGET_VENDOR_GVARIABLE}-${TARGET_OS_GVARIABLE}"
