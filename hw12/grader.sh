RUN="./hw*_test"
for ((i=1; i<=1000; i++)) do
  output="$($RUN 2>&1))"
  if [[ "$?" != "100" ]]; then
    echo "$output Failed at run #$i"; break; fi; done
