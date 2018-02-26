CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11
INCLUDE = -Iinclude
LDFLAGS = 
CUBA=/Users/Dan/MonteCarlo/projects/FnFast/Cuba-4.2/
GSLINC=/usr/local/include/
GSLLIB=/usr/local/lib/

# rule for object files
%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -I$(CUBA) -I$(GSLINC) $< -o $@ -L$(CUBA) -lcuba -L$(GSLLIB) -lgsl

# executables
all: trispectrum_example

trispectrum_example:  trispectrum_example.o KernelBase.o SPTkernels.o EFTkernels.o Diagram.o Random.o Labels.o Momentum.o Propagator.o LinearPowerSpectrumCAMB.o PowerSpectrum.o Bispectrum.o Trispectrum.o InterpolatingFunction.o
	mkdir -p bin
	$(CXX) -o bin/$@ $^ $(CXXFLAGS) -L$(CUBA) -lcuba -L$(GSLLIB) -lgsl

clean:
	rm -f *.o
