SUMMARY = "GNU cc and gcc C compilers"
HOMEPAGE = "http://www.gnu.org/software/gcc/"
SECTION = "devel"
LICENSE = "GPL"

NATIVEDEPS = ""

inherit autotools_exotic gettext texinfo

BPN = "${EXOTIC_TARGET_PREFIX}gcc"

#
# Set some default values
#

BINV = "${PV}"

S = "${TMPDIR}/work-shared/${EXOTIC_TARGET_PREFIX}gcc-${PV}-${PR}/git"
B = "${WORKDIR}/${EXOTIC_TARGET_PREFIX}gcc-${PV}/build.${HOST_SYS}.${EXOTIC_TARGET_SYS}"

oldincludedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
includedir="${exec_prefix}/${EXOTIC_TARGET_SYS}/include"
# Update bitbake.conf setting - libdir="${exec_prefix}/${baselib}"
libdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"
#   following seems to be auto added gcc/${EXOTIC_TARGET_SYS}/${BINV}
# Update bitbake.conf setting - libexecdir = "${libdir}/${BPN}"
#   following seems to be auto added gcc/${EXOTIC_TARGET_SYS}/${BINV}
libexecdir="${exec_prefix}/${baselib}/${EXOTIC_TARGET_SYS}"
gcclibdir = "${libdir}/gcc"

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

# We need to ensure that for the shared work directory, the do_patch singatures match
# The real WORKDIR location isn't a dependency for the shared workdir.
src_patches[vardepsexclude] = "WORKDIR"
should_apply[vardepsexclude] += "PN"
