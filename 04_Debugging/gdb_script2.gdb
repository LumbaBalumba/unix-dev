set pagination off
break main
run -100 100 3 > /dev/null
break 39
commands
    silent
    if (index >= 28 && index <= 35)
        echo @@@
        printf "start = %ld, end = %ld, step = %ld, i = %ld, index = %d\n", start, end, step, i, index
    end
    continue
end
continue
