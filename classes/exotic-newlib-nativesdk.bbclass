require exotic-newlib/exotic-newlib.inc

## move the libraries to where the compiler looks for them
FILES_${PN}-dbg += "${prefix}/${EXOTIC_TARGET_SYS}/lib/.debug/"
FILES_${PN}-staticdev = "	\
    ${prefix}/${EXOTIC_TARGET_SYS}/lib/* \
	"

inherit nativesdk-exotic

## without empty do_configure and do_compile the DEPENDS condition has no effect!
## if newlib is installed then epiphany-elf-gcc-cross-canadian-epiphany does not compile
## unsure why not.
do_configure () {
}
do_compile () {
}

DEPENDS = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH}"

do_install () {
	   # whilst not fool proof this is the best that can be done for now
	   mkdir -p ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/*.h ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/bits  ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/machine  ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/rpc  ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include
	   cp -r ${STAGING_DIR}/${MACHINE}/usr/${EXOTIC_TARGET_SYS}/include/sys  ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/include

	   mkdir -p ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libc.a ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libg.a ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
	   cp ${STAGING_DIR}/${MACHINE}/usr/lib/${EXOTIC_TARGET_SYS}/libm.a ${D}${exec_prefix}/${EXOTIC_TARGET_SYS}/lib/
}
