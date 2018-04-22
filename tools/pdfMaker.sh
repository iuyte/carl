#!/bin/sh

#Must be run in the root directory of your PROS Project
#Creates PDFs of all .c and .h files and places them in [project directory]/PDFs

#Requires the 'a2ps' and 'ps2pdf' packages

if [ -e "project.pros" ]
then

	rm -rf "docs/include"
	rm -rf "docs/src"
	rm -rf "tmp"
	mkdir -p "docs/src"
	mkdir -p "docs/include"
	mkdir -p "tmp/src"
	mkdir -p "tmp/include"

	#Make PDF's of .h files in the 'include' folder
	if [ -d "include" ]
	then
		for i in include/*.h
		do
			echo "Making PDF of $i"
			q=$(basename $i .h)
			latex="\\documentclass{article}
\\usepackage{minted}
\\\\begin{document}
\\inputminted{c}{include/$q.h}
\\end{document}"
			echo "$latex" > tmp/include/$q.tex
			pdflatex -shell-escape tmp/include/"$q".tex
		done

		rm *.log
		rm *.aux
		rm -r _minted*
		mv *.pdf "docs/include/"
	else
		echo "'include' directory not found. PDFs of header files won't be created"
	fi

	#Make PDF's of .c files in the 'src' folder
	if [ -d "src" ]
	then
		for i in src/*.c
		do
			echo "Making PDF of $i"
			q=$(basename $i .c)
			latex="\\documentclass{article}
\\usepackage{minted}
\\\\begin{document}
\\inputminted{c}{src/$q.c}
\\end{document}"
			echo "$latex" > tmp/src/$q.tex
			pdflatex -shell-escape tmp/src/"$q".tex
		done

		rm *.log
		rm *.aux
		rm -r _minted*
		mv *.pdf "docs/src/"
	else
		echo "'src' directory not found. PDFs of source files won't be created"
	fi
	rm -rf tmp
else
	echo "'project.pros' file not found. Are you sure this is the root directory of your project?"
fi
