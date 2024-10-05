gcc reference.c -o reference
printf "Reference: "
stat -c '%s' reference

gcc designated_null.c -o designated_null
printf "Designated null: "
stat -c '%s' designated_null

gcc designated_non_null.c -o designated_non_null
printf "Designated non null: "
stat -c '%s' designated_non_null

gcc loop.c -o loop
printf "Initialized with loop: "
stat -c '%s' loop

gcc memset.c -o memset
printf "Initialized with memset: "
stat -c '%s' memset

