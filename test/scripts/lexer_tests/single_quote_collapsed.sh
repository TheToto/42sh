#!/bin/sh

echo 'hello \''\\\0echo';
var='hello \''\\\0echo';
echo $var
echo ${var}
