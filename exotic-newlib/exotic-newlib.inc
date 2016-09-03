##
## modelled on combined version of gcc-configure-common.inc and gcc-target.inc
##          no need to have a separate newlib-configure-common.inc
##          also much simpler so significant parts taken from libgcc
##

require exotic-newlib/exotic-newlib-shared-source.inc

## virtual/${EXOTIC_TARGET_PREFIX}binutils - this is required by virtual/${EXOTIC_TARGET_PREFIX}gcc
DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${EXOTIC_TARGET_PREFIX}g++"

# Ensure that static libraries are built
DISABLE_STATIC = ""

PACKAGES = "\
    ${PN} \
    ${PN}-dev \
    ${PN}-staticdev \
    ${PN}-dbg \
    "

FILES_${PN} = " \
    ${prefix}/${EXOTIC_TARGET_SYS}/include/* \
    "
FILES_${PN}-dev = "\
    "
FILES_${PN}-dbg += "${prefix}/lib/${EXOTIC_TARGET_SYS}/.debug/"
FILES_${PN}-staticdev = "	\
    ${prefix}/lib/${EXOTIC_TARGET_SYS}/* \
    "

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
## at the moment it looks like pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-e-os')
HOST_OS="${TARGET_VENDOR_GVARIABLE}-${TARGET_OS_GVARIABLE}"

BPN = "newlib"

##
## configure, compile and install are modelled on libgcc
##
do_configure () {
    target=${EXOTIC_TARGET_SYS}
    install -d ${D}${libdir}

    mkdir -p ${B}/$target/${BPN}/
    cd ${B}/$target/${BPN}
    chmod a+x ${S}/${BPN}/configure
    ${S}/${BPN}/configure ${CONFIGUREOPTS} ${EXTRA_OECONF}
}

do_compile () {
    target=${EXOTIC_TARGET_SYS}
    cd ${B}/$target/${BPN}
    oe_runmake MULTIBUILDTOP=${B}/$target/${BPN}
}

do_install () {
    target=${EXOTIC_TARGET_SYS}
    cd ${B}/$target/${BPN}
    oe_runmake 'DESTDIR=${D}' MULTIBUILDTOP=${B}/$target/ install

    # install the runtime in /usr/lib/${EXOTIC_TARGET_SYS} not in /usr/${EXOTIC_TARGET_SYS}/lib on target
    ## TODO add reason
    mkdir -p ${D}${libdir}
    mv ${D}/usr/$target/lib/* ${D}${libdir}

    # Tidy up remaining folders
    rm -rf ${D}/${prefix}/${EXOTIC_TARGET_SYS}/lib	
    rm -rf ${D}/${prefix}/share
}

## nativesdk build is broken
##BBCLASSEXTEND = "nativesdk"
