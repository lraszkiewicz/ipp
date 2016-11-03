if [[ $1 = '-v' ]]; then
   debug='-v'
   prog=$2
   directory=$3
else
   debug=''
   prog=$1
   directory=$2
fi

waCount=0
waErrCount=0

for file in `ls ${directory}/*.in | sort -V`; do
   filename="${file%.*}"
   echo -n "${filename##*/}: "
   "./$prog" $debug < "${filename}.in" > "temp.out" 2> "temp.err"
   if cmp -s "temp.out" "${filename}.out"; then
      echo -n "stdout OK"
   else
      echo -n "stdout WA"
      waArray[$waCount]=${filename##*/}
      waCount=$((waCount+1))
   fi
   if [[ $debug = '-v' ]]; then
      if cmp -s "temp.err" "${filename}.err"; then
         echo -n ", stderr OK"
      else
         echo -n ", stderr WA"
         waErrArray[$waErrCount]=${filename##*/}
         waErrCount=$((waErrCount+1))
      fi
   fi
   echo
done

if [[ $waCount > 0 ]]; then
   echo -n "stdout WA ($waCount tests): "
   for i in "${waArray[@]}"; do
      echo -n "$i "
   done
   echo
else
   echo "stdout OK on all tests."
fi

if [[ $debug = '-v' ]]; then
   if [[ $waErrCount > 0 ]]; then
      echo -n "stderr WA ($waCount tests): "
      for i in "${waErrArray[@]}"; do
         echo -n "$i "
      done
      echo
   else
      echo "stderr OK on all tests."
   fi
fi

rm -f temp.out temp.err
