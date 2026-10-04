#!/bin/bash
# Compiles and runs every question with its sample input.
cd "$(dirname "$0")"
for d in Q1_Min_Coin_Change:min_coin_change Q2_Coin_Change_Ways:coin_ways Q3_LCS:lcs Q4_LIS:lis \
         Q5_Max_Sum_Increasing_Subsequence:msis Q6_Edit_Distance:edit_distance Q7_Rod_Cutting:rod_cutting Q8_Optimal_BST:obst; do
  dir=${d%%:*}; f=${d##*:}
  echo "=========== $dir"
  (cd "$dir" && gcc -O2 -o "$f" "$f.c" && ./"$f" < input.txt)
done
echo "=========== Q9_Collatz"
(cd Q9_Collatz && gcc -O2 -o collatz collatz.c && ./collatz < input_single.txt | tail -2 && ./collatz < input_interval.txt)
