inherit cross-exotic

INHIBIT_DEFAULT_DEPS = "1"
EXTRADEPENDS = ""

DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}binutils ${EXTRADEPENDS} ${NATIVEDEPS}"
PROVIDES = "virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${EXOTIC_TARGET_PREFIX}g++"
python () {
    if d.getVar("EXOTIC_TARGET_OS", True).startswith("linux"):
        d.setVar("EXTRADEPENDS", "linux-libc-headers")
}

PN = "${EXOTIC_TARGET_PREFIX}gcc-cross-${EXOTIC_TARGET_ARCH}"

require exotic-gcc/exotic-gcc-configure-common.inc

EXTRA_OECONF += "\
    --with-cross-host=${TARGET_SYS_GVARIABLE} \
    --enable-poison-system-directories \
"

EXTRA_OECONF_PATHS = "\
    --with-libtool-sysroot=${STAGING_DIR_HOST} \
    --with-native-system-header-dir=${STAGING_DIR_TARGET}${target_includedir} \
    --with-gxx-include-dir=${STAGING_DIR_TARGET}${target_includedir}/c++/${BINV} \
    --with-sysroot=${STAGING_DIR_TARGET} \
    --with-build-sysroot=${STAGING_DIR_TARGET} \
"

ARCH_FLAGS_FOR_TARGET += "-isystem${STAGING_DIR_TARGET}${target_includedir}"

do_configure_prepend () {
	sed -i 's/BUILD_INFO=info/BUILD_INFO=/' ${S}/gcc/configure
}

do_compile () {
	export CC="${BUILD_CC}"
	export AR_FOR_TARGET="${EXOTIC_TARGET_SYS}-ar"
	export RANLIB_FOR_TARGET="${EXOTIC_TARGET_SYS}-ranlib"
	export LD_FOR_TARGET="${EXOTIC_TARGET_SYS}-ld"
	export NM_FOR_TARGET="${EXOTIC_TARGET_SYS}-nm"
	export CC_FOR_TARGET="${CCACHE} ${EXOTIC_TARGET_SYS}-gcc"
	export CFLAGS_FOR_TARGET="${EXOTIC_TARGET_CFLAGS}"
	export CPPFLAGS_FOR_TARGET="${EXOTIC_TARGET_CPPFLAGS}"
	export CXXFLAGS_FOR_TARGET="${EXOTIC_TARGET_CXXFLAGS}"
	export LDFLAGS_FOR_TARGET="${EXOTIC_TARGET_LDFLAGS}"

	oe_runmake all-host configure-target-libgcc
	# now generate script to drive testing
	echo "#!/usr/bin/env sh" >${B}/${EXOTIC_TARGET_PREFIX}testgcc
	set >> ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	# prune out the unneeded vars
	sed -i -e "/^BASH/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^USER/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^OPT/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^DIRSTACK/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^EUID/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^FUNCNAME/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^GROUPS/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^HOST/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^HOME/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^IFS/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^LC_ALL/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^LOGNAME/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^MACHTYPE/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^OSTYPE/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PIPE/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^SHELL/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^'/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^UID/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^TERM/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PATCH_GET/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PKG_/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^POSIXLY_/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PPID/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PS4/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^Q/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^SHLVL/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^STAGING/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^LD_LIBRARY_PATH/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc
	sed -i -e "/^PSEUDO/d" ${B}/${EXOTIC_TARGET_PREFIX}testgcc

	# append execution part of the script
cat >> ${B}/${EXOTIC_TARGET_PREFIX}testgcc << STOP
target="\$1"
usage () {
	echo "Usage:"
	echo "\$0 user@target 'extra options to dejagnu'"
	echo "\$0 target 'extra options to dejagnu'"
	echo "\$0 target"
	echo "e.g. \$0 192.168.7.2 ' dg.exp=visibility-d.c'"
	echo "will only run visibility-d.c test case"
	echo "e.g. \$0 192.168.7.2 '/-mthumb dg.exp=visibility-d.c'"
	echo "will only run visibility-d.c test case in thumb mode"
	echo "You need to have dejagnu autogen expect installed"
	echo "on the build host"
    }
if [ "x\$target" = "x" ]
then
	echo "Please specify the target machine and remote user in form of user@target\n"
	usage
	exit 1;
fi

shift

echo "\$target" | grep "@" 2>&1 > /dev/null
if [ "x\$?" = "x0" ]
then
   user=\$(echo \$target | cut -d '@' -f 1)
   target=\$(echo \$target | cut -d '@' -f 2)
else
   user=\$USER
fi
ssh \$user@\$target date 2>&1 > /dev/null
if [ "x\$?" != "x0" ]
then
	echo "Failed connecting to \$user@\$target it could be because"
	echo "you don't have passwordless ssh setup to access \$target"
	echo "or sometimes host key has been changed"
	echo "in such case do something like below on build host"
	echo "ssh-keygen -f "~/.ssh/known_hosts" -R \$target"
	echo "and then try ssh \$user@\$target"

	usage
	exit 1
fi
	echo "lappend boards_dir [pwd]/../../.." > ${B}/site.exp
	echo "load_generic_config \"unix\"" > ${B}/${PACKAGE_ARCH}.exp
	echo "set_board_info username \$user" >> ${B}/${PACKAGE_ARCH}.exp
	echo "set_board_info rsh_prog ssh" >> ${B}/${PACKAGE_ARCH}.exp
	echo "set_board_info rcp_prog scp" >> ${B}/${PACKAGE_ARCH}.exp
	echo "set_board_info hostname \$target" >> ${B}/${PACKAGE_ARCH}.exp
	DEJAGNU=${B}/site.exp make -k check RUNTESTFLAGS="--target_board=${PACKAGE_ARCH}\$@"

STOP

	chmod +x ${B}/${EXOTIC_TARGET_PREFIX}testgcc

}

