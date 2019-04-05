#===============================================================#
# Makefile : re-make everything (see bottom for conventions)	#
#===============================================================#

#---------------------------------------------------------------#
# naming conventions						#
#---------------------------------------------------------------#

# no suffix	: tool
# single char	: file type
# *_c		: compiler
# *_d		: directory
# *_f		: flags
# *_h		: header files
# *_l		: library
# *_t		: targets

#---------------------------------------------------------------#
# values							#
#---------------------------------------------------------------#

# @ vals_f	:	values

vals_f = -DMAXEXT=128 -DMAXPAR=8 -DMAXVAL=1024

#---------------------------------------------------------------#
# systems							#
#---------------------------------------------------------------#

# @ cc		:	application compiler
# @ auxb_d	:	binaries directory for auxiliary programs
# @ plat_d	:	root directory for platform
# @ src_d	:	source directory for auxiliary programs
# @ root_d	:	root directory of compilation
# @ exe_f	:	flag to create an executable binary
# @ obj_f	:	flag to create an object
# @ plat_l	:	libraries for platform
# @ .${A}	:	archive file
# @ .${C}	:	source file
# @ .${H}	:	header file
# @ .${O}	:	object file

#----			  ---- SunOS 5 ----		    ----#

# @ x11_d	:	X11 header/library directory
# @ gfx_d	:	VOGLE graphics header/library directory
# @ gfx_f	:	graphics flags
# @ cc_d	:	compiler search directory
# @ sys_f	:	system-dependent flags
# @ sys_l	:	system libraries

x11_d_sunos5	=	/local/X11/lib
vogle_d_sunos5	=	/h/f/gvw
cc_d_sunos5	=	/cs/cs6/SUNWspro/SC3.0
sys_f_sunos5	=	-DSUNOS5=1 -DIEEE=1 -Wall -g		\
			-I${cc_d_sunos5}/include/cc		\
			-L${cc_d_sunos5}/lib			\
			-DGRAPHICS=1				\
			-I${vogle_d_sunos5}/include		\
			-L${vogle_d_sunos5}/lib			\
			-I${x11_d_sunos5}			\
			-R${x11_d_sunos5}			\
			-L${x11_d_sunos5}
sys_l_sunos5	=	-lvogle -lsocket -lnsl -lX11 -lsunmath -lm

# @ ar		:	library creation
# @ ar_f	:	archive installation flags
# @ ranlib	:	library cleanup
# @ txlt	:	.C->.c and .H->.h

ar_sunos5	=	ar
ar_f_sunos5	=	r
ranlib_sunos5	=	touch
txlt_sunos5	=	cat

sunos5		:
	make -f Makefile targets				\
		root_d=`pwd`					\
		sys_f="${sys_f_sunos5}"				\
		sys_l="${sys_l_sunos5}"				\
		serial_f=					\
		serial_l=					\
		parallel_f="-DMIMD=1 -D_REENTRANT"		\
		parallel_l=-lthread				\
		app_lf="-lapp -lutil"				\
		aux_c=gcc					\
		gen_c=gcc					\
		cc=gcc obj_f=-c exe_f=-o			\
		ar=${ar_sunos5} ar_f=${ar_f_sunos5}		\
		ranlib=${ranlib_sunos5} txlt=${txlt_sunos5}	\
		A=a C=c H=h O=o

#----			 ---- NUMAchine ----		    ----#

# @ sys_f	:	system-dependent flags
# @ sys_l	:	system libraries

sys_f_numa	=	-DNUMA=1 -non_shared -w -DGRAPHICS=0
sys_l_numa	=	-lm

# @ ar		:	library creation
# @ ar_f	:	archive installation flags
# @ ranlib	:	library cleanup
# @ txlt	:	.C->.c and .H->.h

ar_numa		=	ar
ar_f_numa	=	r
ranlib_numa	=	ranlib
txlt_m_dir	=	/stumm/d1/NUMAchine/benchmarks/splash/macros/sgi
txlt_m_numa	=	${txlt_m_dir}/c.m4.sgi
txlt_numa	=	"m4 -s ${txlt_m_numa} -Ulen"

numa		:
	make -f Makefile targets				\
		root_d=`pwd`					\
		sys_f="${sys_f_numa}"				\
		sys_l="${sys_l_numa}"				\
		serial_f=					\
		serial_l=					\
		parallel_f="-DMIMD=1"				\
		parallel_l=/usr/lib/libmpc.a			\
		app_lf="-lapp -lutil"				\
		aux_c=gcc					\
		gen_c=cc					\
		cc=cc obj_f=-c exe_f=-o				\
		ar=${ar_numa} ar_f=${ar_f_numa}			\
		ranlib=${ranlib_numa} txlt=${txlt_numa}		\
		A=a C=c H=h O=o

#---------------------------------------------------------------#
#---------------------------------------------------------------#
#								#
#		    DO NOT MODIFY BELOW THIS LINE		#
#								#
#---------------------------------------------------------------#
#---------------------------------------------------------------#

#---------------------------------------------------------------#
# how to rebuild everything					#
#---------------------------------------------------------------#

targets		:
	make -f Makefile auxil					\
		auxb_d=${root_d}/bin src_d=${root_d}/aux
	make -f Makefile binaries plat_d=${root_d}/ser		\
		plat_f="${serial_f}" plat_l="${serial_l}"
	make -f Makefile binaries plat_d=${root_d}/fj		\
		plat_f="${parallel_f}" plat_l="${parallel_l}"
	make -f Makefile binaries plat_d=${root_d}/bar		\
		plat_f="${parallel_f}" plat_l="${parallel_l}"

