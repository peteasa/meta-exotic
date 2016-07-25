require exotic-newlib/exotic-libgloss.inc

FILES_${PN} = " \
    ${prefix}/${EXOTIC_TARGET_SYS}/lib/*.o \
"
FILES_${PN}-dbg += "${prefix}/${EXOTIC_TARGET_SYS}/lib/.debug/"
FILES_${PN}-staticdev = "	\
    ${prefix}/${EXOTIC_TARGET_SYS}/lib/*.a \
	"

inherit nativesdk-exotic

deltask do_configure
deltask do_compile

DEPENDS = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH}"

do_install () {
	   # whilst not fool proof this is the best that can be done for now
	   mkdir -p ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/cacheman* ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/crt0.o ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libepiphany.a ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libnosys.a ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
}
