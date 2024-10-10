set pagination off
break main
run 1 12 > /dev/null
break 39
commands
    silent
    if (i % 5 == 0)
        echo @@@
        printf "start = %ld, end = %ld, step = %ld, i = %ld\n", start, end, step, i
    end
    continue
end
continue
