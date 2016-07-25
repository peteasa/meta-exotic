
BPN = "libgcc"

require exotic-gcc/exotic-gcc-shared-source.inc

INHIBIT_DEFAULT_DEPS = "1"

# Override bitbake.conf settings

TUNE_CCARGS=""

do_configure () {
	target=${EXOTIC_TARGET_SYS}
	install -d ${D}${base_libdir} ${D}${libdir}

	hardlinkdir ${STAGING_INCDIR_NATIVE}/../${EXOTIC_TARGET_SYS}/include/${EXOTICLIBGCCBUILDTREENAME}$target/ ${B}
	## mkdir -p ${B}/${BPN}
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

	# unwind.h is installed here which is shipped in gcc-cross
	# as well as target gcc and they are identical so we dont
	# ship one with libgcc here
	rm -rf ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include
}

## nativesdk build is broken
## BBCLASSEXTEND = "nativesdk"
