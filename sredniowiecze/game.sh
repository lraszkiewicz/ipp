#!/bin/bash

n=10 # default values
k=100
s=1

args=("$@")

nonnegative_int_regex='^[0-9]{1,10}$'

invalid_args() {
    if [[ -z $1 ]]
    then
        echo "Invalid argument."
    else
        echo $1
    fi
    exit 1
}

set_n() {
    n=$1
    if ! [[ $n =~ $nonnegative_int_regex ]] || ((n <= 8)) || ((n >= 2147483648))
    then
        invalid_args "Invalid n."
    fi
}

set_k() {
    k=$1
    if ! [[ $k =~ $nonnegative_int_regex ]] || ((k < 1)) || ((k >= 2147483648))
    then
        invalid_args "Invalid k."
    fi
}

set_s() {
    s=$1
    if ! [[ $s =~ $nonnegative_int_regex ]]
    then
        invalid_args "Invalid s."
    fi
}

set_p1() {
    p1=$1
    temp_array=(${p1//,/ })
    x1=${temp_array[0]}
    y1=${temp_array[1]}
    if ((${#temp_array[@]} != 2)) || ! [[ $x1 =~ $nonnegative_int_regex ]] || ! [[ $y1 =~ $nonnegative_int_regex ]] || ((x1 < 1)) || ((y1 < 1))
    then
        invalid_args "Invalid P1's king's position."
    fi
}

set_p2() {
    p2=$1
    temp_array=(${p2//,/ })
    x2=${temp_array[0]}
    y2=${temp_array[1]}
    if ((${#temp_array[@]} != 2)) || ! [[ $x2 =~ $nonnegative_int_regex ]] || ! [[ $y2 =~ $nonnegative_int_regex ]] || ((x2 < 1)) || ((y2 < 1))
    then
        invalid_args "Invalid P2's king's position."
    fi
}

set_ai1() {
    ai1=$1
    if ! [[ -x "$ai1" ]]
    then
        invalid_args "Invalid path to P1's AI."
    fi
}

set_ai2() {
    ai2=$1
    if ! [[ -x "$ai2" ]]
    then
        invalid_args "Invalid path to P2's AI."
    fi
}

for (( i=0; i<$#; i+=2 ))
do
    case ${args[$((i))]} in
        "-n") set_n ${args[$((i+1))]} ;;
        "-k") set_k ${args[$((i+1))]} ;;
        "-s") set_s ${args[$((i+1))]} ;;
        "-p1") set_p1 ${args[$((i+1))]} ;;
        "-p2") set_p2 ${args[$((i+1))]} ;;
        "-ai1") set_ai1 ${args[$((i+1))]} ;;
        "-ai2") set_ai2 ${args[$((i+1))]} ;;
        *) invalid_args "Unexpected argument."
    esac
done

# verify P1's king's position if set
if ! [[ -z $x1 ]] && ( ((x1 > n-3)) || ((y1 > n)) )
then
    invalid_args "Invalid P1's king's position."
fi

# verify P2's king's position if set
if ! [[ -z $x2 ]] && ( ((x2 > n-3)) || ((y2 > n)) )
then
    invalid_args "Invalid P2's king's position."
fi

# verify the distance between kings if both set
verify_king_distance() {
    x1=$1
    y1=$2
    x2=$3
    y2=$4
    if ! [[ -z $x1 ]] && ! [[ -z $x2 ]]
    then
        x_diff=$((x1-x2))
        y_diff=$((y1-y2))
        if ((x_diff < 0))
        then
            x_diff=$((-x_diff))
        fi
        if ((y_diff < 0))
        then
            y_diff=$((-y_diff))
        fi
        if ((x_diff > y_diff))
        then
            max_diff=$x_diff
        else
            max_diff=$y_diff
        fi
        if ((max_diff < 8))
        then
            echo "1"
        else
            echo "0"
        fi
    else
        echo "0" # it's fine if the position of one or both king isn't specified
    fi
}

if (($(verify_king_distance $x1 $y1 $x2 $y2) == 1))
then
    invalid_args "Kings too close."
fi

if [[ -z $x1 ]] || [[ -z $x2 ]] # need to choose something randomly
then
    # if exactly one king's position is set, check if there is place for another one
    if ! [[ -z $x1 ]] # P1 is set, P2 is not
    then
        x_only=$x1
        y_only=$y1
    fi
    if ! [[ -z $x2 ]] # P2 is set, P1 is not
    then
        x_only=$x2
        y_only=$y2
    fi
    if  ! [[ -z $x_only ]] && \
        (($(verify_king_distance $x_only $y_only 1 1) == 1)) && \
        (($(verify_king_distance $x_only $y_only 1 $n) == 1)) && \
        (($(verify_king_distance $x_only $y_only $((n-3)) 1) == 1)) && \
        (($(verify_king_distance $x_only $y_only $((n-3)) $n) == 1))
    then
        invalid_args "One king's position is set in a place which makes it impossible to choose second king's position."
    fi

    # choosing king's (or kings') position(s) randomly
    chosen=0
    while ((chosen == 0))
    do
        x1r=$(shuf -i 1-$((n-3)) -n 1)
        y1r=$(shuf -i 1-$n -n 1)
        x2r=$(shuf -i 1-$((n-3)) -n 1)
        y2r=$(shuf -i 1-$n -n 1)
        # P1 is already set
        if  ! [[ -z $x1 ]] && \
            (($(verify_king_distance $x1 $y1 $x2r $y2r) == 0))
        then
            x2=$x2r
            y2=$y2r
            chosen=1
        fi
        # P2 is already set
        if  ! [[ -z $x2 ]] && \
            (($(verify_king_distance $x1r $y1r $x2 $y2) == 0)) && \
            ((chosen == 0))
        then
            x1=$x1r
            y1=$y1r
            chosen=1
        fi
        # neither is already set
        if  [[ -z $x1 ]] && [[ -z $x2 ]] && \
            (($(verify_king_distance $x1r $y1r $x2r $y2r) == 0)) && \
            ((chosen == 0))
        then
            x1=$x1r
            y1=$y1r
            x2=$x2r
            y2=$y2r
            chosen=1
        fi
    done
fi

PIPE1=$(mktemp -u)
mkfifo $PIPE1
eval "exec 3<>${PIPE1}"
rm $PIPE1

PIPE2=$(mktemp -u)
mkfifo $PIPE2
eval "exec 4<>${PIPE2}"
rm $PIPE2

PIPE3=$(mktemp -u)
mkfifo $PIPE3
eval "exec 5<>${PIPE3}"
rm $PIPE3

PIPE4=$(mktemp -u)
mkfifo $PIPE4
eval "exec 6<>${PIPE4}"
rm $PIPE4

PIPE5=$(mktemp -u)
mkfifo $PIPE5
eval "exec 7<>${PIPE5}"
rm $PIPE5

PIPE6=$(mktemp -u)
mkfifo $PIPE6
eval "exec 8<>${PIPE6}"
rm $PIPE6

is_ai1=true
is_ai2=true

if [[ -z $ai1 ]]
then
    human1="-human1"
    is_ai1=false
fi

if [[ -z $ai2 ]]
then
    human2="-human2"
    is_ai2=false
fi

./sredniowiecze_gui_with_libs.sh $human1 $human2 <&7 >&8 &
gui_pid=$!

if $is_ai1
then
    "$ai1" <&3 >&4 &
    ai1_pid=$!
    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
fi
echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&7

if $is_ai2
then
    "$ai2" <&5 >&6 &
    ai2_pid=$!
    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&5
fi
echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7

# pipes:
# 3 - ai1 input
# 4 - ai1 output
# 5 - ai2 input
# 6 - ai2 output
# 7 - gui input
# 8 - gui output

current_player=1

if (! $is_ai1) && (! $is_ai2)
then
    wait $gui_pid
    gui_exit_code=$?
    if ((gui_exit_code != 0))
    then
        exit 1
    else
        exit 0
    fi
fi

check_running() {
    ai1_isrunning=false
    ai2_isrunning=false
    gui_isrunning=false
    if $is_ai1 && (kill -0 $ai1_pid 2> /dev/null)
    then
        ai1_isrunning=true
    fi

    if $is_ai2 && (kill -0 $ai2_pid 2> /dev/null)
    then
        ai2_isrunning=true
    fi

    if kill -0 $gui_pid 2> /dev/null
    then
        gui_isrunning=true
    fi
}

while true
do
    check_running
    if ! ( ($ai1_isrunning || $ai2_isrunning) && $gui_isrunning )
    then
        break
    fi

    if ((current_player == 1))
    then
        if $is_ai1
        then
            str=
            read -t 0.1 str <&4
            while (kill -0 $ai1_pid 2> /dev/null) && [[ -z $str ]]
            do # nothing read, but AI is still running
                read -t 0.1 str <&4
            done
            echo "$str" >&7
            if $is_ai2
            then
                echo "$str" >&5
            fi
        else
            str=
            read -t 0.1 str <&8
            while (kill -0 $gui_pid 2> /dev/null) && [[ -z $str ]]
            do # nothing read, but GUI is still running
                read -t 0.1 str <&8
            done
            if $is_ai2
            then
                echo "$str" >&5
            fi
        fi
        if [[ "$str" == "END_TURN" ]]
        then
            if $is_ai1 && $is_ai2
            then
                sleep $s
            fi
            current_player=2
        fi
    fi
    if ((current_player == 2))
    then
        if $is_ai2
        then
            str=
            read -t 0.1 str <&6
            while (kill -0 $ai2_pid 2> /dev/null) && [[ -z $str ]]
            do # nothing read, but AI is still running
                read -t 0.1 str <&6
            done
            echo "$str" >&7
            if $is_ai1
            then
                echo "$str" >&3
            fi
        else
            str=
            read -t 0.1 str <&8
            while (kill -0 $gui_pid 2> /dev/null) && [[ -z $str ]]
            do # nothing read, but GUI is still running
                read -t 0.1 str <&8
            done
            if $is_ai1
            then
                echo "$str" >&3
            fi
        fi
        if [[ "$str" == "END_TURN" ]]
        then
            if $is_ai1 && $is_ai2
            then
                sleep $s
            fi
            current_player=1
        fi
    fi
done

check_running

if $is_ai1 && (! $ai1_isrunning)
then
    wait $ai1_pid
    ai1_exit_code=$?
fi

if $is_ai2 && (! $ai2_isrunning)
then
    wait $ai2_pid
    ai2_exit_code=$?
fi

if ! $gui_isrunning
then
    wait $gui_pid
    gui_exit_code=$?
fi

exit_code=0

if (! [[ -z $ai1_exit_code ]]) && ((ai1_exit_code != 0)) && ((ai1_exit_code != 1)) && ((ai1_exit_code != 2))
then
    exit_code=1
fi

if (! [[ -z $ai2_exit_code ]]) && ((ai2_exit_code != 0)) && ((ai2_exit_code != 1)) && ((ai2_exit_code != 2))
then
    exit_code=1
fi

if ((! [[ -z $gui_exit_code ]]) && ((gui_exit_code != 0)))
then
    exit_code=1
fi

if ((exit_code != 0))
then
    exit_code=1
    check_running
    if $ai1_isrunning
    then
        pkill -TERM -P $ai1_pid
    fi
    if $ai2_isrunning
    then
        pkill -TERM -P $ai2_pid
    fi
    if $gui_isrunning
    then
        pkill -TERM -P $gui_pid
    fi
fi

check_running

if $is_ai1 && $ai1_isrunning
then
    wait $ai1_pid
    ai1_exit_code=$?
fi

if $is_ai2 && $ai2_isrunning
then
    wait $ai2_pid
    ai2_exit_code=$?
fi

if $gui_isrunning
then
    wait $gui_pid
    gui_exit_code=$?
fi

exit $exit_code
