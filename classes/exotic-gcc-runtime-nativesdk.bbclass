require exotic-gcc/exotic-gcc-runtime.inc

## move the libraries to where the compiler looks for them
FILES_${PN}-dbg = "\
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libstdc++.*-gdb.py \
    ${datadir}/gcc-${BINV}/python/libstdcxx \
"

FILES_${PN}-libstdc++ = "${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libstdc++.so.*"
FILES_${PN}-libstdc++-dev = "\
    ${includedir}/c++/ \
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libstdc++.so \
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libstdc++.la \
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libsupc++.la \
"
FILES_${PN}-libstdc++-staticdev = "\
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libstdc++.a \
    ${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libsupc++.a \
"

inherit nativesdk-exotic

deltask do_configure
deltask do_compile

DEPENDS = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH}"

do_install () {
	   # whilst not fool proof this is the best that can be done for now
	   mkdir -p ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/c++ ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include/

	   mkdir -p ${D}${exec_prefix}/lib/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libstdc* ${D}${exec_prefix}/lib/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libsupc* ${D}${exec_prefix}/lib/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/

	   mkdir -p ${D}${exec_prefix}/share/gcc-${BINV}/python
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/share/gcc-${BINV}/python/libstdcxx ${D}${exec_prefix}/share/gcc-${BINV}/python/
}
