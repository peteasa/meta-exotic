##################################################################
# The intention of this layer is to provide an easy to use
# way of building exotic (ie non-native) toolchains.
#
# For example an arm target may have an avr processor attached.
# This layer enables the toolchain to be built for the avr processor
# so that avr code can be built within the yocto sdk and on target
# and that avr code can then be included in the distribution.
##################################################################

# Typically autotools_exotic is inherited before this so 
#   TARGET ??_GVARIABLE are set
#   EXOTIC_??

MODIFYTOS = "0"

# Now use these updated variables with cross-canadian
inherit cross-canadian

# Now override things that we need changed