#---------------------------------------------------------------#
# source transformation						#
#---------------------------------------------------------------#

.SUFFIXES	: .C .H

.C.c		:
	$(txlt) $*.C > $*.c

.H.h		:
	$(txlt) $*.H > $*.h

#---------------------------------------------------------------#
# directories							#
#---------------------------------------------------------------#

# special directories

# @ generic_d : generic source directory
generic_d	=	${root_d}/generic

# relative directories

# @ app_d  : platform application source directory
# @ bin_d  : platform binary directory
# @ hdr_d  : platform header directory
# @ lib_d  : platform library directory
# @ main_d : platform main source directory

app_d		=	${plat_d}/app
bin_d		=	${plat_d}/bin
hdr_d		=	${plat_d}/hdr
lib_d		=	${plat_d}/lib
main_d		=	${plat_d}/main

#---------------------------------------------------------------#
# shared flags							#
#---------------------------------------------------------------#

# @ shr_f : shared flags (platform application objects and binaries)

shr_f		=	${vals_f} ${sys_f} ${plat_f} -L${generic_d}/lib

#---------------------------------------------------------------#
# header files							#
#---------------------------------------------------------------#

# @ app_h     : platform application header files
# @ generic_h : generic header files

app_h		=	${hdr_d}/generic.${H}			\
			${hdr_d}/gfx.${H}			\
			${hdr_d}/proto.${H}			\
			${hdr_d}/specific.${H}			\
			${hdr_d}/type.${H}

generic_h	=	${generic_d}/hdr/util.h			\
			${generic_d}/hdr/generic.h		\
			${generic_d}/hdr/gfx.h			\
			${generic_d}/hdr/proto.h		\
			${generic_d}/hdr/type.h

#---------------------------------------------------------------#
# libraries							#
#---------------------------------------------------------------#

# @ app_l  : platform application library
# @ util_l : generic utility library

app_l		=	${lib_d}/libapp.${A}

util_l		=	${generic_d}/lib/libutil.${A}

#---------------------------------------------------------------#
# housekeeping							#
#---------------------------------------------------------------#

# @ all_d   : all directories containing source
# @ clean_t : things to delete when cleaning
# @ tidy_t  : things to delete when tidying up

all_d		= bar fj generic ser
clean_t		= app/*.c bin/* hdr/*.h lib/* main/*.c
tidy_t		= ./*~ ./*/*~ ./*.o ./*/*.o

