#!/bin/sh

echo 'hello \''\\\n\0echo';
var='hello \''\\\n\0echo';
echo $var
echo ${var}
