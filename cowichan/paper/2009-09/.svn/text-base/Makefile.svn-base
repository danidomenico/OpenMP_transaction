all : paper.pdf

paper.pdf : paper.tex
	pdflatex paper
	bibtex paper
	pdflatex paper
	pdflatex paper

tidy :
	@rm -f *~

clean : tidy
	@rm -f *.pdf *.aux *.dvi *.log *.bbl *.blg
