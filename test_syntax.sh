#!/bin/bash
script_dir=$(dirname $0)
for filename in $script_dir/tests/syntax/*.in; do
	echo ..................................................$filename
	output=$script_dir/tests/syntax/$(basename "$filename" .in).out
	answer=$script_dir/tests/syntax/$(basename "$filename" .in).std
	$script_dir/a.out $filename --syntax >$output
	diff $output $answer && rm -f $output
done
total=$(find $script_dir/tests/syntax/*.std 2>/dev/null | wc -l)
failed=$(find $script_dir/tests/syntax/*.out 2>/dev/null | wc -l)
passed=`expr $total - $failed`
echo $passed of $total tests passed