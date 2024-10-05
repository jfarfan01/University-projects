printf "reference: "
stat -c '%s' ref

printf "just malloc "
stat -c '%s' w_malloc

printf "const: "
stat -c '%s' const

printf "for loop in main: "
stat -c '%s' loop


