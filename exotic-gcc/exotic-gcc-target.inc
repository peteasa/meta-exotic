GCCMULTILIB = "--enable-multilib"

require exotic-gcc/exotic-gcc-configure-common.inc

DEPENDS =+ "virtual/${EXOTIC_TARGET_SYS}-gcc ${EXOTIC_TARGET_SYS}-libgcc"

# Add paths of HOST and TARGET compilers
PATH_prepend = "${STAGING_BINDIR_TOOLCHAIN}:${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}:"

oldincludedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
includedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
# Update bitbake.conf setting - libdir="${exec_prefix}/${baselib}"
libdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"
#   following seems to be auto added gcc/${EXOTIC_TARGET_SYS}/${BINV}
# Update bitbake.conf setting - libexecdir = "${libdir}/${BPN}"
#   following seems to be auto added gcc/${EXOTIC_TARGET_SYS}/${BINV}
libexecdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"
gcclibdir = "${libdir}/gcc"

EXTRA_OECONF += "\
    --with-cross-host=${TARGET_SYS_GVARIABLE} \
"

EXTRA_OECONF_PATHS = "\
    --with-libtool-sysroot=${STAGING_DIR_HOST} \
    --with-native-system-header-dir=${STAGING_DIR_TARGET}${target_includedir} \
    --with-gxx-include-dir=${target_includedir}/c++/${BINV} \
    --with-sysroot=/ \
    --with-build-sysroot=${STAGING_DIR_TARGET} \
"

#oops something gone wrong -  ${EXOTIC_TARGET_PREFIX}cpp-symlinks
PACKAGES = "\
    ${PN} ${PN}-plugins ${PN}-symlinks \
    ${EXOTIC_TARGET_PREFIX}g++ ${EXOTIC_TARGET_PREFIX}g++-symlinks \
    ${EXOTIC_TARGET_PREFIX}cpp  \
    ${PN}-plugin-dev \
    ${PN}-doc \
    ${PN}-dev \
    ${PN}-dbg \
"

FILES_${PN} = "\
    ${bindir}/${EXOTIC_TARGET_PREFIX}gcov \
    ${bindir}/${EXOTIC_TARGET_PREFIX}gcc* \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/collect2 \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/cc* \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/lto* \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/lib*${SOLIBS} \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/liblto*${SOLIBSDEV} \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/*.o \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/specs \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/lib*${SOLIBS} \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include-fixed \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/fix* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/mk* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/gsys* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/macro* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/include \
"
INSANE_SKIP_${PN} += "dev-so"

FILES_${PN}-dbg += "\
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/.debug/ \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/plugin/.debug/ \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/install-tools/.debug/ \
"
#oops something gone wrong - nothing in this group
FILES_${PN}-dev = "\
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/lib*${SOLIBSDEV} \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/lib*${SOLIBSDEV} \
"
FILES_${PN}-plugin-dev = "\
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/include/ \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/plugin/gengtype \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/gtype.state \
"
ALLOW_EMPTY_${PN}-plugin-dev = "1"

#oops something gone wrong -    ${bindir}/${EXOTIC_TARGET_PREFIX}gccbug
FILES_${PN}-symlinks = "\
    ${bindir}/${EXOTIC_TARGET_PREFIX}gcc \
    ${bindir}/${EXOTIC_TARGET_PREFIX}cc \
"

FILES_${PN}-plugins = "\
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/include/ \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/gengtype \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/gtype.state \
"
ALLOW_EMPTY_${PN}-plugins = "1"

#oops something gone wrong -   ${base_libdir}/${EXOTIC_TARGET_PREFIX}cpp
FILES_${EXOTIC_TARGET_PREFIX}cpp = "\
    ${bindir}/${EXOTIC_TARGET_PREFIX}cpp \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/cc1 \
"
#oops something gone wrong -   FILES_${EXOTIC_TARGET_PREFIX}cpp-symlinks = "${bindir}/${EXOTIC_TARGET_PREFIX}cpp"

FILES_${EXOTIC_TARGET_PREFIX}gcov = "${bindir}/${EXOTIC_TARGET_PREFIX}gcov"
FILES_${EXOTIC_TARGET_PREFIX}gcov-symlinks = "${bindir}/gcov"

FILES_${EXOTIC_TARGET_PREFIX}g++ = "\
    ${bindir}/${EXOTIC_TARGET_PREFIX}g++ \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/cc1plus \
"
#oops something gone wrong -    ${bindir}/${EXOTIC_TARGET_PREFIX}g++
FILES_${EXOTIC_TARGET_PREFIX}g++-symlinks = "\
    ${bindir}/${EXOTIC_TARGET_PREFIX}c++ \
"

FILES_${PN}-doc = "\
    ${infodir} \
    ${mandir} \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include/README \
"

# Skip the architecture qa check
# this allows EXOTIC_TARGET_ARCH code to be packaged alongside arm code
INSANE_SKIP_${PN} += "arch"
INSANE_SKIP_${PN}-dbg += "arch"
INSANE_SKIP_${PN}-plugins += "arch"
INSANE_SKIP_${EXOTIC_TARGET_PREFIX}g++ += "arch"
INSANE_SKIP_${EXOTIC_TARGET_PREFIX}cpp += "arch"

do_compile () {
	oe_runmake all-host
}

do_install () {
	oe_runmake 'DESTDIR=${D}' install-host

	# Add unwind.h, it comes from libgcc which we don't want to build again
	install ${STAGING_LIBDIR_NATIVE}/${EXOTIC_TARGET_SYS}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/unwind.h ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/

	# Info dir listing isn't interesting at this point so remove it if it exists.
	if [ -e "${D}${infodir}/dir" ]; then
		rm -f ${D}${infodir}/dir
	fi

	# Cleanup some of the ${libdir}{,exec}/${EXOTIC_TARGET_PREFIX}gcc stuff ...
	#rm -r ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/install-tools
	#rm -r ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/install-tools
	rm -rf ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/*.la
	#rmdir ${D}${includedir}
	#rm -rf ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/finclude

	# Hack around specs file assumptions
	test -f ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/specs && sed -i -e '/^*cross_compile:$/ { n; s/1/0/; }' ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/specs

	# Cleanup manpages..
	rm -rf ${D}${mandir}/man7

	cd ${D}${bindir}

	# We care about g++ not c++
	rm -f *c++

	# We don't care about the gcc-<version> ones for this
	rm -f *gcc-?.?*

	# We use libiberty from binutils
	#find ${D}/usr/${EXOTIC_TARGET_SYS} -name libiberty.a | xargs rm -f
	find ${D}/usr/${EXOTIC_TARGET_SYS} -name libiberty.h | xargs rm -f
	rm -rf ${D}/usr/${EXOTIC_TARGET_SYS}

	# Not sure why we end up with these but we don't want them...
	rm -f ${EXOTIC_TARGET_PREFIX}${EXOTIC_TARGET_PREFIX}*

	ln -sf ${EXOTIC_TARGET_PREFIX}g++ ${EXOTIC_TARGET_PREFIX}c++
	ln -sf ${EXOTIC_TARGET_PREFIX}gcc ${EXOTIC_TARGET_PREFIX}cc

	chown -R root:root ${D}
}