# Not good to strip exotic binaries
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

# Compute how to get from libexecdir to bindir in python (easier than shell)
BINRELPATH = "${@os.path.relpath(d.expand("${STAGING_DIR_NATIVE}${prefix_native}/bin/${EXOTIC_TARGET_SYS}"), d.expand("${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}"))}"

do_install () {
	( cd ${B}/${EXOTIC_TARGET_SYS}/libgcc; oe_runmake 'DESTDIR=${D}' install-unwind_h )
	oe_runmake 'DESTDIR=${D}' install-host

	install -d ${D}${target_base_libdir}
	install -d ${D}${target_libdir}

	# Insert symlinks into libexec so when tools without a prefix are searched for, the correct ones are
	# found. These need to be relative paths so they work in different locations.
	dest=${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	install -d $dest
	for t in ar as ld nm objcopy objdump ranlib strip gcc cpp ; do
		ln -sf ${BINRELPATH}/${EXOTIC_TARGET_PREFIX}$t $dest$t
		ln -sf ${BINRELPATH}/${EXOTIC_TARGET_PREFIX}$t ${dest}${EXOTIC_TARGET_PREFIX}$t
	done

	# Remove things we don't need but keep share/java
	for d in info man share/doc share/locale share/man share/info; do
		rm -rf ${D}${STAGING_DIR_NATIVE}${prefix_native}/$d
	done

	# libquadmath headers need to  be available in the gcc libexec dir
	install -d ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/
	cp ${S}/libquadmath/quadmath.h ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/
	cp ${S}/libquadmath/quadmath_weak.h ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/

	# We use libiberty from binutils
	find ${D}${exec_prefix}/lib -name libiberty.a | xargs rm -f
	find ${D}${exec_prefix}/lib -name libiberty.h | xargs rm -f

	# gcc-runtime installs libgcc into a special location in staging since it breaks doing a standalone build
	case ${PN} in
		*gcc-cross-${EXOTIC_TARGET_ARCH}|*gcc-crosssdk-${EXOTIC_TARGET_ARCH})
            dest=${D}/${includedir}/gcc-build-internal-${EXOTIC_TARGET_SYS}
			hardlinkdir . $dest
		;;
	esac
}

do_package[noexec] = "1"
do_packagedata[noexec] = "1"
do_package_write_ipk[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_package_write_deb[noexec] = "1"
