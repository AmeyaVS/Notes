
#export VERILATOR_ROOT=$VERILATOR_HOME

# When using SystemC Backend
export SYSTEMC_INCLUDE=$SYSTEMC_HOME/include
export SYSTEMC_LIBDIR=$SYSTEMC_HOME/lib

# Verilate with systemc backend
verilator -Wall --sc our.v

#make OPT_FAST="-Os -march=native" -f Vour.mk Vour__ALL.a

# Build Verilated sources
make -j -C obj_dir/ -f Vour.mk default
make -j -C obj_dir/ -f Vour.mk verilated.o
# Build trampoline for SystemC
make -j -C obj_dir/ -f Vour.mk ../sc_main_verilator.o
# Build dpi calls Object
make -j -C obj_dir/ -f Vour.mk ../add.o

cd obj_dir
# Link the final simulation binary
g++ -L$SYSTEMC_LIBDIR ../sc_main_verilator.o ../add.o Vour__ALL.a verilated.o -o Vour -lsystemc
cd ..
# Run the simulation executable
./obj_dir/Vour
