
who=$1
echo "$(grep "^.*\s$who\s.*$" "log.txt")"
