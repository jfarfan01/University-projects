printf "reference: "
stat -c '%s' reference

printf "volatile: "
stat -c '%s' volatile

printf "for loop in main: "
stat -c '%s' main_for_loop

printf "designated initializer, zero: "
stat -c '%s' designated_zero

printf "designated initializer, non-zero: "
stat -c '%s' designated_non-zero

printf "static: "
stat -c '%s' static
