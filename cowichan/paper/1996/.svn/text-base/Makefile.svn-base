#===============================================================#
# Makefile : rebuild paper					#
#===============================================================#

all : code.ps paper.ps

code.ps : code.dvi
	dvips -f code.dvi > code.ps

paper.ps : paper.dvi
	dvips -f paper.dvi > paper.ps

code.dvi : code.tex
	latex code
	latex code

paper.dvi : paper.tex
	latex paper
	bibtex paper
	latex paper
	latex paper

export :
	test -n ${DIR};						\
	make postscript EXPORT_DIR=${DIR}

postscript : code.dvi paper.dvi
	test -n ${EXPORT_DIR};					\
	dvips -f code.dvi > ${EXPORT_DIR}/code.ps
	test -n ${EXPORT_DIR};					\
	dvips -f paper.dvi > ${EXPORT_DIR}/paper.ps

clean : tidy
	@rm -f *.ps *.aux *.dvi *.log *.bbl *.blg

tidy :
	@rm -f *~
