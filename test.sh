#!/bin/bash
script_dir=$(dirname $0)
for filename in $script_dir/tests/*.in; do
	echo ..................................................$filename
	output=$script_dir/tests/$(basename "$filename" .in).out
	answer=$script_dir/tests/$(basename "$filename" .in).std
	$script_dir/a.out $filename >$output
	diff $output $answer && rm -f $output
done
for filename in $script_dir/tests/*.errin; do
	echo ..................................................$filename
	output=$script_dir/tests/$(basename "$filename" .errin).out
	answer=$script_dir/tests/$(basename "$filename" .errin).std
	$script_dir/a.out $filename 2>$output >/dev/null
	diff $output $answer && rm -f $output
done
total=$(find $script_dir/tests/*.std 2>/dev/null | wc -l)
failed=$(find $script_dir/tests/*.out 2>/dev/null | wc -l)
passed=`expr $total - $failed`
echo $passed of $total tests passed