##
## modelled on libgcc-common.inc
##

BPN = "libgloss"

INHIBIT_DEFAULT_DEPS = "1"

# Override bitbake.conf settings

TUNE_CCARGS=""

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
    oe_runmake MULTIBUILDTOP=${B}/$target/${BPN}/
}

do_install () {
    target=${EXOTIC_TARGET_SYS}
    cd ${B}/$target/${BPN}
    oe_runmake 'DESTDIR=${D}' MULTIBUILDTOP=${B}/$target/${BPN}/ install

    # install the runtime in /usr/lib/${EXOTIC_TARGET_SYS} not in /usr/${EXOTIC_TARGET_SYS}/lib on target
    ## TODO add reason
    mkdir -p ${D}${libdir}
    mv ${D}/usr/$target/lib/* ${D}${libdir}

    # Tidy up remaining folders
    rm -rf ${D}/${prefix}/${EXOTIC_TARGET_SYS}
}

## nativesdk build is broken
##BBCLASSEXTEND = "nativesdk"
