##
## modelled on gcc-common.inc
##

NATIVEDEPS = ""

inherit autotools_exotic gettext texinfo

INHIBIT_DEFAULT_DEPS = "1"

#
# Set some default values
#

BINV = "${PV}"

# Source and Build shared by newlib and libgloss
#S = "${WORKDIR}/${EXOTIC_TARGET_PREFIX}newlib-${PV}"
#S = "${TMPDIR}/work-shared/${EXOTIC_TARGET_PREFIX}gcc-${PV}-${PR}/${EXOTIC_TARGET_PREFIX}newlib-${PV}"

## TODO this may not need to be shared so consider replacing this with normal source store
S = "${TMPDIR}/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-${PR}/git"
#B = "${WORKDIR}/${EXOTIC_TARGET_PREFIX}newlib-${PV}/build.${HOST_SYS}.${EXOTIC_TARGET_SYS}"
B = "${WORKDIR}/git/build.${HOST_SYS}.${EXOTIC_TARGET_SYS}"

# SS means Shared Stamps directory
SS = "${TMPDIR}/stamps/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-${PR}"
do_fetch[stamp-base] = "${SS}"
do_unpack[stamp-base] = "${SS}"
do_patch[stamp-base] = "${SS}"
do_preconfigure[stamp-base] = "${SS}"
SSCLEAN = "${TMPDIR}/stamps/work-shared/${EXOTIC_TARGET_PREFIX}newlib-[0-9]*-*"
do_fetch[stamp-base-clean] = "${SSCLEAN}"
do_unpack[stamp-base-clean] = "${SSCLEAN}"
do_unpack[umask] = "022"
do_patch[stamp-base-clean] = "${SSCLEAN}"
do_preconfigure[stamp-base-clean] = "${SSCLEAN}"

# SW means Shared Work directory
SW = "${TMPDIR}/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-${PR}"
SSTATE_SWSPEC = "sstate:${EXOTIC_TARGET_PREFIX}newlib::${PV}:${PR}::${SSTATE_VERSION}:"
WORKDIR_task-unpack = "${SW}"
WORKDIR_task-patch = "${SW}"
WORKDIR_task-preconfigure = "${SW}"

oldincludedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
includedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
# install the runtime in /usr/lib/${EXOTIC_TARGET_SYS} not in /usr/${EXOTIC_TARGET_SYS}/lib on target
## TODO add reason
# Update bitbake.conf setting - libdir="${exec_prefix}/${baselib}"
libdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"
# Update bitbake.conf setting - libexecdir = "${libdir}/${BPN}"
libexecdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"

target_includedir ?= "${includedir}"
target_libdir ?= "${libdir}"
target_base_libdir ?= "${base_libdir}"
target_prefix ?= "${prefix}"

export CC_FOR_TARGET="${EXOTIC_TARGET_PREFIX}gcc"
export GCC_FOR_TARGET="${EXOTIC_TARGET_PREFIX}gcc"
export CXX_FOR_TARGET="${EXOTIC_TARGET_PREFIX}g++"
export AS_FOR_TARGET="${EXOTIC_TARGET_PREFIX}as"
export LD_FOR_TARGET="${EXOTIC_TARGET_PREFIX}ld"
export NM_FOR_TARGET="${EXOTIC_TARGET_PREFIX}nm"
export AR_FOR_TARGET="${EXOTIC_TARGET_PREFIX}ar"
export RANLIB_FOR_TARGET="${EXOTIC_TARGET_PREFIX}ranlib"

#export CC_FOR_HOST = "${CCACHE}${HOST_PREFIX}gcc ${HOST_CC_ARCH}"
#export CXX_FOR_HOST = "${CCACHE}${HOST_PREFIX}gcc ${HOST_CC_ARCH}"

#export CC_FOR_BUILD = "${BUILD_CC}"

CLEANFUNCS += "workshared_clean"
# The do_clean should be exclusive since share ${S}
do_clean[lockfiles] = "${SW}.clean.lock"

python workshared_clean () {
    """clear the source directory"""
    dir = d.expand("${SW}")
    bb.note("Removing " + dir)
    oe.path.remove(dir)

    """clear the the stamps in work-shared"""
    dir = "%s.*" % bb.data.expand(d.getVarFlag('do_fetch', 'stamp-base', True), d)
    bb.note("Removing " + dir)
    oe.path.remove(dir)
}

# We need to ensure that for the shared work directory, the do_patch singatures match
# The real WORKDIR location isn't a dependency for the shared workdir.
src_patches[vardepsexclude] = "WORKDIR"
should_apply[vardepsexclude] += "PN"

