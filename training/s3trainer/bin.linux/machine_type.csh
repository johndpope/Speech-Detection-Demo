#!/bin/csh -f
#
# Brain-dead thing to get some limited automatic machine
# type determination
#

###set mach_name = `hostname | tr '[a-z]' '[A-Z]'`

if (-f /usr/bin/sparc && { /usr/bin/sparc } ) then
	echo "sun"
	exit 0
endif

if (-f /hp-ux) then
	echo "hppa"
	exit 0
endif

if (-f /vmlinuz) then
	echo "linux"
	exit 0
endif

if (-f /boot/vmlinuz) then
	echo "linux"
	exit 0
endif

if (-f /boot/vmlinuz-2.0.36-0.7) then
	echo "linux"
	exit 0
endif

##if (-f /proc/version) then
##    if (`cat /proc/version` =~ Linux) then
##	echo "linux"
##	exit 0
##    endif
##endif

if (-f /vmunix && -f /osf_boot) then
	echo "alpha"
	exit 0
endif

echo "linux"

exit 1
