# Notes on Chip Design using Open Source Tools

This directory contains a collection of scripts/notes on open source tools for chip-design.

## Tools

* **[Yosys](https://github.com/YosysHQ/yosys)**: Framework for Verilog RTL synthesis.
* **[Graywolf](https://github.com/rubund/graywolf)**: For floorplanning, placement, cts.
* **[QRouter](https://github.com/RTimothyEdwards/qrouter)**: Detail router for digital ASIC designs
* **[OpenTimer](https://github.com/OpenTimer/OpenTimer)**: Timing Analysis Tool for VLSI Designs
* **[MAGIC](https://github.com/RTimothyEdwards/magic)**: Magic Layout Viewer for standard cells mapped viewer, DRC Checks GDS Generation.
* **[ngSPICE](https://git.code.sf.net/p/ngspice/ngspice)**: Pre, post layout simulation.
* **[eSim](https://github.com/FOSSEE/eSim)**: Circuit Design, Schematic simulation
* **[Qflow](https://github.com/RTimothyEdwards/qflow)**: Complete RTL2GDS Flow scripts

## High-Level approach

* **Design Intent**: Verilog RTL description of a design -> Yosys -> Gate Level Net-list
* **Floorplanning**: Design scope limitations of Gate Level Net-list using a particular standard cell library.
* **Placement**: Placement of the synthesized net-list standard cells within the bounds of the floor-plan. 
* **CTS**: Derive a clock tree of the placed design to minimize clock skew and ensure viability of the design on the silicon.
* **Routing**: Routing of other net-lists for data/control flows of the design.

Between each of these stages a common analysis flow is ensure that the design meets the criteria called the STA or the Static Timing Analysis.

* **Final Stage Sign-off**: Sign-off of the design once everything is clean at every stage of the design flow.