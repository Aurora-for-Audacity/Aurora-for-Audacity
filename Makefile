
kiss_fft_dir := thirdyParty/kissfft-1.31
audacity_src_prefix := /home/simone/src
audacity_src_dir := $(audacity_src_prefix)/audacity-2.4.1
audacity_build_dir := $(audacity_src_prefix)/audacity-2.4.1/build/bin/Debug

#modules_path = $(AUDACITY_MODULES_PATH)
modules_path = /usr/share/audacity/modules

CFLAGS = -Wall -std=c++1z -fPIC
CCFLAGS= -Wall -fPIC

ifeq ($(RELEASE),1)
	BIN=Aurora.so
	CFLAGS += -O3
	CCFLAGS += -O3
else
	BIN=Aurorad.so
	CFLAGS += -g -O0
	CCFLAGS += -g -O0
endif

CFLAGS +=-Isrc/Common/include \
         -Isrc/Common/src \
         -Isrc \
         -I$(kiss_fft_dir) \
         -I$(audacity_src_dir)/include \
         -I$(audacity_src_dir)/src \
         -I$(audacity_build_dir)/src \
         -I/usr/local/include \
         -I/usr/local/lib/wx/include/gtk2-unicode-3.1 \
         -I/usr/local/include/wx-3.1 
          
CFLAGS += -DAUDACITY_HEX_VERSION=0x020401 \
          -D_FILE_OFFSET_BITS=64 \
          -DWXUSINGDLL \
          -D__WXGTK__ \
          -DUSE_KISS_FFT=1 \
          -pthread
          
CCFLAGS += -I$(kiss_fft_dir) 

LDFLAGS = -fPIC -DPIC -shared -pthread -export-dynamic \
           -L/usr/local/lib 
           
LIBS = -lwx_gtk2u_xrc-3.1 \
       -lwx_gtk2u_html-3.1 \
       -lwx_gtk2u_qa-3.1 \
       -lwx_gtk2u_adv-3.1 \
       -lwx_gtk2u_core-3.1 \
       -lwx_baseu_xml-3.1 \
       -lwx_baseu_net-3.1 \
       -lwx_baseu-3.1
 
common_dir     := src/Common/src
acparam_dir    := src/AcParameters/src
convolver_dir  := src/Convolver/src
kirkeby_dir    := src/Kirkeby/src
sinesweep_dir  := src/SineSweepGenerator/src
sti_dir        := src/STI/src
thanalyzer_dir := src/TimeHistoryAnalyzer/src
xfunctions_dir := src/XFunctions/src

build_dir := $(audacity_src_prefix)/../builds/Aurora
obj_dir := $(build_dir)/Intermediate
 
common_src     := $(shell find $(common_dir)     -type f -name *.cpp)
acparam_src    := $(shell find $(acparam_dir)    -type f -name *.cpp)
convolver_src  := $(shell find $(convolver_dir)  -type f -name *.cpp)
kirkeby_src    := $(shell find $(kirkeby_dir)    -type f -name *.cpp)
sinesweep_src  := $(shell find $(sinesweep_dir)  -type f -name *.cpp)
sti_src        := $(shell find $(sti_dir)        -type f -name *.cpp)
thanalyzer_src := $(shell find $(thanalyzer_dir) -type f -name *.cpp)
xfunctions_src := $(shell find $(xfunctions_dir) -type f -name *.cpp)

kissfft_src   := $(kiss_fft_dir)/kiss_fft.c \
                 $(kiss_fft_dir)/tools/kiss_fftnd.c \
                 $(kiss_fft_dir)/tools/kiss_fftr.c  

common_obj     := $(patsubst $(common_dir)/%,    $(obj_dir)/Common/%,      $(common_src:.cpp=.o))
acparam_obj    := $(patsubst $(acparam_dir)/%,   $(obj_dir)/AcParam/%,     $(acparam_src:.cpp=.o))
convolver_obj  := $(patsubst $(convolver_dir)/%, $(obj_dir)/Convolver/%,   $(convolver_src:.cpp=.o))
kirkeby_obj    := $(patsubst $(kirkeby_dir)/%,   $(obj_dir)/Kirkeby/%,     $(kirkeby_src:.cpp=.o))
sinesweep_obj  := $(patsubst $(sinesweep_dir)/%, $(obj_dir)/SineSweep/%,   $(sinesweep_src:.cpp=.o))
sti_obj        := $(patsubst $(sti_dir)/%,       $(obj_dir)/Sti/%,         $(sti_src:.cpp=.o))
thanalyzer_obj := $(patsubst $(thanalyzer_dir)/%, $(obj_dir)/ThAnalyzer/%, $(thanalyzer_src:.cpp=.o))
xfunctions_obj := $(patsubst $(xfunctions_dir)/%, $(obj_dir)/Xfn/%,        $(xfunctions_src:.cpp=.o))

kissfft_obj   := $(obj_dir)/kiss_fft/kiss_fft.o \
                 $(obj_dir)/kiss_fft/tools/kiss_fftnd.o \
                 $(obj_dir)/kiss_fft/tools/kiss_fftr.o


all: $(obj_dir) $(BIN)

$(BIN): $(common_obj) $(acparam_obj) $(convolver_obj) $(kirkeby_obj) $(sinesweep_obj) $(sti_obj) $(thanalyzer_obj) $(xfunctions_obj) $(kissfft_obj)
	g++ $(LDFLAGS) -o $@ $^ $(LIBS)

$(obj_dir)/Common/%.o: $(common_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/AcParam/%.o: $(acparam_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/Convolver/%.o: $(convolver_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/Kirkeby/%.o: $(kirkeby_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/SineSweep/%.o: $(sinesweep_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/Sti/%.o: $(sti_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<
	
$(obj_dir)/ThAnalyzer/%.o: $(thanalyzer_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<
	
$(obj_dir)/Xfn/%.o: $(xfunctions_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -o $@ -c $<

$(obj_dir)/kiss_fft/%.o: $(kiss_fft_dir)/%.c
	@mkdir -p $(dir $@)
	cc $(CCFLAGS) -o $@ -c $<

$(obj_dir)/kiss_fft/tools/%.o: $(kiss_fft_dir)/tools/%.c
	@mkdir -p $(dir $@)
	cc $(CCFLAGS) -o $@ -c $<

$(obj_dir):
	@mkdir -p $(obj_dir)

clean:
	rm -rf $(build_dir)/*

install: $(BIN)
	@mkdir -p $(modules_path)
	@cp $< $(modules_path)

uninstall:
	rm -f $(modules_path)/$(BIN)



