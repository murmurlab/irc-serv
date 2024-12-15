
NICK=irccat31
SERVER=10.11.6.11
PORT=6667
CHAN="&irccat-kanel"
PASS="1234"

crlf=$'\r'

{
# join channel and say hi
cat << IRC
PASS :$PASS$crlf
NICK $NICK$crlf
USER irccat 8 x : irccat$crlf
JOIN $CHAN$crlf
JOIN $CHAN,&aa$crlf
PRIVMSG $CHAN :Greetings!$crlf
PRIVMSG $CHAN :Greetin2222gs!$crlf
IRC

  # forward messages from STDIN to the chan, indefinitely
  while read line ; do
    echo "$line"
  done

  # close connection
  echo QUIT
} | nc -C $SERVER $PORT

# | while read line ; do
#   case "$line" in
#     *PRIVMSG\ $CHAN\ :*) echo "$line" | cut -d: -f3- ;;
#     #*) echo "[IGNORE] $line" >&2 ;;
#   esac
# done
