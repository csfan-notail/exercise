if [ -n "${SERVICE_DEBUG_OUTPUT}" ]; then
	[ "${SERVICE_DEBUG_OUTPUT}" -eq 1 ] && export SVCDBG="debug_"
fi

debug_service_start() {
        local serv="`echo $1 | awk -F'/' '{print $NF}'`"

        "$@" >/dev/console 2>/dev/console &
        echo `ps | grep $1 | grep -v grep | awk '{print \$1}'`> /var/run/${serv}.pid
}

debug_service_stop() {
        local serv="`echo $1 | awk -F'/' '{print $NF}'`"
        local serv_pid="`ps | grep $1 | grep -v grep | awk '{print \$1}'`"

        [ -z "$serv_pid" ] || kill $serv_pid
        rm -f /var/run/${serv}.pid
}

whc_debug() {
	if [ -n "$WHC_DEBUG_OUTPUT" ]; then
		if [ "$WHC_DEBUG_OUTPUT" -gt 0 ]; then
				echo "${1}: ""$2"> /dev/console
		fi
	fi
}

whc_echo() {
	echo "${1}: ""$2"> /dev/console
}

whc_error() {
	echo "${1}: ERROR: ""$2"> /dev/console
}

local __LOCK

__whc_trap_cb() {
	whc_error $0 "unexpected termination"
}

# whc_lock
# input: $1: (optional) lock suffix, $2: (optional) trap callback
whc_lock() {
	local trap_cb="$2"

	__LOCK=/var/run/`echo $0 | awk -F/ '{print $NF}'`${1}.lock
	lock $__LOCK
	[ -z "$trap_cb" ] && trap_cb=__whc_trap_cb
	trap $trap_cb INT TERM ABRT QUIT ALRM
}

# whc_unlock
whc_unlock() {
	trap - INT TERM ABRT QUIT ALRM
	lock -u $__LOCK
}