clean		:
	@cd aux; rm -f ${tidy_t}; cd ..
	@for p in ${all_d};  do					\
	  cd $$p;						\
	  make -f ../Makefile clean_p;				\
	  cd ..;						\
	done
	@cd generic/util; rm -f ${tidy_t} *.c; cd ../..
	@rm -f ./*.o

clean_p		:
	@rm -f ${clean_t} ${tidy_t}

tidy		:
	@cd aux; rm -f ${tidy_t}; cd ..
	@for p in ${all_d};  do					\
	  cd $$p;						\
	  make -f ../Makefile tidy_p;				\
	  cd ..;						\
	done
	@cd generic/util; rm -f ${tidy_t}; cd ../..
	@rm -f ./*.o

tidy_p		:
	@rm -f ${tidy_t}

clean_tests	:
	@for p in ${all_d}; do					\
	  cd $$p;						\
	  rm -f test/*;						\
	  cd ..;						\
	done

sterile		: clean clean_tests

#---------------------------------------------------------------#
# auxiliary programs						#
#---------------------------------------------------------------#

# @ aux_f : flags for auxiliary programs
# @ aux_l : libaries for auxiliary programs

aux_f		=	${shr_f} -g -o
aux_l		=	-lutil ${sys_l}

auxil		:	${auxb_d}/chk				\
			${auxb_d}/gen				\
			${auxb_d}/pgm				\
			${auxb_d}/sdu				\
			${auxb_d}/slc				\
			${auxb_d}/view

${auxb_d}/chk	:	${src_d}/chk.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/chk.c ${src_d}/aux.c ${aux_l}

${auxb_d}/gen	:	${src_d}/gen.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/gen.c ${src_d}/aux.c ${aux_l}

${auxb_d}/pgm	:	${src_d}/pgm.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/pgm.c ${src_d}/aux.c ${aux_l}

${auxb_d}/sdu	:	${src_d}/sdu.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/sdu.c ${src_d}/aux.c ${aux_l}

${auxb_d}/slc	:	${src_d}/slc.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/slc.c ${src_d}/aux.c ${aux_l}

${auxb_d}/view	:	${src_d}/view.c				\
			${src_d}/aux.c ${src_d}/specific.h	\
			${util_l}
	${aux_c} ${aux_f} $@ ${src_d}/view.c ${src_d}/aux.c ${aux_l}

#---------------------------------------------------------------#
# generic library						#
#---------------------------------------------------------------#

# @ gen_c : compiler for generic library
# @ gen_f : flags for generic library

gen_f		=	-g ${shr_f} -I${generic_d}/hdr -c

${util_l}	:	${util_l}(arg.o)			\
			${util_l}(gfx.o)			\
			${util_l}(io.o)				\
			${util_l}(misc.o)			\
			${util_l}(sch.o)			\
			${util_l}(str.o)
	${ranlib} ${util_l}

${util_l}(arg.o) : ${generic_d}/util/arg.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/arg.c
	${ar} ${ar_f} ${util_l} arg.o
	@rm -f arg.o

${util_l}(gfx.o) : ${generic_d}/util/gfx.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/gfx.c
	${ar} ${ar_f} ${util_l} gfx.o
	@rm -f gfx.o

${util_l}(io.o) :	${generic_d}/util/io.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/io.c
	${ar} ${ar_f} ${util_l} io.o
	@rm -f io.o

${util_l}(misc.o) : ${generic_d}/util/misc.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/misc.c
	${ar} ${ar_f} ${util_l} misc.o
	@rm -f misc.o

${util_l}(sch.o) : ${generic_d}/util/sch.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/sch.c
	${ar} ${ar_f} ${util_l} sch.o
	@rm -f sch.o

${util_l}(str.o) : ${generic_d}/util/str.c ${generic_h}
	${gen_c} ${gen_f} ${generic_d}/util/str.c
	${ar} ${ar_f} ${util_l} str.o
	@rm -f str.o

#---------------------------------------------------------------#
# application library						#
#---------------------------------------------------------------#

# @ app_f : flags for platform application library

app_f		=	${shr_f} -I${plat_d}/hdr -L${plat_d}/lib

${app_l}	:	${app_l}(app.${O})			\
			${app_l}(elastic.${O})			\
			${app_l}(gauss.${O})			\
			${app_l}(half.${O})			\
			${app_l}(invperc.${O})			\
			${app_l}(life.${O})			\
			${app_l}(mandel.${O})			\
			${app_l}(norm.${O})			\
			${app_l}(outer.${O})			\
			${app_l}(product.${O})			\
			${app_l}(randmat.${O})			\
			${app_l}(sor.${O})			\
			${app_l}(thresh.${O})			\
			${app_l}(vecdiff.${O})			\
			${app_l}(winnow.${O})
	${ranlib} ${app_l}

${app_l}(app.${O}) :	${app_d}/app.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/app.${C}
	${ar} ${ar_f} ${app_l} app.${O}
	@rm -f app.${O}

${app_l}(elastic.${O}) : ${app_d}/elastic.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/elastic.${C}
	${ar} ${ar_f} ${app_l} elastic.${O}
	@rm -f elastic.${O}

${app_l}(gauss.${O}) :	${app_d}/gauss.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/gauss.${C}
	${ar} ${ar_f} ${app_l} gauss.${O}
	@rm -f gauss.${O}

${app_l}(half.${O}) :	${app_d}/half.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/half.${C}
	${ar} ${ar_f} ${app_l} half.${O}
	@rm -f half.${O}

${app_l}(invperc.${O}) : ${app_d}/invperc.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/invperc.${C}
	${ar} ${ar_f} ${app_l} invperc.${O}
	@rm -f invperc.${O}

${app_l}(life.${O}) :	${app_d}/life.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/life.${C}
	${ar} ${ar_f} ${app_l} life.${O}
	@rm -f life.${O}

${app_l}(mandel.${O}) :	${app_d}/mandel.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/mandel.${C}
	${ar} ${ar_f} ${app_l} mandel.${O}
	@rm -f mandel.${O}

${app_l}(norm.${O}) : ${app_d}/norm.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/norm.${C}
	${ar} ${ar_f} ${app_l} norm.${O}
	@rm -f norm.${O}

${app_l}(outer.${O}) :	${app_d}/outer.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/outer.${C}
	${ar} ${ar_f} ${app_l} outer.${O}
	@rm -f outer.${O}

${app_l}(product.${O}) : ${app_d}/product.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/product.${C}
	${ar} ${ar_f} ${app_l} product.${O}
	@rm -f product.${O}

${app_l}(randmat.${O}) : ${app_d}/randmat.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/randmat.${C}
	${ar} ${ar_f} ${app_l} randmat.${O}
	@rm -f randmat.${O}

${app_l}(sor.${O}) :	${app_d}/sor.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/sor.${C}
	${ar} ${ar_f} ${app_l} sor.${O}
	@rm -f sor.${O}

${app_l}(thresh.${O}) :	${app_d}/thresh.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/thresh.${C}
	${ar} ${ar_f} ${app_l} thresh.${O}
	@rm -f thresh.${O}

${app_l}(vecdiff.${O}) : ${app_d}/vecdiff.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/vecdiff.${C}
	${ar} ${ar_f} ${app_l} vecdiff.${O}
	@rm -f vecdiff.${O}

${app_l}(winnow.${O}) :	${app_d}/winnow.${C} ${app_h}
	${cc} ${app_f} ${obj_f} ${app_d}/winnow.${C}
	${ar} ${ar_f} ${app_l} winnow.${O}
	@rm -f winnow.${O}

#---------------------------------------------------------------#
# executable binaries						#
#---------------------------------------------------------------#

# @ bin_f : flags for platform binaries
# @ bin_l : libraries for platform binaries

bin_f		=	${shr_f} -I${plat_d}/hdr -L${plat_d}/lib
bin_l		=	${app_lf} ${sys_l} ${plat_l}

binaries	:	${bin_d}/chain				\
			${bin_d}/elastic			\
			${bin_d}/gauss				\
			${bin_d}/half				\
			${bin_d}/invperc			\
			${bin_d}/life				\
			${bin_d}/mandel				\
			${bin_d}/norm				\
			${bin_d}/outer				\
			${bin_d}/product			\
			${bin_d}/randmat			\
			${bin_d}/sor				\
			${bin_d}/thresh				\
			${bin_d}/vecdiff			\
			${bin_d}/winnow

${bin_d}/chain	:	${main_d}/chain.${C}			\
			${app_l} ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/chain.${C} ${bin_l}

${bin_d}/elastic :	${main_d}/elastic.${C}			\
			${app_l}(elastic.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/elastic.${C} ${bin_l}

${bin_d}/gauss	:	${main_d}/gauss.${C}			\
			${app_l}(gauss.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/gauss.${C} ${bin_l}

${bin_d}/half	:	${main_d}/half.${C}			\
			${app_l}(half.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/half.${C} ${bin_l}

${bin_d}/invperc :	${main_d}/invperc.${C}			\
			${app_l}(invperc.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/invperc.${C} ${bin_l}

${bin_d}/life	:	${main_d}/life.${C}			\
			${app_l}(life.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/life.${C} ${bin_l}

${bin_d}/mandel	:	${main_d}/mandel.${C}			\
			${app_l}(mandel.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/mandel.${C} ${bin_l}

${bin_d}/norm	:	${main_d}/norm.${C}			\
			${app_l}(norm.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/norm.${C} ${bin_l}

${bin_d}/outer	:	${main_d}/outer.${C}			\
			${app_l}(outer.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/outer.${C} ${bin_l}

${bin_d}/product :	${main_d}/product.${C}			\
			${app_l}(product.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/product.${C} ${bin_l}

${bin_d}/randmat :	${main_d}/randmat.${C}			\
			${app_l}(randmat.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/randmat.${C} ${bin_l}

${bin_d}/sor	:	${main_d}/sor.${C}			\
			${app_l}(sor.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/sor.${C} ${bin_l}

${bin_d}/thresh	:	${main_d}/thresh.${C}			\
			${app_l}(thresh.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/thresh.${C} ${bin_l}

${bin_d}/vecdiff :	${main_d}/vecdiff.${C}			\
			${app_l}(vecdiff.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/vecdiff.${C} ${bin_l}

${bin_d}/winnow	:	${main_d}/winnow.${C}			\
			${app_l}(winnow.${O})			\
			${app_l}(app.${O}) ${util_l} ${app_h}
	${cc} ${bin_f} ${exe_f} $@ ${main_d}/winnow.${C} ${bin_l}

#---------------------------------------------------------------#
# tests : re-make test files					#
#---------------------------------------------------------------#

tests_d		=	ser bar fj

alltests	:
	@for p in ${tests_d};  do				\
	  make -f Makefile tests dir=$$p;			\
	done

tests		:
	@if (test ! -d ${dir}); then				\
	  echo "no such directory: " ${dir};			\
	elif (test ! -d ${dir}/test); then			\
	  echo "no test directory: " ${dir}/test;		\
	else							\
	  cd ${dir};						\
	  make -f ../Makefile					\
		tests_p t_d=`pwd`/test b_d=`pwd`/bin;		\
	fi

tests_p	:	${t_d}/Avim.r0 ${t_d}/Avtm.r0			\
		${t_d}/Avir.r0 ${t_d}/Avtr.r0			\
		${t_d}/IMvim.r0 ${t_d}/TMvtm.r0			\
		${t_d}/IRvir.r0 ${t_d}/TRvtr.r0

#---------------------------------------------------------------#
# mandel							#
#---------------------------------------------------------------#

${t_d}/Am.i2	:	${b_d}/mandel
	${b_d}/mandel -M -1.5 -1.5 3.0 3.0 -S 128 128 -o ${t_d}/Am.i2

${t_d}/Ahm.i2	:	${b_d}/half ${t_d}/Am.i2
	${b_d}/half -i ${t_d}/Am.i2 -o ${t_d}/Ahm.i2

#---------------------------------------------------------------#
# invperc(mandel)						#
#---------------------------------------------------------------#

${t_d}/Aim.b2	:	${b_d}/invperc ${t_d}/Ahm.i2
	${b_d}/invperc -F 0.5 -i ${t_d}/Ahm.i2 -o ${t_d}/Aim.b2

${t_d}/Alim.b2	:	${b_d}/life ${t_d}/Aim.b2
	${b_d}/life -L 50 -i ${t_d}/Aim.b2 -o ${t_d}/Alim.b2

${t_d}/Awim.p1	:	${b_d}/winnow ${t_d}/Am.i2 ${t_d}/Alim.b2
	${b_d}/winnow -N 20 -i ${t_d}/Am.i2 ${t_d}/Alim.b2 -o ${t_d}/Awim.p1

${t_d}/Anim.p1	:	${b_d}/norm ${t_d}/Awim.p1
	${b_d}/norm -i ${t_d}/Awim.p1 -o ${t_d}/Anim.p1

${t_d}/Aeim.p1	:	${b_d}/elastic ${t_d}/Anim.p1
	${b_d}/elastic -E 500 50 -i ${t_d}/Anim.p1 -o ${t_d}/Aeim.p1

${t_d}/Aoim.r2 ${t_d}/Aoim.r1 :${b_d}/outer ${t_d}/Aeim.p1
	${b_d}/outer -i ${t_d}/Aeim.p1 -o ${t_d}/Aoim.r2 ${t_d}/Aoim.r1

${t_d}/Agim.r1	:	${b_d}/gauss ${t_d}/Aoim.r2 ${t_d}/Aoim.r1
	${b_d}/gauss -i ${t_d}/Aoim.r2 ${t_d}/Aoim.r1 -o ${t_d}/Agim.r1

${t_d}/Apgim.r1	:	${b_d}/product ${t_d}/Aoim.r2 ${t_d}/Agim.r1
	${b_d}/product -i ${t_d}/Aoim.r2 ${t_d}/Agim.r1 -o ${t_d}/Apgim.r1

${t_d}/Asim.r1	:	${b_d}/sor ${t_d}/Aoim.r2 ${t_d}/Aoim.r1
	${b_d}/sor -T 0.000001 -i ${t_d}/Aoim.r2 ${t_d}/Aoim.r1 -o ${t_d}/Asim.r1

${t_d}/Apsim.r1	:	${b_d}/product ${t_d}/Aoim.r2 ${t_d}/Asim.r1
	${b_d}/product -i ${t_d}/Aoim.r2 ${t_d}/Asim.r1 -o ${t_d}/Apsim.r1

${t_d}/Avim.r0	:	${b_d}/vecdiff ${t_d}/Apgim.r1 ${t_d}/Apsim.r1
	${b_d}/vecdiff -i ${t_d}/Apgim.r1 ${t_d}/Apsim.r1 -o ${t_d}/Avim.r0

#---------------------------------------------------------------#
# thresh(mandel)						#
#---------------------------------------------------------------#

${t_d}/Atm.b2	:	${b_d}/thresh ${t_d}/Ahm.i2
	${b_d}/thresh -F 0.5 -i ${t_d}/Ahm.i2 -o ${t_d}/Atm.b2

${t_d}/Altm.b2	:	${b_d}/life ${t_d}/Atm.b2
	${b_d}/life -L 50 -i ${t_d}/Atm.b2 -o ${t_d}/Altm.b2

${t_d}/Awtm.p1	:	${b_d}/winnow ${t_d}/Am.i2 ${t_d}/Altm.b2
	${b_d}/winnow -N 20 -i ${t_d}/Am.i2 ${t_d}/Altm.b2 -o ${t_d}/Awtm.p1

${t_d}/Antm.p1	:	${b_d}/norm ${t_d}/Awtm.p1
	${b_d}/norm -i ${t_d}/Awtm.p1 -o ${t_d}/Antm.p1

${t_d}/Aetm.p1	:	${b_d}/elastic ${t_d}/Antm.p1
	${b_d}/elastic -E 500 50 -i ${t_d}/Antm.p1 -o ${t_d}/Aetm.p1

${t_d}/Aotm.r2 ${t_d}/Aotm.r1 :${b_d}/outer ${t_d}/Aetm.p1
	${b_d}/outer -i ${t_d}/Aetm.p1 -o ${t_d}/Aotm.r2 ${t_d}/Aotm.r1

${t_d}/Agtm.r1	:	${b_d}/gauss ${t_d}/Aotm.r2 ${t_d}/Aotm.r1
	${b_d}/gauss -i ${t_d}/Aotm.r2 ${t_d}/Aotm.r1 -o ${t_d}/Agtm.r1

${t_d}/Apgtm.r1	:	${b_d}/product ${t_d}/Aotm.r2 ${t_d}/Agtm.r1
	${b_d}/product -i ${t_d}/Aotm.r2 ${t_d}/Agtm.r1 -o ${t_d}/Apgtm.r1

${t_d}/Astm.r1	:	${b_d}/sor ${t_d}/Aotm.r2 ${t_d}/Aotm.r1
	${b_d}/sor -T 0.000001 -i ${t_d}/Aotm.r2 ${t_d}/Aotm.r1 -o ${t_d}/Astm.r1

${t_d}/Apstm.r1	:	${b_d}/product ${t_d}/Aotm.r2 ${t_d}/Astm.r1
	${b_d}/product -i ${t_d}/Aotm.r2 ${t_d}/Astm.r1 -o ${t_d}/Apstm.r1

${t_d}/Avtm.r0	:	${b_d}/vecdiff ${t_d}/Apgtm.r1 ${t_d}/Apstm.r1
	${b_d}/vecdiff -i ${t_d}/Apgtm.r1 ${t_d}/Apstm.r1 -o ${t_d}/Avtm.r0

#---------------------------------------------------------------#
# randmat							#
#---------------------------------------------------------------#

${t_d}/Ar.i2	:	${b_d}/randmat
	${b_d}/randmat -R 128 7493418 -S 128 128 -o ${t_d}/Ar.i2

${t_d}/Ahr.i2	:	${b_d}/half ${t_d}/Ar.i2
	${b_d}/half -i ${t_d}/Ar.i2 -o ${t_d}/Ahr.i2

#---------------------------------------------------------------#
# invperc(randmat)						#
#---------------------------------------------------------------#

${t_d}/Air.b2	:	${b_d}/invperc ${t_d}/Ahr.i2
	${b_d}/invperc -F 0.5 -i ${t_d}/Ahr.i2 -o ${t_d}/Air.b2

${t_d}/Alir.i2	:	${b_d}/life ${t_d}/Air.b2
	${b_d}/life -L 50 -i ${t_d}/Air.b2 -o ${t_d}/Alir.i2

${t_d}/Awir.p1	:	${b_d}/winnow ${t_d}/Ar.i2 ${t_d}/Alir.i2
	${b_d}/winnow -N 20 -i ${t_d}/Ar.i2 ${t_d}/Alir.i2 -o ${t_d}/Awir.p1

${t_d}/Anir.p1	:	${b_d}/norm ${t_d}/Awir.p1
	${b_d}/norm -i ${t_d}/Awir.p1 -o ${t_d}/Anir.p1

${t_d}/Aeir.p1	:	${b_d}/elastic ${t_d}/Anir.p1
	${b_d}/elastic -E 500 50 -i ${t_d}/Anir.p1 -o ${t_d}/Aeir.p1

${t_d}/Aoir.r2 ${t_d}/Aoir.r1 :${b_d}/outer ${t_d}/Aeir.p1
	${b_d}/outer -i ${t_d}/Aeir.p1 -o ${t_d}/Aoir.r2 ${t_d}/Aoir.r1

${t_d}/Agir.r1	:	${b_d}/gauss ${t_d}/Aoir.r2 ${t_d}/Aoir.r1
	${b_d}/gauss -i ${t_d}/Aoir.r2 ${t_d}/Aoir.r1 -o ${t_d}/Agir.r1

${t_d}/Apgir.r1	:	${b_d}/product ${t_d}/Aoir.r2 ${t_d}/Agir.r1
	${b_d}/product -i ${t_d}/Aoir.r2 ${t_d}/Agir.r1 -o ${t_d}/Apgir.r1

${t_d}/Asir.r1	:	${b_d}/sor ${t_d}/Aoir.r2 ${t_d}/Aoir.r1
	${b_d}/sor -T 0.000001 -i ${t_d}/Aoir.r2 ${t_d}/Aoir.r1 -o ${t_d}/Asir.r1

${t_d}/Apsir.r1	:	${b_d}/product ${t_d}/Aoir.r2 ${t_d}/Asir.r1
	${b_d}/product -i ${t_d}/Aoir.r2 ${t_d}/Asir.r1 -o ${t_d}/Apsir.r1

${t_d}/Avir.r0	:	${b_d}/vecdiff ${t_d}/Apgir.r1 ${t_d}/Apsir.r1
	${b_d}/vecdiff -i ${t_d}/Apgir.r1 ${t_d}/Apsir.r1 -o ${t_d}/Avir.r0

#---------------------------------------------------------------#
# thresh(randmat)						#
#---------------------------------------------------------------#

${t_d}/Atr.b2	:	${b_d}/thresh ${t_d}/Ahr.i2
	${b_d}/thresh -F 0.5 -i ${t_d}/Ahr.i2 -o ${t_d}/Atr.b2

${t_d}/Altr.i2	:	${b_d}/life ${t_d}/Atr.b2
	${b_d}/life -L 50 -i ${t_d}/Atr.b2 -o ${t_d}/Altr.i2

${t_d}/Awtr.p1	:	${b_d}/winnow ${t_d}/Ar.i2 ${t_d}/Altr.i2
	${b_d}/winnow -N 20 -i ${t_d}/Ar.i2 ${t_d}/Altr.i2 -o ${t_d}/Awtr.p1

${t_d}/Antr.p1	:	${b_d}/norm ${t_d}/Awtr.p1
	${b_d}/norm -i ${t_d}/Awtr.p1 -o ${t_d}/Antr.p1

${t_d}/Aetr.p1	:	${b_d}/elastic ${t_d}/Antr.p1
	${b_d}/elastic -E 500 50 -i ${t_d}/Antr.p1 -o ${t_d}/Aetr.p1

${t_d}/Aotr.r2 ${t_d}/Aotr.r1 :${b_d}/outer ${t_d}/Aetr.p1
	${b_d}/outer -i ${t_d}/Aetr.p1 -o ${t_d}/Aotr.r2 ${t_d}/Aotr.r1

${t_d}/Agtr.r1	:	${b_d}/gauss ${t_d}/Aotr.r2 ${t_d}/Aotr.r1
	${b_d}/gauss -i ${t_d}/Aotr.r2 ${t_d}/Aotr.r1 -o ${t_d}/Agtr.r1

${t_d}/Apgtr.r1	:	${b_d}/product ${t_d}/Aotr.r2 ${t_d}/Agtr.r1
	${b_d}/product -i ${t_d}/Aotr.r2 ${t_d}/Agtr.r1 -o ${t_d}/Apgtr.r1

${t_d}/Astr.r1	:	${b_d}/sor ${t_d}/Aotr.r2 ${t_d}/Aotr.r1
	${b_d}/sor -T 0.000001 -i ${t_d}/Aotr.r2 ${t_d}/Aotr.r1 -o ${t_d}/Astr.r1

${t_d}/Apstr.r1	:	${b_d}/product ${t_d}/Aotr.r2 ${t_d}/Astr.r1
	${b_d}/product -i ${t_d}/Aotr.r2 ${t_d}/Astr.r1 -o ${t_d}/Apstr.r1

${t_d}/Avtr.r0	:	${b_d}/vecdiff ${t_d}/Apgtr.r1 ${t_d}/Apstr.r1
	${b_d}/vecdiff -i ${t_d}/Apgtr.r1 ${t_d}/Apstr.r1 -o ${t_d}/Avtr.r0

#---------------------------------------------------------------#
# chained							#
#---------------------------------------------------------------#

${t_d}/IMvim.r0	:	${b_d}/chain
	${b_d}/chain -c im -S 128 128 -N 20 -F 0.5 -L 50	\
		-E 500 50 -M -1.5 -1.5 3.0 3.0 -T 0.000001	\
		-d ${t_d}/IM

${t_d}/TMvtm.r0	:	${b_d}/chain
	${b_d}/chain -c tm -S 128 128 -N 20 -F 0.5 -L 50	\
		-E 500 50 -M -1.5 -1.5 3.0 3.0 -T 0.000001	\
		-d ${t_d}/TM

${t_d}/IRvir.r0	:	${b_d}/chain
	${b_d}/chain -c ir -S 128 128 -N 20 -F 0.5 -L 50	\
		-E 500 50 -R 128 7493418 -T 0.000001		\
		-d ${t_d}/IR

${t_d}/TRvtr.r0	:	${b_d}/chain
	${b_d}/chain -c tr -S 128 128 -N 20 -F 0.5 -L 50	\
		-E 500 50 -R 128 7493418 -T 0.000001		\
		-d ${t_d}/TR

#---------------------------------------------------------------#
# compare test results (d_a and d_b)				#
#---------------------------------------------------------------#

chk		= bin/chk

test_cmp	: test_cmp_A

test_cmp_A	: test_cmp_Am test_cmp_Aim test_cmp_Atm		\
		  test_cmp_Ar test_cmp_Air test_cmp_Atr

test_cmp_Am	:
	@echo "Am:" `${chk} -d -t i2 ${d_a}/test/Am.i2 ${d_b}/test/Am.i2`
	@echo "Ahm:" `${chk} -d -t i2 ${d_a}/test/Ahm.i2 ${d_b}/test/Ahm.i2`

test_cmp_Aim	:
	@echo "Aim:" `${chk} -d -t i2 ${d_a}/test/Aim.b2 ${d_b}/test/Aim.b2`
	@echo "Alim:" `${chk} -d -t i2 ${d_a}/test/Alim.b2 ${d_b}/test/Alim.b2`
	@echo "Awim:" `${chk} -d -t p1 ${d_a}/test/Awim.p1 ${d_b}/test/Awim.p1`
	@echo "Anim:" `${chk} -d -t p1 ${d_a}/test/Anim.p1 ${d_b}/test/Anim.p1`
	@echo "Aeim:" `${chk} -d -t p1 ${d_a}/test/Aeim.p1 ${d_b}/test/Aeim.p1`
	@echo "Aoim:" `${chk} -d -t r2 ${d_a}/test/Aoim.r2 ${d_b}/test/Aoim.r2`	\
		     `${chk} -d -t r1 ${d_a}/test/Aoim.r1 ${d_b}/test/Aoim.r1`
	@echo "Agim:" `${chk} -d -t r1 ${d_a}/test/Agim.r1 ${d_b}/test/Agim.r1`
	@echo "Apgim:" `${chk} -d -t r1 ${d_a}/test/Apgim.r1 ${d_b}/test/Apgim.r1`
	@echo "Asim:" `${chk} -d -t r1 ${d_a}/test/Asim.r1 ${d_b}/test/Asim.r1`
	@echo "Apsim:" `${chk} -d -t r1 ${d_a}/test/Apsim.r1 ${d_b}/test/Apsim.r1`
	@echo "Avim:" `${chk} -d -t r0 ${d_a}/test/Avim.r0 ${d_b}/test/Avim.r0`

test_cmp_Atm	:
	@echo "Atm:" `${chk} -d -t i2 ${d_a}/test/Atm.b2 ${d_b}/test/Atm.b2`
	@echo "Altm:" `${chk} -d -t i2 ${d_a}/test/Altm.b2 ${d_b}/test/Altm.b2`
	@echo "Awtm:" `${chk} -d -t p1 ${d_a}/test/Awtm.p1 ${d_b}/test/Awtm.p1`
	@echo "Antm:" `${chk} -d -t p1 ${d_a}/test/Antm.p1 ${d_b}/test/Antm.p1`
	@echo "Aetm:" `${chk} -d -t p1 ${d_a}/test/Aetm.p1 ${d_b}/test/Aetm.p1`
	@echo "Aotm:" `${chk} -d -t r2 ${d_a}/test/Aotm.r2 ${d_b}/test/Aotm.r2`	\
		     `${chk} -d -t r1 ${d_a}/test/Aotm.r1 ${d_b}/test/Aotm.r1`
	@echo "Agtm:" `${chk} -d -t r1 ${d_a}/test/Agtm.r1 ${d_b}/test/Agtm.r1`
	@echo "Apgtm:" `${chk} -d -t r1 ${d_a}/test/Apgtm.r1 ${d_b}/test/Apgtm.r1`
	@echo "Astm:" `${chk} -d -t r1 ${d_a}/test/Astm.r1 ${d_b}/test/Astm.r1`
	@echo "Apstm:" `${chk} -d -t r1 ${d_a}/test/Apstm.r1 ${d_b}/test/Apstm.r1`
	@echo "Avtm:" `${chk} -d -t r0 ${d_a}/test/Avtm.r0 ${d_b}/test/Avtm.r0`

test_cmp_Ar	:
	@echo "Ar:" `${chk} -d -t i2 ${d_a}/test/Ar.i2 ${d_b}/test/Ar.i2`
	@echo "Ahr:" `${chk} -d -t i2 ${d_a}/test/Ahr.i2 ${d_b}/test/Ahr.i2`

test_cmp_Air	:
	@echo "Air:" `${chk} -d -t i2 ${d_a}/test/Air.b2 ${d_b}/test/Air.b2`
	@echo "Alir:" `${chk} -d -t i2 ${d_a}/test/Alir.i2 ${d_b}/test/Alir.i2`
	@echo "Awir:" `${chk} -d -t p1 ${d_a}/test/Awir.p1 ${d_b}/test/Awir.p1`
	@echo "Anir:" `${chk} -d -t p1 ${d_a}/test/Anir.p1 ${d_b}/test/Anir.p1`
	@echo "Aeir:" `${chk} -d -t p1 ${d_a}/test/Aeir.p1 ${d_b}/test/Aeir.p1`
	@echo "Aoir:" `${chk} -d -t r2 ${d_a}/test/Aoir.r2 ${d_b}/test/Aoir.r2`	\
		     `${chk} -d -t r1 ${d_a}/test/Aoir.r1 ${d_b}/test/Aoir.r1`
	@echo "Agir:" `${chk} -d -t r1 ${d_a}/test/Agir.r1 ${d_b}/test/Agir.r1`
	@echo "Apgir:" `${chk} -d -t r1 ${d_a}/test/Apgir.r1 ${d_b}/test/Apgir.r1`
	@echo "Asir:" `${chk} -d -t r1 ${d_a}/test/Asir.r1 ${d_b}/test/Asir.r1`
	@echo "Apsir:" `${chk} -d -t r1 ${d_a}/test/Apsir.r1 ${d_b}/test/Apsir.r1`
	@echo "Avir:" `${chk} -d -t r0 ${d_a}/test/Avir.r0 ${d_b}/test/Avir.r0`

test_cmp_Atr	:
	@echo "Atr:" `${chk} -d -t i2 ${d_a}/test/Atr.b2 ${d_b}/test/Atr.b2`
	@echo "Altr:" `${chk} -d -t i2 ${d_a}/test/Altr.i2 ${d_b}/test/Altr.i2`
	@echo "Awtr:" `${chk} -d -t p1 ${d_a}/test/Awtr.p1 ${d_b}/test/Awtr.p1`
	@echo "Antr:" `${chk} -d -t p1 ${d_a}/test/Antr.p1 ${d_b}/test/Antr.p1`
	@echo "Aetr:" `${chk} -d -t p1 ${d_a}/test/Aetr.p1 ${d_b}/test/Aetr.p1`
	@echo "Aotr:" `${chk} -d -t r2 ${d_a}/test/Aotr.r2 ${d_b}/test/Aotr.r2`	\
		     `${chk} -d -t r1 ${d_a}/test/Aotr.r1 ${d_b}/test/Aotr.r1`
	@echo "Agtr:" `${chk} -d -t r1 ${d_a}/test/Agtr.r1 ${d_b}/test/Agtr.r1`
	@echo "Apgtr:" `${chk} -d -t r1 ${d_a}/test/Apgtr.r1 ${d_b}/test/Apgtr.r1`
	@echo "Astr:" `${chk} -d -t r1 ${d_a}/test/Astr.r1 ${d_b}/test/Astr.r1`
	@echo "Apstr:" `${chk} -d -t r1 ${d_a}/test/Apstr.r1 ${d_b}/test/Apstr.r1`
	@echo "Avtr:" `${chk} -d -t r0 ${d_a}/test/Avtr.r0 ${d_b}/test/Avtr.r0`

#---------------------------------------------------------------#
# compare test results (internal consistency)			#
#---------------------------------------------------------------#

chk		= bin/chk

test_con	:
	@echo "A[gs]im:" `${chk} -d -t r1 ${dir}/test/Agim.r1 ${dir}/test/Asim.r1`
	@echo "Ap[gs]im:" `${chk} -d -t r1 ${dir}/test/Apgim.r1 ${dir}/test/Apsim.r1`
	@echo "A[gs]tm:" `${chk} -d -t r1 ${dir}/test/Agtm.r1 ${dir}/test/Astm.r1`
	@echo "Ap[gs]tm:" `${chk} -d -t r1 ${dir}/test/Apgtm.r1 ${dir}/test/Apstm.r1`
	@echo "A[gs]ir:" `${chk} -d -t r1 ${dir}/test/Agir.r1 ${dir}/test/Asir.r1`
	@echo "Ap[gs]ir:" `${chk} -d -t r1 ${dir}/test/Apgir.r1 ${dir}/test/Apsir.r1`
	@echo "A[gs]tr:" `${chk} -d -t r1 ${dir}/test/Agtr.r1 ${dir}/test/Astr.r1`
	@echo "Ap[gs]tr:" `${chk} -d -t r1 ${dir}/test/Apgtr.r1 ${dir}/test/Apstr.r1`

#---------------------------------------------------------------#
# make release							#
#---------------------------------------------------------------#

pack		:
	tar czf cow.tar.gz Makefile aux				\
		bar/*/*.[CH] fj/*/*.[CH] ser/*/*.[CH]		\
		generic/*/*.[CH]

unpack		:
	mkdir	bar/bin bar/lib bar/test			\
		fj/bin fj/lib fj/test				\
		ser/bin ser/lib ser/test			\
		generic/lib bin
