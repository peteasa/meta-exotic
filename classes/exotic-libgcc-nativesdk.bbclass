require exotic-gcc/exotic-libgcc.inc

inherit nativesdk-exotic

deltask do_configure
deltask do_compile

DEPENDS = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH}"

do_install () {
	   # whilst not fool proof this is the best that can be done for now
	   mkdir -p ${D}${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/crt*.o ${D}${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/libg*.a ${D}${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
}
