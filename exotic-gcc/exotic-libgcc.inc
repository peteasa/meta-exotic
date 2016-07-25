require exotic-gcc/exotic-libgcc-common.inc

DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${EXOTIC_TARGET_PREFIX}g++ "

PACKAGES = "\
    ${PN} \
    ${PN}-dev \
    ${PN}-libgcov \
"

# TODO does not like     ${@base_conditional('TARGET_SYS_NO_EXTENSION', '${EXOTIC_TARGET_SYS}', '', '${libdir}/${TARGET_SYS_NO_EXTENSION}', d)}

FILES_${PN} = "${base_libdir}/libgcc*.so.*"

ALLOW_EMPTY_${PN} = "1"

FILES_${PN}-dev = "\
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/*crt* \
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libgcc* \
"
FILES_${PN}-libgcov = "\
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libgcov.a \
"

EXOTICLIBGCCBUILDTREENAME = "gcc-build-internal-"

do_package[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_ipk[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_deb[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_rpm[depends] += "virtual/${MLPREFIX}libc:do_packagedata"

INSANE_SKIP_${PN}-dev = "staticdev"
INSANE_SKIP_${PN}-libgcov = "staticdev"

## Following causes a problem with the SDK creation!  HOST_OS is here fixed
## so SDK build does not fully work
## look at package_rpm.bbclass -
## need to override pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-poky-linux-gnueabi')
## at the moment it looks like pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-e-os')
HOST_OS="${TARGET_VENDOR_GVARIABLE}-${TARGET_OS_GVARIABLE}"
