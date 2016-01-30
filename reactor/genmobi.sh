#!/bin/bash

# this sample script is to generate a .epub and a .mobi
# ebooks.
# Ensure the ebook-convert utility (from Calibre) are in
# your PATH environment.

trap "echo Abort; exit" SIGINT

#name=`pwd|perl -e '$d=<>;$d=~s{.*/}{}g;print $d'`
name='reactor'

export PATH=$PWD/../..:$PATH

echo "Generating HTMLs from ./ for $name..."
src2html.pl --navigator --color --cross-reference --line-numbers . $name

echo "Generating .pdf file for $name..."
rm -rf *.pdf
ebook-convert html_out/index.html $name.pdf \
	--override-profile-size \
	--paper-size a4 \
	--pdf-default-font-size 12 \
	--pdf-mono-font-size 12 \
	--margin-left 10 --margin-right 10 \
	--margin-top 10 --margin-bottom 10 \
	--page-breaks-before='/'

